cmake_minimum_required(VERSION 3.10)

project(bossFight)

set(THIRD_PARTY_ASSETS_DIR "C:/vcpkg/packages/third-party-assets_x64-windows")
message(STATUS "third party assets dir = ${THIRD_PARTY_ASSETS_DIR}")
file(COPY "${THIRD_PARTY_ASSETS_DIR}/Include" DESTINATION ${CMAKE_SOURCE_DIR})
file(COPY "${THIRD_PARTY_ASSETS_DIR}/Lib" DESTINATION ${CMAKE_SOURCE_DIR})
file(COPY "${THIRD_PARTY_ASSETS_DIR}/Redistribute" DESTINATION ${CMAKE_SOURCE_DIR})
file(COPY "${THIRD_PARTY_ASSETS_DIR}/Models" DESTINATION ${CMAKE_SOURCE_DIR})


#CPP libraries
#System
add_library(SystemFolder STATIC ${CMAKE_SOURCE_DIR}/System/GameLauncher.cpp ${CMAKE_SOURCE_DIR}/System/Usage.cpp)
target_include_directories(SystemFolder PUBLIC ${CMAKE_SOURCE_DIR}/System)

#WindowsOS
add_library(Windows_OS STATIC ${CMAKE_SOURCE_DIR}/Windows_OS/Windows_OS.cpp)
target_include_directories(Windows_OS PUBLIC ${CMAKE_SOURCE_DIR}/Windows_OS)

#Game
add_library(Game STATIC ${CMAKE_SOURCE_DIR}/Game/Game.cpp)
target_include_directories(Game PUBLIC ${CMAKE_SOURCE_DIR}/Game)

#Input
add_library(InputLib STATIC ${CMAKE_SOURCE_DIR}/Input/Input.cpp ${CMAKE_SOURCE_DIR}/Input/IInput.cpp)
target_include_directories(InputLib PUBLIC ${CMAKE_SOURCE_DIR}/Input)

#Graphics
add_library(Graphics STATIC ${CMAKE_SOURCE_DIR}/Graphics/DirectX.cpp ${CMAKE_SOURCE_DIR}/Graphics/IGraphics.cpp ${CMAKE_SOURCE_DIR}/Graphics/IRender.cpp ${CMAKE_SOURCE_DIR}/Graphics/IResources.cpp)
target_include_directories(Graphics PUBLIC ${CMAKE_SOURCE_DIR}/Graphics)

#Math2
add_library(MathLib STATIC ${CMAKE_SOURCE_DIR}/Math2/Vector.cpp ${CMAKE_SOURCE_DIR}/Math2/Matrix.cpp ${CMAKE_SOURCE_DIR}/Math2/Math2.cpp)
target_include_directories(MathLib PUBLIC ${CMAKE_SOURCE_DIR}/Math2)

#Camera
add_library(Camera STATIC ${CMAKE_SOURCE_DIR}/Game/Camera/Camera.cpp)
target_include_directories(Camera PUBLIC ${CMAKE_SOURCE_DIR}/Game/Camera)

#World
add_library(World STATIC ${CMAKE_SOURCE_DIR}/Game/World/World.cpp
                         ${CMAKE_SOURCE_DIR}/Game/World/Entity.cpp
                         ${CMAKE_SOURCE_DIR}/Game/World/Physics.cpp
                         ${CMAKE_SOURCE_DIR}/Game/World/Mesh.cpp
                         ${CMAKE_SOURCE_DIR}/Game/World/Position.cpp)
target_include_directories(World PUBLIC ${CMAKE_SOURCE_DIR}/Game/World)

#DebugUtils
add_library(DebugUtils STATIC ${CMAKE_SOURCE_DIR}/DebugUtils/DebugUtils.cpp)
target_include_directories(DebugUtils PUBLIC ${CMAKE_SOURCE_DIR}/DebugUtils)

add_executable(bossFight main.cpp)


#All of the libraries from the makefile (-ld3d11 -ld3dcompiler -ldxgi -lshlwapi -lhid -lSDL3 -luser32 -lKernel32 -LLib)
target_link_libraries(bossFight PUBLIC Shlwapi)
target_link_libraries(bossFight PUBLIC hid)
target_link_libraries(Graphics PUBLIC d3d11)
target_link_libraries(Graphics PUBLIC d3dcompiler)
target_link_libraries(Graphics PUBLIC dxgi)

#SDL (for input only at the moment)
target_include_directories(InputLib PUBLIC ${CMAKE_SOURCE_DIR}/Include)
target_link_libraries(InputLib PUBLIC ${CMAKE_SOURCE_DIR}/Lib/SDL_Lib/libSDL3.dll.a)

#FBX (for Mesh class inside world)
target_include_directories(World PUBLIC ${CMAKE_SOURCE_DIR}/Include)
target_link_libraries(World PUBLIC ${CMAKE_SOURCE_DIR}/Lib/release/libfbxsdk.lib)


#Link folder libs to the main project
#main
target_link_libraries(bossFight PUBLIC SystemFolder)
target_link_libraries(bossFight PUBLIC Windows_OS)
target_link_libraries(bossFight PUBLIC Game)
target_link_libraries(bossFight PUBLIC DebugUtils)
#Game
target_link_libraries(Game PUBLIC Windows_OS)
target_link_libraries(Game PUBLIC InputLib)
target_link_libraries(Game PUBLIC Graphics)
target_link_libraries(Game PUBLIC MathLib)
target_link_libraries(Game PUBLIC Camera)
target_link_libraries(Game PUBLIC World)
#Graphics
target_link_libraries(Graphics PUBLIC MathLib)
target_link_libraries(Graphics PUBLIC SystemFolder)
target_link_libraries(Graphics PUBLIC Windows_OS)
#Camera
target_link_libraries(Camera PUBLIC MathLib)
target_link_libraries(Camera PUBLIC InputLib)
target_link_libraries(Camera PUBLIC Windows_OS)
target_link_libraries(Camera PUBLIC Graphics)
#World
target_link_libraries(World PUBLIC Graphics)
target_link_libraries(World PUBLIC MathLib)
target_link_libraries(World PUBLIC Camera)


#Properties for Win32
target_compile_definitions(bossFight PRIVATE UNICODE UNICODE _UNICODE)
set_target_properties(bossFight PROPERTIES WIN32_EXECUTABLE ON)

target_compile_definitions(bossFight PUBLIC _CRTDBG_MAP_ALLOC)

#Copy dlls inside the .exe folder
add_custom_command(TARGET bossFight POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy
                   "${CMAKE_SOURCE_DIR}/Lib/release/libfbxsdk.dll"
                   $<TARGET_FILE_DIR:bossFight>) 
add_custom_command(TARGET bossFight POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy
                   "${CMAKE_SOURCE_DIR}/Lib/SDL_Lib/SDL3.dll"
                   $<TARGET_FILE_DIR:bossFight>)  