#include <map>


class Player : public physical
{
public:
  int hp = 100;
  int speed = 40;

  bool isGrounded = false;

  std::map<std::string, int> intentions;

  std::array<double, 2> size;

  Player()
  {
    position = {0.5, 0.5};
    size = {1.5, 1.5};
  }

    // Player(std::array<double, 2> pos)
    // {
    //     position = pos;
    //     size = {50, 50};
    // }

 void apply_intent() {
   acceleration = {0, 0};
   if (intentions.count("left")) {
     acceleration[0] += -1 * speed;
   }
   if (intentions.count("right")) {
     acceleration[0] += 1 * speed;
   }
   //to think about
   if (intentions.count("up") && isGrounded) {
     acceleration[1] += -1 * speed;
     isGrounded = false;
   }

   intentions.clear();
 }

 std::array<int, 2> get_scaled_size(int meterToPixels){
   std::array<int, 2> result = {size[0] * meterToPixels, size[1] * meterToPixels};
   return result;
 }

};
