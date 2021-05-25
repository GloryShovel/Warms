#include <functional>
#include <iostream>

class physical
{
public:
    std::array<double, 2> position;
    std::array<double, 2> velocity;
    std::array<double, 2> acceleration;

    void update(double dt_f, std::function<void(physical *, std::array<double, 2> &pos, std::array<double, 2> &vel)> callback_f)
    {
        using namespace tp::operators;
        std::cout << "acc: " << acceleration[0] << "," << acceleration[1] << " ||| " << velocity[0] << "," << velocity[1] << std::endl;
        auto new_position = position + velocity * dt_f + velocity * acceleration * dt_f * dt_f * 0.5;
        auto new_velocity = (velocity + acceleration * dt_f) * 0.94;
        callback_f(this,new_position, new_velocity);
    }
};
