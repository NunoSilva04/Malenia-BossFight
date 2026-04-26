/**********************************************************************************
*   File: Game.h
* 
*   Description: 
*       Responsible for the inteire game logic, as well as assets creation and more.
* 
*   See: 
*       docs/Game/Game.md 
*       for full documentation of the header
* 
*************************************************************************************/

#ifndef __GAME_H__
#define __GAME_H__   

#include "Graphics.h"
#include "Player.h"

class Input;
class Window;

class Game{
public:
    explicit Game(Input *input_class, Window *window_class) noexcept;
    ~Game();
    Game(const Game &other) = delete;
    Game &operator=(const Game &other) = delete;
    Game(Game &&other) = delete;
    Game &operator=(Game &&other) = delete;

public:
    bool initialize_game(void);
    bool should_render_game(void);
    void update_game(double frame_time);
    void render_game(int fps);
    void close_game(void);

private:
    Input *input;
    Window *window;
    Graphics gfx;
    Player *player;
};

#endif