cmake_minimum_required(VERSION 3.10.0)
project(BossFight LANGUAGES CXX)

add_subdirectory(src/testFile)

set(SourceFiles src/main.cpp)

add_executable(${PROJECT_NAME} ${SourceFiles})
target_link_libraries(${PROJECT_NAME} PUBLIC testFile)

#Compilation and Macros
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_11)
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    message("Debug Enabled")
    target_compile_definitions(${PROJECT_NAME} PRIVATE DEBUG)
    target_compile_options(${PROJECT_NAME} PRIVATE -g -Wall -Wextra -Wpedantic)
else()
    message("Debug Disabled")
    target_compile_options(${PROJECT_NAME} PRIVATE -O3 -finline-functions)
endif()