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

    Game bossfight(&input, &window);
    if(!bossfight.initialize_game())
        return 0;
    
    Core::n_time::Time_Data *start_frame = nullptr, *end_frame = nullptr;
    start_frame = Core::n_time::time_start();
    end_frame = Core::n_time::time_start();
    int fps = 0;
    double timer = 0.0f, frame_time = 0.0f;

    while(bossfight.should_render_game()){
        if(timer > 1.0f){
            timer = 0.0f;
            fps = 0;
        }

        Core::n_time::get_time(start_frame);
        input.update_input();
        bossfight.update_game(frame_time);
        bossfight.render_game(fps);
        Core::n_time::get_time(end_frame);
        frame_time = Core::n_time::time_diff(start_frame, end_frame);
        timer += frame_time;
        fps += 1;
    }

    Core::n_time::time_end(start_frame);
    Core::n_time::time_end(end_frame);
    bossfight.close_game();
    input.close_input();
    window.destroy_window();
    return 0;
}