#pragma once

#include <memory>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "projectile.hpp"
#include <ctime>

class Game
{
public:
  std::shared_ptr<SDL_Window> window_p;
  std::shared_ptr<SDL_Renderer> renderer_p;
  std::shared_ptr<SDL_Texture> playerTexture;
  std::shared_ptr<SDL_Texture> mouseTexture;
  std::shared_ptr<SDL_Texture> projectileTexture;
  std::shared_ptr<SDL_Texture> explosionTexture;
  std::shared_ptr<SDL_Texture> pointerTexture;
  std::shared_ptr<SDL_Texture> groundTexture;
  std::shared_ptr<SDL_Texture> endTexture;
  std::shared_ptr<SDL_Texture> player1Texture;
  std::shared_ptr<SDL_Texture> player2Texture;

  int mouseX, mouseY, turn = 0;

  bool isResolved = false;

  Projectile projectile;

  std::array<Player,2> players;

  std::clock_t strength_timer;
};
