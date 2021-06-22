#include "player.hpp"

void Player::apply_intent() {
 acceleration = {0, 0};
 if(isGrounded){
   if (intentions.count("left")) {
     acceleration[0] += -1 * speed;
   }
   if (intentions.count("right")) {
     acceleration[0] += 1 * speed;
   }
   //to think about
   if (intentions.count("up")) {
     velocity[1] += -1 * jumpStrenght;
     isGrounded = false;
   }
 }

 intentions.clear();
}

std::array<int, 2> Player::get_scaled_size(int meterToPixels){
 std::array<int, 2> result = {size[0] * meterToPixels, size[1] * meterToPixels};
 return result;
}
