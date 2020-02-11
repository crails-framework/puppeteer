cmake_minimum_required(VERSION 3.0)

file(GLOB task_files
     *.cpp)

add_executable(refresh-builds ${task_files})

target_link_libraries(refresh-builds ${dependencies})

set_target_properties(refresh-builds PROPERTIES OUTPUT_NAME "task")
