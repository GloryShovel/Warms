
class Projectail : public physical
{
public:
  std::array<double, 2> size = {0.5,0.5};
  std::array<double, 2> direction = {0,0};

  int strenght = 0;

  bool isActive = false;

  Projectail(){
    position = {0,0};
  }

  void instanciate(Player player, int mouseX, int mouseY){
    using namespace tp::operators;

    //Setup position
    position[0] = player.position[0];
    position[1] = player.position[1] - 0.5;

    //Calculate direction of projectail
    std::array<double, 2> vec = player.position - std::array<double, 2>{mouseX, mouseY};
    direction = (vec / sqrt(vec[0]*vec[0]+vec[1]*vec[1]));

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
  }

  //need to set acceleration to 0,0
  void applyAcceleration(){
    using namespace tp::operators;

    //acceleration needs to be const and velocity is changing ?
    acceleration = acceleration + (direction * strenght * 1);

    strenght = 0;

  }

  void destroy(){
    isActive = false;
  }

  void explode(){
    isActive = false;
  }

  // WARNING!!! engine stuff
  std::array<int, 2> get_scaled_size(int meterToPixels){
    std::array<int, 2> result = {size[0] * meterToPixels, size[1] * meterToPixels};
    return result;
  }

};
