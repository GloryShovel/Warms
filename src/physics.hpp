#pragma once

//=============================
#include "vectors.hpp"
#include <functional>
#include <iostream>
//=============================

class Physical
{
public:
  double mass;
  double friction = 0.95;

  std::array<double, 2> old_position = {0,0};
  std::array<double, 2> old_velocity = {0,0};

  std::array<double, 2> position = {0,0};
  std::array<double, 2> velocity = {0,0};
  std::array<double, 2> acceleration = {0,0};
  std::array<double, 2> gravityAcc = {0,9.8};

  //Physical update
  void update(double dt_f);

  //Writes physical data to console
  void write();

  //Returns scaled position (for display)
  std::array<double, 2> get_scaled_position(int meterToPixels);

};
