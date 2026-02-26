set(SourceFiles testFile.cpp)
add_library(testFile ${SourceFiles})

target_include_directories(testFile PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})