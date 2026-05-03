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
add_subdirectory(src/Game)

set(SourceFiles src/main.cpp) 
add_executable(${PROJECT_NAME} ${SourceFiles})

# Export dynamic libs correctly
if(UNIX)
    set_target_properties(${PROJECT_NAME} PROPERTIES
    BUILD_RPATH "$ORIGIN"
    INSTALL_RPATH "$ORIGIN")               
elseif(APPLE)
    set_target_properties(${PROJECT_NAME} PROPERTIES
    INSTALL_RPATH "@executable_path")
endif()

#Link libraries
target_link_libraries(${PROJECT_NAME} PUBLIC project_options) 
target_link_libraries(${PROJECT_NAME} PUBLIC Engine)
target_link_libraries(${PROJECT_NAME} PUBLIC Game)


#Shaders
file(MAKE_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Shaders)
set(Shader_Output_Dir ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Shaders)

set(
    Shader_Source_Dir
    ${CMAKE_SOURCE_DIR}/src/assets/Shaders/VertexShader.vert
    ${CMAKE_SOURCE_DIR}/src/assets/Shaders/FragmentShader.frag
)

set(Shader_Output_List "")
foreach(SHADER IN LISTS Shader_Source_Dir)
    get_filename_component(SHADER_NAME "${SHADER}" NAME_WE)
    set(Out_File "${Shader_Output_Dir}/${SHADER_NAME}.spv")
    
    if(WIN32)
        set(GLSLC "$ENV{VULKAN_SDK}/Bin/glslc.exe")
    elseif(UNIX)
        set(GLSLC "$ENV{VULKAN_SDK}/bin/glslc")
    elseif(APPLE)
        # MISSING THIS PART
    endif()
    
    add_custom_command(
        OUTPUT "${Out_File}"
        COMMAND ${GLSLC} "${SHADER}" -o "${Out_File}"
        DEPENDS "${SHADER}"
    )
    list(APPEND Shader_Output_List "${Out_File}")
endforeach()

add_custom_target(shaders ALL DEPENDS ${Shader_Output_List})
add_dependencies(${PROJECT_NAME} shaders)