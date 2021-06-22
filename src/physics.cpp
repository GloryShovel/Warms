#include "physics.hpp"

void Physical::update(double dt_f)
{
  using namespace tp::operators;

  auto new_acceleration = (acceleration - velocity * length(velocity) * friction) + (gravityAcc);
  auto new_velocity = (velocity + new_acceleration * dt_f);
  auto new_position = position + new_velocity * dt_f + new_acceleration * dt_f * dt_f * 0.5;

  //Save old position for preventing movvement
  old_position = position;
  old_velocity = velocity;

  //Pass new data to object
  velocity = new_velocity;
  position = new_position;
}

void Physical::write(){
  //Just devStuff
  std::cout << "acc:" << acceleration[0] << "," << acceleration[1] << std::endl;
  std::cout << "vel:" << velocity[0]<< "," << velocity[1] << std::endl;
  std::cout << "pos:" << position[0] << "," << position[1] << std::endl;
}

std::array<double, 2> Physical::get_scaled_position(int meterToPixels){
 std::array<double, 2> result = {position[0] * meterToPixels, position[1] * meterToPixels};
 return result;
}
