#include "vectors.hpp"
#include "physics.hpp"
#include "player.hpp"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <array>
#include <chrono>
#include <cstdint>
#include <functional>
#include <iomanip>
#include <iostream>
#include <memory>
#include <set>
#include <stdexcept>
#include <string>
#include <thread>
#include <tuple>
#include <vector>

void draw_o(std::shared_ptr<SDL_Renderer> r, std::array<double, 2> p, std::shared_ptr<SDL_Texture> tex, double w, double h, double a)
{
    SDL_Rect dst_rect = {(int)(p[0] - w / 2), (int)(p[1] - h / 2), (int)w, (int)h};
    SDL_RenderCopyEx(r.get(), tex.get(), NULL, &dst_rect, a, NULL, SDL_RendererFlip::SDL_FLIP_NONE);
}

int main(int, char**)
{
    using namespace std;
    using namespace std::chrono;
    using namespace tp::operators;

    SDL_Init(SDL_INIT_EVERYTHING);
    IMG_Init(IMG_INIT_PNG);

    // Window
    shared_ptr<SDL_Window> window_p(
      SDL_CreateWindow(
        "Better Worms",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        1080, 720,
        SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE),
      [](auto* window) { SDL_DestroyWindow(window); });

    // Renderer
    shared_ptr<SDL_Renderer> renderer_p(
        SDL_CreateRenderer(
          &*window_p,
          -1,
          SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC),
        [](auto* renderer) { SDL_DestroyRenderer(renderer); });

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "0");
    SDL_RenderSetLogicalSize(&*renderer_p, 1080, 720);

    // Load textures
    shared_ptr<SDL_Texture> tex_player(IMG_LoadTexture(&*renderer_p, "data/player.png"),[](auto* tex) { SDL_DestroyTexture(tex); });

    // Instanciate player
    Player player;

    // Looptime setup
    milliseconds dt(15);
    steady_clock::time_point current_time = steady_clock::now(); // remember current time

    // Main gameloop
    for (bool game_active = true; game_active;) {
      // Event system
        SDL_Event event;
        while (SDL_PollEvent(&event)) { // check if there are some events
            if (event.type == SDL_QUIT)
                game_active = false;
        }

        //keyboard input
        auto kbdstate = SDL_GetKeyboardState(NULL);
        //TODO: redo player controls (in range from -1 to 1)
        //if (kbdstate[SDL_SCANCODE_LEFT]) player.intentions["left"] = 1;
        //if (kbdstate[SDL_SCANCODE_RIGHT]) player.intentions["right"] = 1;

        // TODO: scrap what is needed
        //for (auto [k,v] : player.intentions) {
        //    std::cout << "[" << k << ":" << v << "] ";
        //}
        //std::cout << std::endl;
        //    if (kbdstate[SDL_SCANCODE_RIGHT])


        // Physics
        double dt_f = dt.count() / 1000.0;
        /*
        player.apply_intent();
        player.update(dt_f, [&](auto p, auto pos, auto vel){
            if (pos[1] < 150) {
                p->position = pos;
                p->velocity = vel;
            } else {
                p->velocity = {vel[0],0};
                p->position[0] = pos[0];
            }
        });
        */

        // Graphics
        SDL_SetRenderDrawColor(&*renderer_p, 135, 206, 235, 255);
        SDL_RenderClear(&*renderer_p);
        SDL_SetRenderDrawColor(&*renderer_p, 255, 100, 200, 255);
        //this sets window background so, not needed?
        //SDL_RenderCopy(&*renderer_p, &*tex_p, NULL, NULL);
        draw_o(renderer_p, player.position, tex_player, player.size[0], player.size[1], 0);
        //draw_o(&*renderer_p,{50,20},&*tex_p,16,16,30);
        SDL_RenderPresent(&*renderer_p);

        // Loop clock
        this_thread::sleep_until(current_time = current_time + dt);
    }
    SDL_Quit();
    return 0;
}
