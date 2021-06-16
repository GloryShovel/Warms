#include "vectors.hpp"
#include <functional>
#include <iostream>

class physical
{
public:
  // Scale to make physics more real
  //double scale = 2;

  double mass;
  double friction = 0.97;

  std::array<double, 2> old_position = {0,0};
  std::array<double, 2> old_velocity = {0,0};

  std::array<double, 2> position = {0,0};
  std::array<double, 2> velocity = {0,0};
  std::array<double, 2> acceleration = {0,0};
  std::array<double, 2> gravityAcc = {0,9.8};

  void update(double dt_f)
  {
    using namespace tp::operators;

    auto new_acceleration = (acceleration - velocity * length(velocity) * friction) + (gravityAcc);
    auto new_velocity = (velocity + new_acceleration * dt_f);
    auto new_position = position + new_velocity * dt_f + new_acceleration * dt_f * dt_f * 0.5;

    //Save old position for preventing movvement
    old_position = position;
    old_velocity = velocity;

    //Pass new data to object
    acceleration = new_acceleration;
    velocity = new_velocity;
    position = new_position;
  }

  void write(){
    //Just devStuff
    std::cout << "acc:" << acceleration[0] << "," << acceleration[1] << std::endl;
    std::cout << "vel:" << velocity[0]<< "," << velocity[1] << std::endl;
    std::cout << "pos:" << position[0] << "," << position[1] << std::endl;
    std::cout << std::endl;
  }

};
