/*
 _____ _ _          __ _____ _____ _____     _     ___     
|     |_| |_ _ _ __|  |   __|     |   | |___|_|___|  _|___ 
|   --| |  _| | |  |  |__   |  |  | | | |___| |   |  _| . |
|_____|_|_| |_  |_____|_____|_____|_|___|   |_|_|_|_| |___|
            |___|                                          

  cityjson-info
  Created by Hugo Ledoux on 14/06/2017.
  Copyright © 2017 Hugo Ledoux. All rights reserved.

*/

#include <iostream>
#include <fstream>
#include "json.hpp"
#include <set>
#include <string>

using json = nlohmann::json;

int main(int argc, const char * argv[]) {
  const char* filename = (argc > 1) ? argv[1] : "../../../../example-datasets/Munich/LOD2_4424_5482_solid.json";
  std::ifstream input(filename);
  json j;
  input >> j;

  std::cout << "Printing (some) information about:" << std::endl;
  std::cout << "  " << filename << std::endl << std::endl;

  //-- CityJSON version
  std::string tmp = j["version"];
  std::string version;
  version = tmp.substr(tmp.size() - 3);
  std::cout << "CityJSON version: " << version << std::endl;
  
  //-- CityObjects
  std::cout << "===== CityObjects =====" << std::endl;
  std::cout << "Total : " << j["CityObjects"].size() << std::endl;
  std::set<std::string> d;
  for (auto& co : j["CityObjects"]) {
    std::string tmp = co["type"];
    d.insert(tmp);
  }
  std::cout << "Types:" << std::endl;
  for (auto& each : d)
    std::cout << "  " << each << std::endl; 
  d.clear();
  for (auto& co : j["CityObjects"]) {
    for (auto& g : co["geometry"]) {
      std::string tmp = g["type"];
      d.insert(tmp);
    }
  }
  std::cout << "Geometries present:" << std::endl;
  for (auto& each : d)
    std::cout << "  " << each << std::endl; 
  d.clear();

  //-- metadata
  std::cout << "===== Metadata =====" << std::endl;
  if (j.count("metadata") == 0)
    std::cout << "  none" << std::endl;
  else {
    for (json::iterator it = j["metadata"].begin(); it != j["metadata"].end(); ++it) {
      if (it.key() == "crs")
        std::cout << "  crs: EPSG:" << j["metadata"]["crs"]["epsg"] << std::endl;
      else  
        std::cout << "  " << it.key() << std::endl;
    }
  }

  //-- vertices
  std::cout << "===== Vertices =====" << std::endl;
  std::cout << "Total: " << j["vertices"].size() << std::endl;

  //-- appearance
  std::cout << "===== Appearance =====" << std::endl;
  if (j.count("appearance") == 0)
    std::cout << "  none" << std::endl;
  else {
    if (j["appearance"].count("textures") > 0)
      std::cout << "  textures: " << j["appearance"]["textures"].size() << std::endl;
    if (j["appearance"].count("materials") > 0)
      std::cout << "  materials: " << j["appearance"]["materials"].size() << std::endl;    
  }
    
    
// write prettified JSON to another file
//    std::ofstream o("/Users/hugo/projects/cityjson/pretty.json");
//    o << std::setw(4) << j << std::endl;
    
    return 0;
}
