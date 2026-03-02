cmake_minimum_required(VERSION 3.10.0)
project(BossFight LANGUAGES C CXX)

#SDL3
#include(FetchContent)
#set(FETCHCONTENT_BASE_DIR "${CMAKE_SOURCE_DIR}/SDL3" CACHE PATH "fetchcontent base dir")
#FetchContent_Declare(
#    SDL3
#    GIT_REPOSITORY https://github.com/libsdl-org/SDL
#    GIT_TAG release-3.4.2
#)
#FetchContent_MakeAvailable(SDL3)

add_subdirectory(SDL EXCLUDE_FROM_ALL)

set(SourceFiles src/main.cpp) 
add_executable(${PROJECT_NAME} ${SourceFiles})

target_link_libraries(${PROJECT_NAME} PRIVATE SDL3::SDL3) 

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