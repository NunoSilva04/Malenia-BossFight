#include "Game.h"
#include "window.h"
#include "input.h"
#include "Graphics.h"

Game::Game(Input *input_class, Window *window_class) noexcept
    : input(input_class), window(window_class) {}

Game::~Game() {}

bool Game::initialize_game(void){
    Graphics_Errors gfx_error;
    gfx = initialize_graphics(window->get_window_handle(), window->get_extensions(), &gfx_error);
    if(gfx == NULL){
        Core::debug::log(Core::debug::Fatal, "Couldn't initialize graphics\nExited with error number: %d (%s)\n", gfx_error, graphics_error_str(gfx_error));
        return false;
    }

    return true;
}

bool Game::should_render_game(void){
    static bool render = true;
    
    Core::n_vector<Event> queue_event = input->dispatch_input(Game_Id);
    for(size_t i = 0; i < queue_event.vector_size(); i++){
        Event e;
        queue_event.pop_first(&e);
        if(e == Keyboard_Escape){
            render = false;
        }
    }

    return render;
}

void Game::update_game(double frame_time){

    return;
}

void Game::render_game(int fps){
    temp_render_function(gfx);

    return;
}

void Game::close_game(void){
    close_graphics(gfx);

    return;
}