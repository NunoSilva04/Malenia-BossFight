#include <stdio.h>
#include "SDL_SubSystem.h"
#include "window.h"
#include "input.h"
#include "Game.h"
#include "time.h"

int main(void){
    SDL_SubSytem sdl;

    Window window;
    if(!window.create_window("SDL Window")) 
        return 0;

    Input input;
    input.initialize_input();

    Core::n_time::Time_Data *start_time = nullptr, *end_time = nullptr;
    start_time = Core::n_time::time_start();
    end_time = Core::n_time::time_start();

    Core::n_time::get_time(start_time);
    Game bossfight(&input, &window);
    if(!bossfight.initialize_game())
        return 0;
    Core::n_time::get_time(end_time);
    double diff = Core::n_time::time_diff(start_time, end_time);
    printf("Time diff: %lf\n\n\n", diff);

    while(bossfight.should_render_game()){
        input.update_input();
        bossfight.update_game();
        bossfight.render_game();
    }


    bossfight.close_game();
    input.close_input();
    window.destroy_window();
    return 0;
}