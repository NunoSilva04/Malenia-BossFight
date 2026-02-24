cmake_minimum_required(VERSION 3.10.0)
project(BossFight LANGUAGES CXX)

set(SourceFiles src/main.cpp)

if(DEBUG)
    message("Debug Enabled")
endif()

add_executable(${PROJECT_NAME} ${SourceFiles})
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_11)