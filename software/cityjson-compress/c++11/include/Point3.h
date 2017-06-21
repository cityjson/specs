
#include <string>
#include <array>

class Point3 {
public:
  Point3(double x, double y, double z, std::string id = "");
  std::string           get_key(int precision = 6);
  void                  translate(double x, double y, double z);
  std::array<double,3>  get_array_double();
  std::array<int,3>     get_array_int(int precision = 3);
private:
  std::string _id;
  double      _x;
  double      _y;
  double      _z;

};