#include "vectors.hpp"
#include "physics.hpp"
#include "player.hpp"
#include "game.hpp"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <array>
#include <chrono>
#include <iostream>
#include <string>
#include <thread>
#include <ctime>

void draw_sprite(std::shared_ptr<SDL_Renderer> r, std::array<double, 2> p, std::shared_ptr<SDL_Texture> tex, std::array<int, 2> size, double rotation, SDL_RendererFlip flip = SDL_RendererFlip::SDL_FLIP_NONE)
{
    SDL_Rect dst_rect = {(int)(p[0] - size[0] / 2), (int)(p[1] - size[1] / 2), (int)size[0], (int)size[1]};
    SDL_RenderCopyEx(r.get(), tex.get(), NULL, &dst_rect, rotation, NULL, flip);
}

Game init_all(std::array<int, 2> windowRealSize, int meterToPixels){
  Game game;

  using namespace std;
      SDL_Init(SDL_INIT_EVERYTHING);
      IMG_Init(IMG_INIT_PNG);
      //Testing this input mode is dangerous
      //SDL_SetRelativeMouseMode(SDL_TRUE);

      //Window
      game.window_p = shared_ptr<SDL_Window>(
        SDL_CreateWindow(
          "Better Worms",
          SDL_WINDOWPOS_UNDEFINED,
          SDL_WINDOWPOS_UNDEFINED,
          1080, 720,
          SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE),
        [](auto* window) { SDL_DestroyWindow(window); });

      //Renderer
      game.renderer_p = shared_ptr<SDL_Renderer>(
          SDL_CreateRenderer(
            game.window_p.get(),
            -1,
            SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC),
          [](auto* renderer) { SDL_DestroyRenderer(renderer); });

      SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "0");
      SDL_RenderSetLogicalSize(game.renderer_p.get(), windowRealSize[0] * meterToPixels, windowRealSize[1] * meterToPixels);

      //Load textures
      game.playerTexture = shared_ptr<SDL_Texture>(IMG_LoadTexture(game.renderer_p.get(), "data/player.png"),[](auto* tex) { SDL_DestroyTexture(tex); });
      game.mouseTexture = shared_ptr<SDL_Texture>(IMG_LoadTexture(game.renderer_p.get(), "data/mouse.png"),[](auto* tex) { SDL_DestroyTexture(tex); });
      game.projectileTexture = shared_ptr<SDL_Texture>(IMG_LoadTexture(game.renderer_p.get(), "data/bullet.png"),[](auto* tex) { SDL_DestroyTexture(tex); });
      game.explosionTexture = shared_ptr<SDL_Texture>(IMG_LoadTexture(game.renderer_p.get(), "data/explosion.png"),[](auto* tex) { SDL_DestroyTexture(tex); });
      game.pointerTexture = shared_ptr<SDL_Texture>(IMG_LoadTexture(game.renderer_p.get(), "data/pointer.png"),[](auto* tex) { SDL_DestroyTexture(tex); });
      game.groundTexture = shared_ptr<SDL_Texture>(IMG_LoadTexture(game.renderer_p.get(), "data/ground.png"),[](auto* tex) { SDL_DestroyTexture(tex); });
      game.endTexture = shared_ptr<SDL_Texture>(IMG_LoadTexture(game.renderer_p.get(), "data/end.png"),[](auto* tex) { SDL_DestroyTexture(tex); });
      game.player1Texture = shared_ptr<SDL_Texture>(IMG_LoadTexture(game.renderer_p.get(), "data/player1.png"),[](auto* tex) { SDL_DestroyTexture(tex); });
      game.player2Texture = shared_ptr<SDL_Texture>(IMG_LoadTexture(game.renderer_p.get(), "data/player2.png"),[](auto* tex) { SDL_DestroyTexture(tex); });


      //Mouse
      game.mouseX = 10, game.mouseY = 10;

      //Players
      game.players[0].position = {1.2, 8.0};
      game.players[1].position = {8.0, 8.0};

      //Projectile
      game.projectile.friction = 0.05;

      return game;
}

void physicalUpdate(Game& game, std::chrono::milliseconds deltaTime, std::array<int, 2> windowRealSize){
  // Physics
  // Physical update time
  double deltaTime_physics = deltaTime.count() / 1000.0;

  //Players
  for(int i=0; i < game.players.size(); i++){
    if (!(game.players[i].position[1] < 9.9 - (game.players[i].size[1]/2))) {
      game.players[i].position[1] = 9.9 - (game.players[i].size[1]/2);
      game.players[i].isGrounded = true;
    }
    game.players[i].apply_intent();
    game.players[i].update(deltaTime_physics);

    //chcecks does player lives
    if(game.players[i].hp < 0){
      game.isResolved = true;
    }

    //devStuff
    //game.player.write();
  }


  //Projectile
  if(game.projectile.isActive){
    //Ground

    //Physical update
    game.projectile.update(deltaTime_physics);
    //Calculate rotation
    game.projectile.animate();
    //devStuff
    //game.projectile.write();

    //Border
    /*if (game.projectile.position[0] < 0 || game.projectile.position[0] > windowRealSize[0]-0.1 || game.projectile.position[1] < 0) {
      game.projectile.destroy();
      std::cout << "Projectile destroyed" << std::endl;
    }
    else */
    if(game.projectile.position[1] > windowRealSize[1]-0.1){
      game.projectile.explode(game.players);
      std::cout << "Projectile exploded" << std::endl;
    }
  }
  // !!WARNING!! Lazy solution !!WARNING!!
  game.projectile.updateExplosion();
}

void drawstuff(Game& game, int meterToPixels, auto kbdstate)
{
  using namespace tp::operators;

  SDL_SetRenderDrawColor(game.renderer_p.get(), 135, 206, 235, 255);
  SDL_RenderClear(game.renderer_p.get());
  //SDL_SetRenderDrawColor(game.renderer_p.get(), 255, 100, 200, 255);

  // Draw players
  for(int i=0; i < game.players.size(); i++){
    if(game.players[i].hp > 0){
      if(game.players[i].velocity[0] < 0){
        draw_sprite(game.renderer_p, game.players[i].get_scaled_position(meterToPixels), game.playerTexture, game.players[i].get_scaled_size(meterToPixels), 0, SDL_FLIP_HORIZONTAL);
      }else{
        draw_sprite(game.renderer_p, game.players[i].get_scaled_position(meterToPixels), game.playerTexture, game.players[i].get_scaled_size(meterToPixels), 0);
      }
    }
  }
  // Draw pointer above player head and player tags
  draw_sprite(game.renderer_p, {game.players[game.turn].get_scaled_position(meterToPixels)[0], game.players[game.turn].get_scaled_position(meterToPixels)[1]-120}, game.pointerTexture, {40,40}, 0);
  draw_sprite(game.renderer_p, {game.players[0].get_scaled_position(meterToPixels)[0]+20, game.players[game.turn].get_scaled_position(meterToPixels)[1]-80}, game.player1Texture, {120,60}, 0);
  draw_sprite(game.renderer_p, {game.players[1].get_scaled_position(meterToPixels)[0]+20, game.players[game.turn].get_scaled_position(meterToPixels)[1]-80}, game.player2Texture, {120,60}, 0);


  // Draw mouse pointer
  draw_sprite(game.renderer_p, {game.mouseX, game.mouseY}, game.mouseTexture, {20,20}, 0);

  //Draw bullet
  // TEMP draw visualization line for projectile to shoot
  //SDL_SetRenderDrawColor(game.renderer_p.get(), 255, 100, 200, 255);
  //SDL_RenderDrawLine(game.renderer_p.get(), game.player.position[0], game.player.position[1], game.mouseX, game.mouseY);
  //SDL_SetRenderDrawColor(game.renderer_p.get(), 255, 100, 200, 255);
  //SDL_RenderDrawLine(game.renderer_p.get(), game.player.position[0], game.player.position[1], game.mouseX, game.mouseY);
  if(game.projectile.isActive){
    draw_sprite(game.renderer_p, game.projectile.get_scaled_position(meterToPixels), game.projectileTexture, game.projectile.get_scaled_size(meterToPixels), game.projectile.angle);
  }
  // Explosion
  if(game.projectile.isExploding){
    draw_sprite(game.renderer_p, game.projectile.get_scaled_position(meterToPixels), game.explosionTexture, game.projectile.get_scaled_exp_size(meterToPixels), 0);
  }

  // Ground
  draw_sprite(game.renderer_p, {750, 990}, game.groundTexture, {1500,50}, 0);

  // Texts
  //saw that this should work with sdl2 yet not shure how to implement it
  // TTF_Font* Sans = TTF_OpenFont("Sans.ttf", 24);

  // Game over text XD
  if(game.isResolved){
    draw_sprite(game.renderer_p, {750, 500}, game.endTexture, {500,250}, 0);
  }

  SDL_RenderPresent(game.renderer_p.get());
}

int main(int, char**)
{
    using namespace std;
    using namespace std::chrono;
    using namespace tp::operators;

    //Initialization
    int meterToPixels = 100;
    std::array<int, 2> windowRealSize = {15, 10};
    Game game = init_all(windowRealSize,meterToPixels);

    //Looptime setup
    milliseconds deltaTime(15);
    //Remember current time
    steady_clock::time_point current_time = steady_clock::now();

    // Main gameloop
    for (bool game_active = true; game_active;) {
      // Event system
      SDL_Event event;
      //Check if there are some events
      while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT){
          game_active = false;
        }
        if(event.type == SDL_MOUSEMOTION){
          game.mouseX = event.motion.x;
          game.mouseY = event.motion.y;
          //devStuff
          //cout << event.motion.x << "," << event.motion.y << endl;
        }
        if(event.type == SDL_MOUSEBUTTONDOWN){
          if(event.button.button == SDL_BUTTON_LEFT){
            //start incrementing strenght of projectile (acceleration ++)
            //TODO: prevent incrementing when projectile is active
            //NOTE: try redoing it with just incremention in physics update
            game.strength_timer = clock();
          }
        }

        if(event.type == SDL_MOUSEBUTTONUP){
          if(event.button.button == SDL_BUTTON_LEFT && !game.projectile.isActive && !game.isResolved){
            //end the timer and shoot projectile
            game.projectile.strength = tp::operators::clamp(((clock() - game.strength_timer) / (double) CLOCKS_PER_SEC) * 100, 0.0, 15.0);
            game.projectile.instanciate(game.players[game.turn], game.mouseX, game.mouseY, meterToPixels);
            game.turn += 1 + (-2*game.turn);
            //devStuff
            //cout << "turn:" << game.turn << endl;
          }
        }
      }

      // Keyboard input
      auto kbdstate = SDL_GetKeyboardState(NULL);
      if (kbdstate[SDL_SCANCODE_A]) game.players[game.turn].intentions["left"] = 1;
      if (kbdstate[SDL_SCANCODE_D]) game.players[game.turn].intentions["right"] = 1;
      if (kbdstate[SDL_SCANCODE_W]) game.players[game.turn].intentions["up"] = 1;
      if (kbdstate[SDL_SCANCODE_ESCAPE]) game_active = false;

      //Physics
      if(!game.isResolved){
        physicalUpdate(game, deltaTime, windowRealSize);
      }

      // Graphics
      drawstuff(game, meterToPixels, kbdstate);

      // Loop clock
      this_thread::sleep_until(current_time = current_time + deltaTime);
    }
    SDL_Quit();
    return 0;
}
