cmake_minimum_required(VERSION 3.10.0)
project(BossFight LANGUAGES C CXX)

#Compilation and Macros
add_library(project_options INTERFACE)
target_compile_features(project_options INTERFACE cxx_std_11)
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    message("Debug Enabled")
    target_compile_definitions(project_options INTERFACE DEBUG)
    target_compile_options(project_options INTERFACE -g -Wall -Wextra -Wpedantic -Wconversion -Wsign-conversion)
else()
    message("Debug Disabled")
    target_compile_options(project_options INTERFACE -O3 -finline-functions)
endif()

#Subdirectories
add_subdirectory(third-party-libraries/SDL EXCLUDE_FROM_ALL)
find_package(Vulkan REQUIRED)
add_subdirectory(src/Engine)
add_subdirectory(src/core)

set(SourceFiles src/main.cpp) 
add_executable(${PROJECT_NAME} ${SourceFiles})

#Link libraries
target_link_libraries(${PROJECT_NAME} PUBLIC project_options) 
target_link_libraries(${PROJECT_NAME} PUBLIC Engine)