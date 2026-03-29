/**********************************************************************************
*   File: window.h
* 
*   Description: 
*        Handles all window events such as creation, destruction, rendering and more. 
*        Uses SDL3.
* 
*   See: 
*       docs/Engine/window.md 
*       for detailed documentation.
* 
*************************************************************************************/

#ifndef __WINDOW_H__
#define __WINDOW_H__

#include <SDL3/SDL.h>
#include <SDL3/SDL_video.h>
#include <Rect.h>

class Input;
class Graphics;

class Window{
public:
    Window();
    ~Window();
    bool create_window(const char *window_name);
    bool should_render_window(void);
    void temp_render_func(void);
    void destroy_window(void);

private:
    SDL_Window *window;
    typedef struct properties_t{
        uint32_t id;
        const char *name;
        n_math::rect<int> bounds;
        n_math::rect<int> usable_bounds;
        float scale;
        float pixel_density;
        float refresh_rate;
    }properties_t;
    properties_t display_properties;

    //Graphics
    Graphics *gfx;
};

#endif