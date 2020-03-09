cmake_minimum_required (VERSION 3.11)
ENABLE_LANGUAGE(CXX)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -Wall -Wunused -std=c++11 -pthread -D_GLIBCXX_USE_NANOSLEEP")

set(CMAKE_SHARED_LIBRARY_LINK_CXX_FLAGS "")

project (rootproject)

############################
# Add the basic libraries  #
############################

include(FetchContent)
FetchContent_Declare(
		yaml-cpp
		GIT_REPOSITORY https://github.com/jbeder/yaml-cpp.git
		GIT_SHALLOW	ON
		GIT_TAG yaml-cpp-0.6.0
	)

FetchContent_Declare(
		cppzmq
		GIT_REPOSITORY https://github.com/zeromq/cppzmq.git
		GIT_SHALLOW	ON
		GIT_TAG v4.6.0
	)

FetchContent_GetProperties(yaml-cpp)
if(NOT yaml-cpp_POPULATED)
	message(STATUS "Populating yaml-cpp...")
	FetchContent_Populate(yaml-cpp)
	# Add here options for yaml-cpp building
	set(YAML_CPP_BUILD_TESTS OFF CACHE BOOL "disable yaml tests")
	add_subdirectory(${yaml-cpp_SOURCE_DIR} ${yaml-cpp_BINARY_DIR})
	message(STATUS "Done.")
endif()

include_directories(BEFORE SYSTEM ${yaml-cpp_SOURCE_DIR} ${yaml-cpp_BINARY_DIR}/include)
target_link_libraries(${YAM_CPP} yaml-cpp)


FetchContent_GetProperties(cppzmq)
if(NOT cppzmq_POPULATED)
	message(STATUS "Populating zmq-cpp...")
	FetchContent_Populate(cppzmq)
	# Add here options for cppzmq building
	set(CPPZMQ_BUILD_TESTS OFF CACHE BOOL "Whether or not to build the tests")
	add_subdirectory(${cppzmq_SOURCE_DIR} ${cppzmq_BINARY_DIR})
	message(STATUS "Done.")
endif()

include_directories(BEFORE SYSTEM ${ZMQ_SOURCE_DIR} ${ZMQ_BINARY_DIR}/include)
target_link_libraries(${ZMQ} cppzmq)


###################################
# Define every external extension #
###################################

# FetchContent_Declare(
#		extension
#		GIT_REPOSITORY Local/path/extensions or Github/path/extensions
#	)
#
## Add definitions here ...

###############################################
# Populate -- only populate extensions wanted #
# Dont forget to disable tests                #
###############################################

#FetchContent_GetProperties(extension)
#if(NOT extension_POPULATED)
#	message(STATUS "Populating extension...")
#	FetchContent_Populate(extension)
#	add_subdirectory(${extension_SOURCE_DIR} ${extension_BINARY_DIR})
#	message(STATUS "Done.")
#endif()

#include_directories(BEFORE SYSTEM ${extension_SOURCE_DIR} ${extension_BINARY_DIR}/include)

## Populate new extensions here 


add_subdirectory(lib)
add_subdirectory(falcon)

