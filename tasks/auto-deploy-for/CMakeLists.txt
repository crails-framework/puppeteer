cmake_minimum_required(VERSION 3.0)

file(GLOB task_files
     *.cpp)

add_executable(auto-deploy-for ${task_files})

target_link_libraries(auto-deploy-for ${dependencies})

set_target_properties(auto-deploy-for PROPERTIES OUTPUT_NAME "task")
