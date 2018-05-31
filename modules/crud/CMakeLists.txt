file (GLOB_RECURSE module_files
      *.cpp)

set(crails_app ${crails_app} ${module_files} PARENT_SCOPE)

