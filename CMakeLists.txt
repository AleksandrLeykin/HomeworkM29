cmake_minimum_required(VERSION 3.26)

set(PROJECT_NAME FineGrainedQueue)

project(${PROJECT_NAME})



add_executable(${PROJECT_NAME} 
                src/main.cpp 
                src/FineGrainedQueue.h)

target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_17)

set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/)
