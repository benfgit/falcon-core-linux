cmake_minimum_required (VERSION 2.8)
ENABLE_LANGUAGE(CXX)

MACRO(SUBDIRLIST result curdir)
  FILE(GLOB children RELATIVE ${curdir} ${curdir}/*)
  SET(dirlist "")
  FOREACH(child ${children})
    IF(IS_DIRECTORY ${curdir}/${child})
      LIST(APPEND dirlist ${child})
    ENDIF()
  ENDFOREACH()
  SET(${result} ${dirlist})
ENDMACRO()

project (falcon)

set(PLATFORM_LINK_LIBRARIES rt dl)

add_definitions(-DG2_DYNAMIC_LOGGING)

#
Set( RESOURCES_PATH $ENV{HOME}/.falcon CACHE STRING "")
mark_as_advanced( RESOURCES_PATH )
configure_file(configuration.hpp.in configuration.hpp)


# list of source files
file(GLOB sources *.cpp)
include_directories( "../resources" )
include_directories( "../lib" )
include_directories(".")
include_directories(${CMAKE_CURRENT_BINARY_DIR})

set( DATATYPE_LIBS )
set( PROCESSOR_LIBS )
set( PROCESSOR_DOC )
option(COMPILE_EXTENSIONS "compile all extensions" ON)

if (COMPILE_EXTENSIONS)
    
    
    # get real absolute extension paths
    SET( REAL_FALCON_PATHS, "" )
    
    foreach( FALCON_PATH ${EXTENSION_PATHS} )
      get_filename_component(FALCON_PATH ${FALCON_PATH} REALPATH)
      LIST(APPEND REAL_FALCON_PATHS ${FALCON_PATH})
    endforeach()
    
    # first, let's add all include directories
    foreach( FALCON_PATH ${REAL_FALCON_PATHS} )
      
      MESSAGE( "Adding include directories for extension: ${FALCON_PATH}" )
      
      if (EXISTS ${FALCON_PATH}/lib)
        include_directories( ${FALCON_PATH}/lib )
      endif()

      if (EXISTS ${FALCON_PATH}/resources)
        MESSAGE( "Adding resources for extension: ${FALCON_PATH}" )
        include_directories( ${FALCON_PATH}/resources )
      endif()
      
      if (EXISTS ${FALCON_PATH}/datatypes)
        include_directories( ${FALCON_PATH}/datatypes )
      endif()

      if (EXISTS ${FALCON_PATH}/processors)
        include_directories( ${FALCON_PATH}/processors )
      endif()
      
    endforeach()
    
    # next, let's add all libs in extensions
    SET( ITER 0 )
    
    foreach( FALCON_PATH ${REAL_FALCON_PATHS} )
        if (EXISTS ${FALCON_PATH}/lib/CMakeLists.txt)
          MESSAGE( "Adding libraries for extension: ${FALCON_PATH}" )
          add_subdirectory( ${FALCON_PATH}/lib ${CMAKE_CURRENT_BINARY_DIR}/lib${ITER} )
        endif()
      math(EXPR ITER "${ITER} + 1")
    endforeach()
      
    # next, let's add all datatypes
    
    foreach( FALCON_PATH ${REAL_FALCON_PATHS} )
      
      if (EXISTS ${FALCON_PATH}/datatypes)
        
        MESSAGE( "Adding datatypes for extension: ${FALCON_PATH}" )
        
        SUBDIRLIST( DATATYPES "${FALCON_PATH}/datatypes" )
        
        set(EXCLUDED_ITEMS "")
        if (EXISTS ${FALCON_PATH}/datatypes/exclude.txt)
          FILE(STRINGS ${FALCON_PATH}/datatypes/exclude.txt EXCLUDED_ITEMS)
        endif()
        
        FOREACH( DATATYPE ${DATATYPES} )
          LIST(FIND EXCLUDED_ITEMS ${DATATYPE} DO_EXCLUDE)
          if(DO_EXCLUDE GREATER -1)
            MESSAGE( "Excluding data type: ${DATATYPE}")
            continue()
          else()
            if( EXISTS "${FALCON_PATH}/datatypes/${DATATYPE}/CMakeLists.txt" )
              MESSAGE( "Adding data type: ${DATATYPE}" )
              add_subdirectory(${FALCON_PATH}/datatypes/${DATATYPE} ${CMAKE_CURRENT_BINARY_DIR}/datatypes/${DATATYPE})
              if( TARGET ${DATATYPE} )
                LIST( APPEND DATATYPE_LIBS ${DATATYPE} )
              endif()
            endif()
          endif()
        ENDFOREACH()
        
      endif()
      
    endforeach()
    
    # then, let's add all processors
    
    foreach( FALCON_PATH ${REAL_FALCON_PATHS} )
      
      if (EXISTS ${FALCON_PATH}/processors)
        
        MESSAGE( "Adding processors for extension: ${FALCON_PATH}" )
    
        SUBDIRLIST( PROCESSORS "${FALCON_PATH}/processors" )
        
        set(EXCLUDED_ITEMS "")
        if (EXISTS ${FALCON_PATH}/processors/exclude.txt)
          FILE(STRINGS ${FALCON_PATH}/processors/exclude.txt EXCLUDED_ITEMS)
        endif()

        FOREACH( PROCESSOR ${PROCESSORS} )
          LIST(FIND EXCLUDED_ITEMS ${PROCESSOR} DO_EXCLUDE)
          if(DO_EXCLUDE GREATER -1)
            MESSAGE( "Excluding processor: ${PROCESSOR}")
            continue()
          else()
            if( EXISTS "${FALCON_PATH}/processors/${PROCESSOR}/CMakeLists.txt" )
              MESSAGE( "Adding processor: ${PROCESSOR}" )
              add_subdirectory(${FALCON_PATH}/processors/${PROCESSOR} ${CMAKE_CURRENT_BINARY_DIR}/processors/${PROCESSOR})
              if( TARGET ${PROCESSOR} )
                LIST( APPEND PROCESSOR_LIBS ${PROCESSOR} )
              endif()
              if ( EXISTS "${FALCON_PATH}/processors/${PROCESSOR}/doc.yaml" )
                configure_file(${FALCON_PATH}/processors/${PROCESSOR}/doc.yaml
                            ${CMAKE_CURRENT_BINARY_DIR}/processors/${PROCESSOR} COPYONLY)
              endif()
            endif()
          endif()
        ENDFOREACH()
      
      endif()
    
    endforeach()
    
    # and finally add all tools
    SET( ITER 0 )
    
    foreach( FALCON_PATH ${REAL_FALCON_PATHS} )
    
      if (EXISTS ${FALCON_PATH}/tools/CMakeLists.txt)
        MESSAGE( "Adding tools from extension: ${FALCON_PATH}" )
        add_subdirectory( ${FALCON_PATH}/tools ${CMAKE_CURRENT_BINARY_DIR}/tools${ITER} )
      endif()
      
      math(EXPR ITER "${ITER} + 1")
      
    endforeach()

endif()

set(_processor_doc_path ${CMAKE_CURRENT_BINARY_DIR}/processors/ )
configure_file(buildconstant.hpp.in buildconstant.hpp @ONLY)
# falcon executable

add_executable(falcon ${sources} )

target_link_libraries (falcon logging yaml-cpp zmq ${PLATFORM_LINK_LIBRARIES})

# here we have to use --whole-archive linker option, otherwise the data type and processor libs are not
# linked at all because none of their exported symbols are explicitly used in falcon. Same is true for (part of)
# the disruptor and utilities libraries

SET (WHOLELIBS -Wl,--whole-archive units-static utilities options disruptor ${DATATYPE_LIBS} ${PROCESSOR_LIBS} -Wl,--no-whole-archive)
target_link_libraries (falcon ${WHOLELIBS} )

install(TARGETS falcon RUNTIME DESTINATION bin)

# add all resources
MESSAGE("install files from  ${FALCON_PATH}/resources in  ${RESOURCES_PATH}")

if (COMPILE_EXTENSIONS)
    ADD_CUSTOM_COMMAND(TARGET falcon
              POST_BUILD
              COMMAND mkdir -p ${RESOURCES_PATH}/resources
            )
    foreach( FALCON_PATH ${REAL_FALCON_PATHS} )
            SUBDIRLIST( RESOURCES "${FALCON_PATH}/resources" )

            foreach( RESOURCE ${RESOURCES} )
                    MESSAGE("install files from  ${FALCON_PATH}/resources/${RESOURCE} in  ${RESOURCES_PATH}/resources")
                    ADD_CUSTOM_COMMAND(TARGET falcon
                      POST_BUILD
                      COMMAND cp -r ${FALCON_PATH}/resources/${RESOURCE} ${rootproject_BINARY_DIR}/resources
                    )
                    install(DIRECTORY ${rootproject_BINARY_DIR}/resources/${RESOURCE} DESTINATION /${RESOURCES_PATH}/resources/)
            endforeach()
    endforeach()

    # install in the installation folder




endif()



