#include "projectile.hpp"
#include <math.h>

void Projectile::instanciate(Player player, int mouseX, int mouseY, int meterToPixels){
  using namespace tp::operators;

  //Setup position
  position[0] = player.position[0];
  position[1] = player.position[1] - 0.5;

  //Calculate direction of projectile
  std::array<double, 2> vec = std::array<double, 2>{mouseX/meterToPixels, mouseY/meterToPixels} - player.position;
  direction = normalize(vec);

  //Just devStuff
  /*
  std::cout << "pp:" << player.position[0]<< "," << player.position[1] << std::endl;
  std::cout << "mp:" << mouseX<< "," << mouseY << std::endl;
  std::cout << "vec:" << vec[0]<< "," << vec[1] << std::endl;
  std::cout << "vec_n:" << direction[0] << "," << direction[1] << std::endl;
  std::cout << std::endl;
  */

  //Activate projectail
  isActive = true;

  //Apply initial velocity
  applyVelocity(direction * strength);
}

//need to set acceleration to 0,0
void Projectile::applyVelocity(std::array<double, 2> vector){
  using namespace tp::operators;

  velocity = velocity + vector;
}

void Projectile::animate(){
  using namespace tp::operators;

  std::array<double, 2> right = {1,0};
  //Angle calculation (interpolate in 180 angle with recalculated -1,1 values from dot product that is recalculated to be 0,1 values)
  angle = lerp(180, 0, (dot(normalize(velocity), right) + 1)/2);
  //Check velocity direction (basicly i need to set angle or -angle becouse of interpolatin on 0-180 values)
  if(velocity[1] < 0){
    angle *= -1;
  }
  //sevStuff
  // std::cout << "vel:" << velocity[0] << "," << velocity[1] << " angle:" << angle << std::endl;
}

void Projectile::destroy(){
  clear();
}

void Projectile::explode(std::array<Player, 2>& players){
  using namespace tp::operators;

  //Animation
  isExploding = true;

  //Collision detection
  for(int i = 0; i < players.size(); i++){
    double dist = length(players[i].position - position);
    if(dist <= expRadius){
      players[i].hp -= damage * (1.5-dist);
    }
    //devStuff
    std::cout << "Player" << i << ": " << dist << " HP:" << players[i].hp << std::endl;
  }

  //Reseting values of projectile
  clear();
}

void Projectile::clear(){
  velocity = {0,0};
  acceleration = {0,0};

  strength = 0;
  angle = 0;
  direction = {0,0};
  isActive = false;
}

void Projectile::write(){
  Physical::write();
  std::cout << "dir:" << direction[0] << "," << direction[1] << " angle:" << angle << " strength:" << strength << std::endl;
  std::cout << std::endl;
}

void Projectile::updateExplosion(){
  //Handle explosion timer
  if(isExploding && explosionTimer){
    explosionTimer --;
  }else{
    isExploding = false;
    explosionTimer = 10;
  }
}

// WARNING!!! engine stuff
std::array<int, 2> Projectile::get_scaled_size(int meterToPixels){
  std::array<int, 2> result = {size[0] * meterToPixels, size[1] * meterToPixels};
  return result;
}

std::array<int, 2> Projectile::get_scaled_exp_size(int meterToPixels){
  std::array<int, 2> result = {explosionSize[0] * meterToPixels, explosionSize[1] * meterToPixels};
  return result;
}
