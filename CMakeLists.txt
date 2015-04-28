cmake_minimum_required(VERSION 3.1)
project(jackdynconagent)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

find_package(Jack REQUIRED)
find_package(YamlCpp REQUIRED)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

file(GLOB SOURCE_FILES "src/*.cpp" "include/*.h" "include/*.hpp")
include_directories(include)

add_executable(jackdynconagent ${SOURCE_FILES})
target_include_directories(jackdynconagent SYSTEM PRIVATE ${YAMLCPP_INCLUDE_DIR} ${JACK_INCLUDE_DIRS})
target_link_libraries(jackdynconagent ${YAMLCPP_LIBRARY} ${JACK_LIBRARIES})
target_compile_options(jackdynconagent PRIVATE ${JACK_DEFINITIONS})
