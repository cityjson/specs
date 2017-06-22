/*
   ____ _ _             _ ____   ___  _   _ ____   ___  ____      _ 
  / ___(_) |_ _   _    | / ___| / _ \| \ | |___ \ / _ \| __ )    | |
 | |   | | __| | | |_  | \___ \| | | |  \| | __) | | | |  _ \ _  | |
 | |___| | |_| |_| | |_| |___) | |_| | |\  |/ __/| |_| | |_) | |_| |
  \____|_|\__|\__, |\___/|____/ \___/|_| \_|_____|\___/|____/ \___/ 
              |___/                                                 
  cityjson2obj
  Created by Hugo Ledoux on 14/06/2017.
  Copyright © 2017 Hugo Ledoux. All rights reserved.

*/

#include <iostream>
#include <fstream>
#include "json.hpp"
#include <set>
#include <string>

using json = nlohmann::json;

void write_surface(std::ofstream &output, std::vector<std::vector<int> > &onesurface) {
  output << "f";
  for (auto& v: onesurface[0]) {
    output << " " << int(v) + 1;
  }
  output << std::endl;
}

int main(int argc, const char * argv[]) {
  const char* filename = (argc > 1) ? argv[1] : "../../../../example-datasets/Munich/LOD2_4424_5482_solid.json";
  std::ifstream input(filename);
  json j;
  input >> j;

  std::ofstream output("out.obj", std::ifstream::out);

  //-- 1. process vertices
  if (j.count("transform") == 0) {
    for (auto& v : j["vertices"]) 
      output << "v " << v[0] << " " << v[1] << " " << v[2] <<std::endl;
  }
  else {
    for (auto& v : j["vertices"]) {
      double x = (double(v[0]) * double(j["transform"]["scale"][0])) + double(j["transform"]["translate"][0]);
      double y = (double(v[1]) * double(j["transform"]["scale"][1])) + double(j["transform"]["translate"][1]);
      double z = (double(v[2]) * double(j["transform"]["scale"][2])) + double(j["transform"]["translate"][2]);
      output << std::fixed << std::setprecision(6) << "v " << x << " " << y << " " << z << std::endl;
    } 
  }

  //-- 2. process faces
  for (json::iterator it = j["CityObjects"].begin(); it != j["CityObjects"].end(); ++it) {
    output << "o " << it.key() << std::endl;
    for (auto& g : it.value()["geometry"]) {
      // std::cout << g["type"] << std::endl;
      if (g["type"] == "Solid") {
        for (auto& shell : g["boundaries"]) {
          for (auto& surface : shell) { 
            // std::cout << surface << std::endl;
            std::vector< std::vector<int> > t = surface;
            write_surface(output, t);
          }
        }
      }
      else if ( (g["type"] == "MultiSurface") || (g["type"] == "CompositeSurface") ) {
        for (auto& surface : g["boundaries"]) { 
          std::vector< std::vector<int> > t = surface;
          write_surface(output, t);
        }
      }
      else if ( (g["type"] == "MultiSolid") || (g["type"] == "CompositeSolid") ) {
        for (auto& solid : g["boundaries"]) {
          for (auto& shell : solid) {
            for (auto& surface : shell) { 
              std::vector< std::vector<int> > t = surface;
              write_surface(output, t);
            }
          }
        }
      }            
    }
  }
  output.close();
  return 0;
}
