cmake_minimum_required (VERSION 2.8)
ENABLE_LANGUAGE(CXX)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -Wall -Wunused -std=c++11 -pthread -D_GLIBCXX_USE_NANOSLEEP")

set(CMAKE_SHARED_LIBRARY_LINK_CXX_FLAGS "")

project (rootproject)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")

find_package(YamlCpp REQUIRED)
include_directories(SYSTEM "${YAMLCPP_INCLUDE_DIR}")

find_package(ZMQ REQUIRED)
include_directories(SYSTEM "${ZMQ_INCLUDE_DIR}")

add_subdirectory( lib )

add_subdirectory( falcon )

