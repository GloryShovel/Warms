#include <memory>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "projectail.hpp"
#include <ctime>

class Game
{
public:
  std::shared_ptr<SDL_Window> window_p;
  std::shared_ptr<SDL_Renderer> renderer_p;
  std::shared_ptr<SDL_Texture> playerTexture;
  std::shared_ptr<SDL_Texture> mouseTexture;
  std::shared_ptr<SDL_Texture> projectailTexture;
  Player player;
  int mouseX, mouseY;
  Projectail projectail;
  std::clock_t strenght_timer;
};
