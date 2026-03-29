set(SourceFiles debug_helper.cpp file_system.cpp)
add_library(core STATIC ${SourceFiles})

set(HeaderFiles ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(core PUBLIC ${HeaderFiles})
target_link_libraries(core PUBLIC project_options)