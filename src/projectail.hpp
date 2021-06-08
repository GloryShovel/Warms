
class Projectail : public physical
{
public:
  std::array<int, 2> size = {30,30};

  int strenght = 0;

  bool isActive = false;

  Projectail(){
    position = {0,0};
  }

  Projectail(std::array<double, 2> pos){
    position = pos;
  }

  void setup(Player player){
    position = player.position;
  }

  void destroy(){

  }

};
