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

using json = nlohmann::json;

int main(int argc, const char * argv[]) {
  const char* filename = (argc > 1) ? argv[1] : "../../data/Munich/LOD2_4424_5482_solid.json";
  std::ifstream input(filename);
  json j;
  input >> j;
 
  if (j["metadata"]["crs"]["epsg"].is_null())
    std::cout << "CRS not specified" << std::endl;
  else
    std::cout << "CRS: " << j["metadata"]["crs"]["epsg"] << std::endl;
  
  //-- print the CRS
  if (j.count("metadata"))
    if (j["metadata"].count("crs"))
      if (j["metadata"]["crs"].count("epsg"))
        std::cout << "CRS: " << j["metadata"]["crs"]["epsg"]  << std::endl;
    
  for (json::iterator it = j.begin(); it != j.end(); ++it) {
    std::cout << it.key() << "\n";
  }
    
  std::cout << "=====" << std::endl;
  std::cout << j.is_object() << std::endl;
  std::cout << "=====" << std::endl;
    
//    std::cout << j["metadata"] << std::endl;
  for (auto& entry : j["metadata"])
    std::cout << entry << " " << entry.is_array() << std::endl;
    
    // find an entry
  if (j.find("vertices") != j.end()) {
    std::cout << "size vertices:" << j["vertices"].size() << std::endl;
  }
  std::cout << j.count("vertices") << std::endl;
  std::cout << j.count("verticess") << std::endl;
    
//    for (auto& each : j)
//        std::cout << each.size() << std::endl;
    
// write prettified JSON to another file
//    std::ofstream o("/Users/hugo/projects/cityjson/pretty.json");
//    o << std::setw(4) << j << std::endl;
    
    return 0;
}
