set(SourceFiles window/window.cpp
                input/input.cpp
                Graphics/Graphics.cpp
                Graphics/Graphics_Helper.cpp)
add_library(Engine STATIC ${SourceFiles})

set(HeaderFiles ${CMAKE_CURRENT_SOURCE_DIR}/window
                ${CMAKE_CURRENT_SOURCE_DIR}/SDL_SubSystem
                ${CMAKE_CURRENT_SOURCE_DIR}/input
                ${CMAKE_CURRENT_SOURCE_DIR}/Graphics
                #Tools 
                ${CMAKE_SOURCE_DIR}/src/Math_Lib)
target_include_directories(Engine PUBLIC ${HeaderFiles})

target_link_libraries(Engine PUBLIC project_options)
target_link_libraries(Engine PUBLIC SDL3::SDL3)
target_link_libraries(Engine PRIVATE Vulkan::Vulkan)
target_link_libraries(Engine PUBLIC core)