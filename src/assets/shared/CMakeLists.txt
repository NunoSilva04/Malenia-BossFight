add_library(assets_shared STATIC Assets_Helper.cpp Mesh.cpp)

set(
    Header_Files
    ${CMAKE_CURRENT_SOURCE_DIR}
)
target_include_directories(assets_shared PUBLIC ${Header_Files})
target_link_libraries(assets_shared PUBLIC Math_Lib)