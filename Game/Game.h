#ifndef __GAME_H__
#define __GAME_H__

#include <windows.h>
#include <cstdio>

struct windowPreferances;
class Input;
class Graphics;
class Camera;
class World;

class Game{
public:
    Game();
    ~Game();
    bool initializeGame(HWND hWnd, HMONITOR hMonitor, windowPreferances window);
    void updateGame(float frame_time);
    void renderGame();
    void cleanGame();

private:
    Input *input;
    Graphics *gfx;
    Camera *camera;
    World *world;
};

#endif