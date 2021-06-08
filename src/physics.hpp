#include "vectors.hpp"
#include <functional>
#include <iostream>

class physical
{
public:
  // Scale to make physics more real
  //double scale = 2;

  double mass;
  double friction = 0.94;

  std::array<double, 2> position = {0,0};
  std::array<double, 2> velocity = {0,0};
  std::array<double, 2> acceleration = {0,0};
  std::array<double, 2> gravityAcc = {0,9.8};

  void update(double dt_f, std::function<void(physical *, std::array<double, 2> &pos, std::array<double, 2> &vel)> callback_f)
  {
    using namespace tp::operators;
    // just for debugging
    // std::cout << "pos: " << position[0] << "," << position[1] << " acc: " << acceleration[0] << "," << acceleration[1] << " vel" << velocity[0] << "," << velocity[1] << std::endl;
    auto new_acceleration = acceleration - velocity * length(velocity) * friction;
    auto new_velocity = (velocity + new_acceleration * dt_f) + (gravityAcc * 1);
    std::cout << new_velocity[1] << std::endl;
    auto new_position = position + new_velocity * dt_f + new_acceleration * dt_f * dt_f * 0.5;
    callback_f(this,new_position, new_velocity);
  }
};
