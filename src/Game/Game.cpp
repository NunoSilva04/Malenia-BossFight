#include "Game.h"
#include "window.h"
#include "input.h"

Game::Game(Input *input_class, Window *window_class) noexcept
    : input(input_class), window(window_class) {}

Game::~Game() {}

bool Game::initialize_game(void){
    Core::n_vector<const char *> extensions = window->get_extensions();
    gfx.initialize_graphics(window->get_window_handle(), extensions);
    
    player = create_player();
    destroy_player(player);

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
    gfx.temp_render_func();

    return;
}

void Game::close_game(void){
    gfx.close_graphics();

    return;
}