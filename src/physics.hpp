#include <functional>
#include <iostream>

class physical
{
public:
  double mass;

  std::array<double, 2> position = {0,0};
  std::array<double, 2> velocity = {0,0};
  std::array<double, 2> acceleration = {0,0};
  std::array<double, 2> gravityAcc = {0,9.80};

  void update(double dt_f, std::function<void(physical *, std::array<double, 2> &pos, std::array<double, 2> &vel)> callback_f)
  {
    using namespace tp::operators;
    // just for debugging
    // std::cout << "pos: " << position[0] << "," << position[1] << " acc: " << acceleration[0] << "," << acceleration[1] << " vel" << velocity[0] << "," << velocity[1] << std::endl;
    auto new_position = position + velocity * dt_f + velocity * acceleration * dt_f * dt_f * 0.5;
    auto new_velocity = (velocity + acceleration * dt_f) * 0.94 + gravityAcc;
    callback_f(this,new_position, new_velocity);
  }
};
