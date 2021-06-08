#include <map>


class Player : public physical
{
public:
  int hp = 100;
  int speed = 40;

  std::map<std::string, int> intentions;

  std::array<int, 2> size;

  Player()
  {
    position = {50, 50};
    size = {50, 50};
  }

    // Player(std::array<double, 2> pos)
    // {
    //     position = pos;
    //     size = {50, 50};
    // }

 void apply_intent() {
   acceleration = {0, 0};
   if (intentions.count("left")) {
     acceleration[0] += -100 * speed;
   }
   if (intentions.count("right")) {
     acceleration[0] += 100 * speed;
   }

   intentions.clear();
 }
};
