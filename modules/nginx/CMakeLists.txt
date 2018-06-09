file (GLOB_RECURSE module_files
      *.cpp)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DWITH_NGINX_PLUGIN" PARENT_SCOPE)

set(crails_app ${crails_app} ${module_files} PARENT_SCOPE)

