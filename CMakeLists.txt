cmake_minimum_required(VERSION 3.10.0)
project(BossFight LANGUAGES CXX)

set(SourceFiles src/main.cpp)

add_executable(${PROJECT_NAME} ${SourceFiles})
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_11)
target_compile_definitions(${PROJECT_NAME} PRIVATE DEBUG=TRUE)