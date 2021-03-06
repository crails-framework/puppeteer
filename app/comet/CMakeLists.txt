# -DCMAKE_TOOLCHAIN_FILE "$CHEERP_PATH/share/cmake/Modules/CheerpToolchain.cmake"
cmake_minimum_required(VERSION 3.0)

project(puppeteer-frontend)
option(DEVELOPER_MODE "enable debugging tools" ON)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wno-unknown-pragmas -pedantic")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D__CHEERP_CLIENT__ -D__COMET_CLIENT__")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DMODELS_CLIENT_SUPER_CLASS=Comet::ArchiveModel")

include_directories(include /usr/local/include ../../lib/comet ../..)

if(DEVELOPER_MODE)
  set(CMAKE_BUILD_TYPE "Debug")
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -cheerp-sourcemap=application.js.map -cheerp-sourcemap-standalone")
else()
  set(CMAKE_BUILD_TYPE "Release")
 #set(CMAKE_CXX_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -cheerp-preexecute")
endif()

file(GLOB_RECURSE comet_files
     *.cpp
     ../../lib/app/data/shared/*.cpp
     ../../lib/app/data/*.archive.cpp
     ../../lib/comet/*.cpp)

file(GLOB_RECURSE crails_comet_files
     /usr/local/share/crails/front/*.cpp)

add_executable(application ${crails_comet_files} ${comet_files})
