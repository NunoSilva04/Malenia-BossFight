#ifndef __WINDOW_H__
#define __WINDOW_H__

#include <SDL3/SDL.h>
#include <SDL3/SDL_video.h>

class Input;

class Window{
public:
    Window(void);
    ~Window(void);
    bool create_window(const char *window_name);
    bool should_render_window(void);
    void update_window(float frame_time, Input *input);
    void render_window(void);
    void destroy_window(void);

private:
    SDL_Window *window;
};

#endif