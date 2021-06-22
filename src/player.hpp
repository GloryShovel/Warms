#pragma once

//===============================
#include <map>
#include "physics.hpp"
//===============================


class Player : public Physical
{
public:
  int speed = 4;
  int jumpStrenght = 6;

  double hp = 100;

  bool isGrounded = false;

  std::array<double, 2> size = {1.5, 1.5};

  std::map<std::string, int> intentions;

  //Applying movement based on intensions map
  void apply_intent();

  //Returns scaled size (for display)
  std::array<int, 2> get_scaled_size(int meterToPixels);

};
