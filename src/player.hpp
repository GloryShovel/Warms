#include <map>


class Player : public physical
{
public:
    std::map<std::string, int> intentions;
    std::array<int, 2> size;

    Player()
    {
        position = {100, 100};
        velocity = {0, 0};
        size = {50, 50};
    }

    Player(std::array<double, 2> pos, std::array<double, 2> vel)
    {
        position = pos;
        velocity = vel;
        size = {10, 10};
    }

/**
 * applies and clears intentions
 * */
    void apply_intent() {
        acceleration = {0, 50};
        if (intentions.count("left")) acceleration[0] += -100;
        if (intentions.count("right")) acceleration[0] += 100;

        intentions.clear();
    }
};
