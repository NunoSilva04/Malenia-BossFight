set(SourceFiles window/window.cpp
                input/input.cpp)

add_library(Engine STATIC ${SourceFiles})
target_link_libraries(Engine PUBLIC SDL3::SDL3)

set(HeaderFiles ${CMAKE_CURRENT_SOURCE_DIR}/window
                ${CMAKE_CURRENT_SOURCE_DIR}/SDL_SubSystem
                ${CMAKE_CURRENT_SOURCE_DIR}/input
                ${CMAKE_SOURCE_DIR}/src/core)
target_include_directories(Engine PUBLIC ${HeaderFiles})
target_link_libraries(Engine PUBLIC project_options)