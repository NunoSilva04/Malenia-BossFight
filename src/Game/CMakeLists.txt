set(source_files
    Game.cpp
    Player.cpp)
add_library(Game STATIC ${source_files})

set(header_files
    ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(Game PUBLIC ${header_files})

target_link_libraries(Game PUBLIC project_options)
target_link_libraries(Game PUBLIC Engine)
target_link_libraries(Game PUBLIC assets_shared)