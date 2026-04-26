set(source_files
    Vertex.cpp)
add_library(Math_Lib STATIC ${source_files})

set(header_files
    ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(Math_Lib PUBLIC ${header_files})

target_link_libraries(Math_Lib PUBLIC project_options)