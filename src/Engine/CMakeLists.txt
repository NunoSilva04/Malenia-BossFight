set(SourceFiles window/window.cpp)

add_library(Engine STATIC ${SourceFiles})
target_link_libraries(Engine PUBLIC SDL3::SDL3)
target_include_directories(Engine PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/window)