cmake_minimum_required(VERSION 2.8)

project(puppeteer)
option(ODB_WITH_PGSQL "enables pgsql support for crails-odb" ON)
option(DEVELOPER_MODE "enable debugging tools" ON)
option(WITH_NGINX     "build the nginx plugin" ON)
option(WITH_GANDI     "build the gandi plugin" ON)

set(CMAKE_CXX_FLAGS "-std=c++11 -Wall -Wno-unknown-pragmas -Wno-deprecated-declarations -pedantic ")

find_package(cppnetlib REQUIRED)
find_package(Boost COMPONENTS thread filesystem program_options system random date_time REQUIRED)

include_directories(include /usr/local/include ${CPPNETLIB_INCLUDE_DIRS} ${Boost_INCLUDE_DIRS} .)
link_directories(/usr/local/lib /usr/local/lib64 /usr/lib/odb /usr/local/lib/odb /usr/lib/x86_64-linux-gnu)

if(DEVELOPER_MODE)
  set(CMAKE_BUILD_TYPE "Debug")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DSERVER_DEBUG")
else()
  set(CMAKE_BUILD_TYPE "Release")
endif()

file(GLOB_RECURSE crails_app
     app/controllers/*.cpp
     app/models/*.cpp
     app/models/*.cxx
     app/views/*.cpp
     app/faye/*.cpp
     app/ssh/*.cpp
     app/git/*.cpp
     app/jenkins/*.cpp
     app/recipe_runners/*.cpp
     config/*.cpp
     lib/app/*.cpp
     lib/odb/*.cxx
     lib/renderers/*.cpp)

file(GLOB crails_app_2
     app/routes.cpp
     app/invalid_request.cpp)

file(GLOB server_files
     app/main.cpp)

file(GLOB_RECURSE tests_files
     spec/*.cpp)

# Add your modules here (do not modify this line)
add_subdirectory(modules/crud)
if (WITH_NGINX)
  add_subdirectory(modules/nginx)
endif()
if (WITH_GANDI)
  add_subdirectory(modules/gandi)
endif()

add_library(crails-app SHARED ${crails_app} ${crails_app_2})
add_executable(server ${server_files})
add_executable(tests  ${tests_files})

if(DEVELOPER_MODE)
  set(crails-suffix -debug)
endif()

set(dependencies crails-core${crails-suffix}
                 crails-html${crails-suffix}
                 crails-app
                 ${Boost_LIBRARIES}
                 pthread dl crypto ssl
                 ${CPPNETLIB_LIBRARIES}
                 ${dependencies}
		 odb-pgsql
)

# Custom dependencies (do not modify this line)
set(dependencies ${dependencies} crails-sentry${crails-suffix})
set(dependencies ${dependencies} crails-xmlrpc${crails-suffix})
set(dependencies ${dependencies} crails-sync${crails-suffix})
set(dependencies ${dependencies} crails-sidekic${crails-suffix})
set(dependencies ${dependencies} crails-front${crails-suffix})
set(dependencies ${dependencies} crails-odb${crails-suffix})
set(dependencies ${dependencies} odb)
set(dependencies ${dependencies} ssh)
set(dependencies ${dependencies} git2)
set(dependencies ${dependencies} ${Boost_LIBRARIES})

target_link_libraries(server ${dependencies})
target_link_libraries(tests  crails-tests${crails-suffix} ${dependencies})

if (ODB_WITH_PGSQL)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DODB_WITH_PGSQL")
  #  set(dependencies ${dependencies} odb-pgsql)
endif()

add_subdirectory(tasks/odb_migrate)

add_subdirectory(tasks/sidekic)

add_subdirectory(tasks/auto-deploy-for)

add_subdirectory(tasks/backup)
