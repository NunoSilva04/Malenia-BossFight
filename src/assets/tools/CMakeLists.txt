cmake_minimum_required(VERSION 3.10.0)
project(assets_loader LANGUAGES C CXX)

set(
    Source_Files
    main.cpp 
    glTF_Helper.cpp 
    File_Format.cpp
)
add_executable(${PROJECT_NAME} ${Source_Files})

set(
    Header_Files
    ${CMAKE_SOURCE_DIR}/third-party-libraries/glTF
)
target_include_directories(${PROJECT_NAME} PUBLIC ${Header_Files})
target_link_libraries(${PROJECT_NAME} PUBLIC project_options)
target_link_libraries(${PROJECT_NAME} PUBLIC core)
target_link_libraries(${PROJECT_NAME} PUBLIC Math_Lib)
target_link_libraries(${PROJECT_NAME} PUBLIC assets_shared)

set(
    Assets_Loader_Output_Dir
    "${CMAKE_SOURCE_DIR}/src/assets"
)
set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${Assets_Loader_Output_Dir})
target_compile_definitions(${PROJECT_NAME} PRIVATE OUTPUT_ASSETS_SRC="${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/assets/assets_src/")

add_custom_command(
    TARGET assets_loader
    POST_BUILD
    COMMAND ${Assets_Loader_Output_Dir}/assets_loader.exe
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/src/assets
    COMMENT "Running assets loader..."
)