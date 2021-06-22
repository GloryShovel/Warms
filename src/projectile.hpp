#pragma once

//================================
#include "physics.hpp"
#include "player.hpp"
#include <iostream>
#include <cmath>
//================================

class Projectile : public Physical
{
public:
  int strength = 0;
  int explosionTimer = 10;

  double angle = 0;
  double expRadius = 1.5;
  double damage = 20;

  bool isActive = false;
  bool isExploding = false;

  std::array<double, 2> size = {0.5,0.5};
  std::array<double, 2> explosionSize = {3,3};
  std::array<double, 2> direction = {0,0};

  //This is for handling the strenght, direction and activation of the projectile
  void instanciate(Player player, int mouseX, int mouseY, int meterToPixels);

  //This is for applying velocity to projectile object when shooting starts
  void applyVelocity(std::array<double, 2> vector);

  //Calculates rotation of the bullet to apply it to drawing function
  void animate();

  /*
  Handles the projectile disapirence
  NOTE: There is only one projectile in this game (optimalizaion master XD)
  */
  void destroy();

  //Deactivate projectile and play an explosion where projectile landed
  void explode(std::array<Player, 2>& players);

  //Sets values bac to 0 (some wierd shootng is happening)
  void clear();

  //Overshadow write() with more than just physics
  void write();

  //Overloads updating for explosion
  void updateExplosion();

  // ENGINE STUFF!!! This rescales the size of a projectile back to pixels
  std::array<int, 2> get_scaled_size(int meterToPixels);

  std::array<int, 2> get_scaled_exp_size(int meterToPixels);

};
