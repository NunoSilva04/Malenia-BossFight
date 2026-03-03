cmake_minimum_required(VERSION 3.10.0)
project(BossFight LANGUAGES C CXX)

#Subdirectories
add_subdirectory(third-party-libraries/SDL EXCLUDE_FROM_ALL)
add_subdirectory(src/Engine)

set(SourceFiles src/main.cpp) 
add_executable(${PROJECT_NAME} ${SourceFiles})

#Link libraries
target_link_libraries(${PROJECT_NAME} PUBLIC Engine) 

#Compilation and Macros
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_11)
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    message("Debug Enabled")
    target_compile_definitions(${PROJECT_NAME} PUBLIC DEBUG)
    target_compile_options(${PROJECT_NAME} PUBLIC -g -Wall -Wextra -Wpedantic)
else()
    message("Debug Disabled")
    target_compile_options(${PROJECT_NAME} PUBLIC -O3 -finline-functions)
endif()