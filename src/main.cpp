#include "vectors.hpp"
#include "physics.hpp"
#include "player.hpp"
#include "game.hpp"
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
#include <ctime>

void draw_sprite(std::shared_ptr<SDL_Renderer> r, std::array<double, 2> p, std::shared_ptr<SDL_Texture> tex, std::array<int, 2> size, double alpha)
{
    SDL_Rect dst_rect = {(int)(p[0] - size[0] / 2), (int)(p[1] - size[1] / 2), (int)size[0], (int)size[1]};
    SDL_RenderCopyEx(r.get(), tex.get(), NULL, &dst_rect, alpha, NULL, SDL_RendererFlip::SDL_FLIP_NONE);
}

Game init_all(std::array<int, 2> windowRealSize, int meterToPixels){
  Game game;

  using namespace std;
      SDL_Init(SDL_INIT_EVERYTHING);
      IMG_Init(IMG_INIT_PNG);

      // Window
      game.window_p = shared_ptr<SDL_Window>(
        SDL_CreateWindow(
          "Better Worms",
          SDL_WINDOWPOS_UNDEFINED,
          SDL_WINDOWPOS_UNDEFINED,
          1080, 720,
          SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE),
        [](auto* window) { SDL_DestroyWindow(window); });

      // Renderer
      game.renderer_p = shared_ptr<SDL_Renderer>(
          SDL_CreateRenderer(
            game.window_p.get(),
            -1,
            SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC),
          [](auto* renderer) { SDL_DestroyRenderer(renderer); });

      SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "0");
      SDL_RenderSetLogicalSize(game.renderer_p.get(), windowRealSize[0] * meterToPixels, windowRealSize[1] * meterToPixels);

      // Load textures
      game.playerTexture = shared_ptr<SDL_Texture>(IMG_LoadTexture(game.renderer_p.get(), "data/player.png"),[](auto* tex) { SDL_DestroyTexture(tex); });
      game.mouseTexture = shared_ptr<SDL_Texture>(IMG_LoadTexture(game.renderer_p.get(), "data/mouse.png"),[](auto* tex) { SDL_DestroyTexture(tex); });
      game.projectailTexture = shared_ptr<SDL_Texture>(IMG_LoadTexture(game.renderer_p.get(), "data/bullet.png"),[](auto* tex) { SDL_DestroyTexture(tex); });

      // Mouse
      game.mouseX = -10, game.mouseY = -10;

      return game;
}

void drawstuff(Game& game, int meterToPixels)
{
  using namespace tp::operators;

  SDL_SetRenderDrawColor(game.renderer_p.get(), 135, 206, 235, 255);
  SDL_RenderClear(game.renderer_p.get());
  SDL_SetRenderDrawColor(game.renderer_p.get(), 255, 100, 200, 255);

  // Draw player
  draw_sprite(game.renderer_p, game.player.position * meterToPixels, game.playerTexture, game.player.get_scaled_size(meterToPixels), 0);

  // Draw mouse pointer
  draw_sprite(game.renderer_p, {game.mouseX, game.mouseY}, game.mouseTexture, {20,20}, 0);

  //Draw bullet
  // TEMP draw visualization line for projectail to shoot
  //SDL_RenderDrawLine(game.renderer_p.get(), game.player.position[0], game.player.position[1], game.mouseX, game.mouseY);
  if(game.projectail.isActive){
    draw_sprite(game.renderer_p, game.projectail.position * meterToPixels, game.projectailTexture, game.projectail.get_scaled_size(meterToPixels), 0);
  }

  // Ground
  //SDL_SetRenderDrawColor(game.renderer_p.get(), 255, 255, 255, 255);
  //SDL_RenderDrawLine(game.renderer_p.get(), 0, 650, 1024, 650);

  SDL_RenderPresent(game.renderer_p.get());
}

int main(int, char**)
{
    using namespace std;
    using namespace std::chrono;
    using namespace tp::operators;

    // Initialization
    int meterToPixels = 100;
    std::array<int, 2> windowRealSize = {15, 10};
    Game game = init_all(windowRealSize,meterToPixels);

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
              SDL_GetMouseState(&game.mouseX, &game.mouseY);
              // Just For Debbuging
              // std::cout << "mouseX: " << mouseX << "  mouseY: " << mouseY << std::endl;
            }
            if(event.type == SDL_MOUSEBUTTONDOWN){
              if(SDL_BUTTON_LEFT){
                //start incrementing strenght of projectail (acceleration ++)
                game.strenght_timer = clock();
              }
            }

            if(event.type == SDL_MOUSEBUTTONUP){
              if(SDL_BUTTON_LEFT && !game.projectail.isActive){
                //end the timer and shoot projectail
                game.projectail.strenght = ((clock() - game.strenght_timer) / (double) CLOCKS_PER_SEC) * 100;
                game.projectail.instanciate(game.player, game.mouseX, game.mouseY);

                //set acceleration of bullet to normilized vector of direction times strenght
                //std::array<double, 2> vec = game.player.position - std::array<double, 2>{game.mouseX, game.mouseY};
                //game.projectail.direction = (vec / sqrt(vec[0]*vec[0]+vec[1]*vec[1])) * -1;
              }
            }

        }

        // Keyboard input
        auto kbdstate = SDL_GetKeyboardState(NULL);
        if (kbdstate[SDL_SCANCODE_A]) game.player.intentions["left"] = 1;
        if (kbdstate[SDL_SCANCODE_D]) game.player.intentions["right"] = 1;
        if (kbdstate[SDL_SCANCODE_W]) game.player.intentions["up"] = 1;

        // Physics
        // Physical update time
        double deltaTime_physics = deltaTime.count() / 1000.0;
        //temp solution
        if (!(game.player.position[1] < 9.9 - (game.player.size[1]/2))) {
          game.player.position[1] = 9.9 - (game.player.size[1]/2);
          game.player.isGrounded = true;
        }
        game.player.apply_intent();
        game.player.update(deltaTime_physics);
        //devStuff
        //game.player.write();

        //Projectail
        if(game.projectail.isActive){
          //Border
          if (game.projectail.position[0] < 0 || game.projectail.position[0] > windowRealSize[0]-0.1 || game.projectail.position[1] < 0 || game.projectail.position[1] > windowRealSize[1]-0.1) {
            game.projectail.destroy();
            cout << "out of borders" << endl;
          }

          //Physical update
          //game.projectail.applyAcceleration();
          game.projectail.update(deltaTime_physics);
          //devStuff
          game.projectail.write();
        }

        // Graphics
        drawstuff(game, meterToPixels);

        // Loop clock
        this_thread::sleep_until(current_time = current_time + deltaTime);
    }
    SDL_Quit();
    return 0;
}
