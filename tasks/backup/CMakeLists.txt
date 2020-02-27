cmake_minimum_required(VERSION 3.0)

file(GLOB task_files
     *.cpp)

add_executable(backup ${task_files})

target_link_libraries(backup ${dependencies})

set_target_properties(backup PROPERTIES OUTPUT_NAME "task")
