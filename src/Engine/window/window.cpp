#include "window.h"
#include <stdio.h>
#include "input.h"
#include "Graphics.h"

Window::Window(){
    window = nullptr;
    display_properties = {};
}

Window::~Window(){

}

bool Window::create_window(const char *window_name){
    #ifdef DEBUG
    int num_displays = 0;
    SDL_DisplayID *displays = SDL_GetDisplays(&num_displays);
    if(displays != NULL){
        printf("Found %d displays\n", num_displays);
        for(int i = 0 ; i < num_displays; i++){
            printf("Display %d:\n", i);
             
            const char *display_name = SDL_GetDisplayName(displays[i]);
            if(display_name != NULL) printf("\tDisplay name: %s\n", display_name); 
            SDL_Rect rect;
            printf("\tNormal bounds:\n");
            if(SDL_GetDisplayBounds(displays[i], &rect)) printf("\t\tRect.x = %d\n\t\tRect.y = %d\n\t\tRect.w = %d\n\t\tRect.h = %d\n", rect.x, rect.y, rect.w, rect.h);
            printf("\tUsable Bounds:\n");
            if(SDL_GetDisplayUsableBounds(displays[i], &rect)) printf("\t\tRect.x = %d\n\t\tRect.y = %d\n\t\tRect.w = %d\n\t\tRect.h = %d\n", rect.x, rect.y, rect.w, rect.h);
            float display_scale = SDL_GetDisplayContentScale(displays[i]);
            if(display_scale != 0) printf("\tDisplay scale = %f\n", display_scale);
            const SDL_DisplayMode *display_mode;
            display_mode = SDL_GetCurrentDisplayMode(displays[i]);
            printf("\tDisplay mode data:\n");
            printf("\t\tPixel format = 0x%x\n", display_mode->format);
            printf("\t\tW = %d, H = %d\n", display_mode->w, display_mode->h);
            printf("\t\tPixel density = %f\n", display_mode->pixel_density);
            printf("\t\tRefresh rate = %f\n", display_mode->refresh_rate);
            printf("\t\tRefresh rate numerator = %d, Refresh Rate Denominator = %d\n", display_mode->refresh_rate_numerator, display_mode->refresh_rate_denominator);
        }
    }
    #endif

    display_properties.id = SDL_GetPrimaryDisplay();
    if(display_properties.id  == NULL) return false;
    display_properties.name = SDL_GetDisplayName(display_properties.id); 
    if(display_properties.name == NULL) return false;
    SDL_Rect temp;
    if(!SDL_GetDisplayBounds(display_properties.id, &temp)) return false;
    display_properties.bounds.x = temp.x;
    display_properties.bounds.y = temp.y;
    display_properties.bounds.width = temp.w;
    display_properties.bounds.heigth = temp.h;
    if(!SDL_GetDisplayUsableBounds(display_properties.id, &temp)) return false;
    display_properties.usable_bounds.x = temp.x;
    display_properties.usable_bounds.y = temp.y;
    display_properties.usable_bounds.width = temp.w;
    display_properties.usable_bounds.heigth = temp.h;
    display_properties.scale = SDL_GetDisplayContentScale(display_properties.id);
    if(display_properties.scale == 0.0f) return false;
    const SDL_DisplayMode *display_mode = SDL_GetCurrentDisplayMode(display_properties.id);
    if(display_mode == NULL) return false;
    display_properties.pixel_density = display_mode->pixel_density;
    display_properties.refresh_rate = display_mode->refresh_rate;
    

    #ifdef DEBUG
    printf("\nMain display part\n");
    printf("\tDisplay name: %s\n", display_properties.name); 
    printf("\tNormal bounds:\n");
    printf("\tx = %d\n\ty = %d\n\tw = %d\n\th = %d\n", 
                    display_properties.bounds.x, display_properties.bounds.y, display_properties.bounds.width, display_properties.bounds.heigth);
    printf("\tUsable Bounds:\n");
    printf("\tx = %d\n\ty = %d\n\tw = %d\n\th = %d\n", 
            display_properties.usable_bounds.x, display_properties.usable_bounds.y, display_properties.usable_bounds.width, display_properties.usable_bounds.heigth);
    printf("\tDisplay scale = %f\n", display_properties.scale);
    printf("\tDisplay mode data:\n");
    printf("\t\tPixel format = 0x%x\n", display_mode->format);
    printf("\t\tW = %d, H = %d\n", display_mode->w, display_mode->h);
    printf("\t\tPixel density = %f\n", display_mode->pixel_density);
    printf("\t\tRefresh rate = %f\n", display_mode->refresh_rate);
    printf("\t\tRefresh rate numerator = %d, Refresh Rate Denominator = %d\n", display_mode->refresh_rate_numerator, display_mode->refresh_rate_denominator);
    #endif 

    window = SDL_CreateWindow(window_name, display_properties.bounds.width, display_properties.bounds.heigth, SDL_WINDOW_VULKAN | SDL_WINDOW_BORDERLESS);
    if(window == NULL){
        printf("Couldn't create window\n");
        return false;
    }

    // Graphics
    gfx = new Graphics;

    uint32_t num_extensions = 0;
    char const * const * array_extensions = SDL_Vulkan_GetInstanceExtensions(&num_extensions);
    if(num_extensions == 0) return false;

    if(!gfx->initialize_graphics(array_extensions, num_extensions)){
        printf("Couldn't initialize graphics\n");
        return false;
    }

    return true;
}

bool Window::should_render_window(void){
    static bool render = true;

    Core::n_vector<Event> queue = Input::dispatch_input(Window_Id);
    for(size_t i = 0; i < queue.vector_size(); i++){
        Event e;
        queue.pop_first(&e);
        if(e == Keyboard_Escape){
            render = false;
        }
    }

    return render;
}

void Window::destroy_window(void){
    gfx->close_graphics();
    delete gfx;
    SDL_DestroyWindow(window);

    return;
}