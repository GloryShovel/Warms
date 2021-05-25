
class Projectail : public physical
{
public:
  std::array<int, 2> size = {30,30};

  Projectail(std::array<double, 2> pos){
    position = pos;
  }

};
