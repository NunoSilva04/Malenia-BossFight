#ifndef __WINDOW_H__
#define __WINDOW_H__

#include <SDL3/SDL.h>
#include <SDL3/SDL_video.h>

class Window{
public:
    Window(void);
    ~Window(void);
    bool create_window(const char *window_name);
    void destroy_window(void);

private:
    SDL_Window *window;
};

#endif