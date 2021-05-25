cmake_minimum_required(VERSION 3.0.0 FATAL_ERROR)

project (pjbullethell CXX)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-ggdb")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fopenmp")
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${pjbullethell_SOURCE_DIR}/cmake")


INCLUDE(FindPkgConfig)

PKG_SEARCH_MODULE(SDL2 REQUIRED sdl2)
PKG_SEARCH_MODULE(SDL2IMAGE REQUIRED SDL2_image>=2.0.0)

include_directories(${SDL2_INCLUDE_DIRS}  ${SDL2IMAGE_INCLUDE_DIRS})
include_directories("${PROJECT_SOURCE_DIR}/src")

file(GLOB_RECURSE pjbullethell_SOURCES "src/*.cpp")

add_executable(MySuperAwesomeGame ${pjbullethell_SOURCES} )
target_link_libraries(MySuperAwesomeGame ${SDL2_LIBRARIES}  ${SDL2IMAGE_LIBRARIES})

add_custom_command(TARGET MySuperAwesomeGame PRE_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_directory
  ${CMAKE_SOURCE_DIR}/data $<TARGET_FILE_DIR:MySuperAwesomeGame>/data)

add_custom_target(run 
  COMMAND MySuperAwesomeGame 
  DEPENDS MySuperAwesomeGame
  WORKING_DIRECTORY ${CMAKE_PROJECT_DIR})
