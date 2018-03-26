
#include "Point3.h"
#include "iostream"
#include <sstream>
#include <cmath>

Point3::Point3(double x, double y, double z, std::string id) {
  _x = x;
  _y = y;
  _z = z;
  _id = id;
}

std::string Point3::get_key(int precision) {
  char* buf = new char[100];
  std::stringstream ss;
  ss << "%." << precision << "f " << "%." << precision << "f " << "%." << precision << "f";
  std::sprintf(buf, ss.str().c_str(), _x, _y, _z);
  return buf;
}

void Point3::translate(double x, double y, double z) {
  _x += x;
  _y += y;
  _z += z;
}

std::array<double,3> Point3::get_array_double() {
  return {{_x, _y, _z}};
}

std::array<int,3> Point3::get_array_int(int precision) {
  return {{int(_x * pow(10, precision)), int(_y * pow(10, precision)), int(_z * pow(10, precision))}};
}

double Point3::x() {
  return _x;
}

double Point3::y() {
  return _y;
}

double Point3::z() {
  return _z;
}
