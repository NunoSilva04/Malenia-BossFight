set(SourceFiles debug_helper.cpp)
add_library(core STATIC ${SourceFiles})

set(HeaderFiles ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(core PUBLIC ${HeaderFiles})