#Compiler 
CXX = g++

INCLUDES = -I. -ISystem -IMath -IWindows_OS -IGraphics -IInput -IGame -IWorld -ICamera -IEntity -IComponent -ISystem -IEntity_C -IInclude
LIBRARIES = -LLib/SDL_Lib -ld3d11 -ld3dcompiler -ldxgi -lshlwapi -lhid -lSDL3 -luser32 -lKernel32 -LLib 
COMPILEFLAGS = -O3 -DUNICODE -g 
LINKFLAGS = -static-libgcc -static-libstdc++ -static

#CPP
SOURCES = $(wildcard *.cpp) $(wildcard System/*.cpp) $(wildcard Math/*.cpp) $(wildcard Windows_OS/*.cpp) $(wildcard Graphics/*.cpp) $(wildcard Input/*.cpp) $(wildcard Game/*.cpp) $(wildcard Game/World/*.cpp) $(wildcard Game/Camera/*.cpp) $(wildcard Game/World/Entity/*.cpp) $(wildcard Game/World/Component/*.cpp) $(wildcard Game/World/System/*.cpp) $(wildcard Game/World/Entity_C/*.cpp)
OBJECTS = $(SOURCES:.cpp=.o)

TARGET = main

#Default Rule
$(TARGET): $(OBJECTS)
	@echo "Building..."
	$(CXX) $(LINKFLAGS) $(OBJECTS) $(LIBRARIES) -o $(TARGET)

#Objects
%.o: %.cpp
	$(CXX) $(COMPILEFLAGS) $(INCLUDES) -c $< -o $@ 

#Clean
clean: 
	rm -f $(TARGET) $(OBJECTS)