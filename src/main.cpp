#include "vectors.hpp"
#include "physics.hpp"
#include "player.hpp"
#include "vector2d.hpp"
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

void draw_sprite(std::shared_ptr<SDL_Renderer> r, std::array<double, 2> p, std::shared_ptr<SDL_Texture> tex, std::array<int, 2> size, double alpha)
{
    SDL_Rect dst_rect = {(int)(p[0] - size[0] / 2), (int)(p[1] - size[1] / 2), (int)size[0], (int)size[1]};
    SDL_RenderCopyEx(r.get(), tex.get(), NULL, &dst_rect, alpha, NULL, SDL_RendererFlip::SDL_FLIP_NONE);
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
    shared_ptr<SDL_Texture> playerTexture(IMG_LoadTexture(&*renderer_p, "data/player.png"),[](auto* tex) { SDL_DestroyTexture(tex); });
    shared_ptr<SDL_Texture> mouseTexture(IMG_LoadTexture(&*renderer_p, "data/mouse.png"),[](auto* tex) { SDL_DestroyTexture(tex); });

    // Instanciate player
    Player player;

    // Mouse
    int mouseX = -10, mouseY = -10;

    // Looptime setup
    milliseconds deltaTime(15);
    steady_clock::time_point current_time = steady_clock::now(); // remember current time

    // Main gameloop
    for (bool game_active = true; game_active;) {
      // Event system
        SDL_Event event;
        while (SDL_PollEvent(&event)) { // check if there are some events
            if (event.type == SDL_QUIT){
                game_active = false;
            }
            if(event.type == SDL_MOUSEMOTION){
              SDL_GetMouseState(&mouseX, &mouseY);
              // Just For Debbuging
              // std::cout << "mouseX: " << mouseX << "  mouseY: " << mouseY << std::endl;
            }
            if(event.type == SDL_MOUSEBUTTONDOWN || event.type == SDL_MOUSEBUTTONUP){
              if(SDL_BUTTON_LEFT){
                //start incrementing strenght of projectail (acceleration ++)
              }
              else{
                //end the timer and shoot projectail (acceleration = 0, instanciate projectail)
              }
            }

        }

        // Keyboard input
        auto kbdstate = SDL_GetKeyboardState(NULL);
        if (kbdstate[SDL_SCANCODE_LEFT]) player.intentions["left"] = 1;
        if (kbdstate[SDL_SCANCODE_RIGHT]) player.intentions["right"] = 1;

        // Physics
        // Physical update time
        double deltaTime_physics = deltaTime.count() / 1000.0;
        player.apply_intent();
        player.update(deltaTime_physics, [&](auto p, auto pos, auto vel){
          //temp solution
            if (pos[1] < 650) {
                p->position = pos;
                p->velocity = vel;
            } else {
                p->velocity = {vel[0],0};
                p->position[0] = pos[0];
            }
        });

        // Graphics
        SDL_SetRenderDrawColor(&*renderer_p, 135, 206, 235, 255);
        SDL_RenderClear(&*renderer_p);
        SDL_SetRenderDrawColor(&*renderer_p, 255, 100, 200, 255);
        // Draw player
        draw_sprite(renderer_p, player.position, playerTexture, player.size, 0);
        // Draw mouse pointer
        draw_sprite(renderer_p, {mouseX, mouseY}, mouseTexture, {20,20}, 0);
        // TEMP draw visualization line for projectail to shoot
        SDL_RenderDrawLine(&*renderer_p, player.position[0], player.position[1], mouseX, mouseY);
        SDL_RenderPresent(&*renderer_p);

        // Loop clock
        this_thread::sleep_until(current_time = current_time + deltaTime);
    }
    SDL_Quit();
    return 0;
}
