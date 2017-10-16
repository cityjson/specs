/*
 _____ _ _          __ _____ _____ _____     _     ___     
|     |_| |_ _ _ __|  |   __|     |   | |___|_|___|  _|___ 
|   --| |  _| | |  |  |__   |  |  | | | |___| |   |  _| . |
|_____|_|_| |_  |_____|_____|_____|_|___|   |_|_|_|_| |___|
            |___|                                          

  cityjson-compress
  Created by Hugo Ledoux on 14/06/2017.
  Copyright © 2017 Hugo Ledoux. All rights reserved.

*/

#include <iostream>
#include <fstream>
#include "json.hpp"
#include <set>
#include <vector>
#include <string>
#include "Point3.h"
#include <cstdlib>


bool bConvertInt = false;

using json = nlohmann::json;

int main(int argc, const char * argv[]) {
  const char* inputfile = argv[1];
  const char* d = (argc > 2) ? argv[2] : "3";
  int importantdigits = atoi(d);
  std::cout << "Input file: " << inputfile << std::endl;
  std::cout << "Number of digits kept after the dot: " << importantdigits << std::endl;
  std::ifstream input(inputfile);

  
  //-- size input file
  std::streampos sizei;
  std::streampos begin, end;
  begin = input.tellg();
  input.seekg(0, std::ios::end);
  end = input.tellg();
  sizei = end - begin;
  // std::cout << "size is: " << sizei << " bytes.\n";
  input.seekg(0);
  
  json j;
  input >> j;
  input.close();

  std::cout << "size input vertices: " << j["vertices"].size() << std::endl;

  //-- vertices
  std::vector<Point3> vertices;
  double minx = 1e9;
  double miny = 1e9;
  double minz = 1e9;
  for (auto& v : j["vertices"]) {
    Point3 tmp(v[0], v[1], v[2]);
    if (v[0] < minx)
      minx = v[0];
    if (v[1] < miny)
      miny = v[1];    
    if (v[2] < minz)
      minz = v[2];    
    vertices.push_back(tmp);
  }

  //-- merge close ones (based on a tolerance)
  std::vector<unsigned long> newids (vertices.size(), -1);
  std::map<std::string,unsigned long> m;
  int i = 0;
  int totalmerged = 0;
  for (auto& v : vertices) {
    auto it = m.find(v.get_key(importantdigits));
    if (it == m.end()) {
      unsigned long a = m.size();
      newids[i] = a;
      m[v.get_key(importantdigits)] = a;
    }
    else {
      newids[i] = it->second;
      totalmerged++;
    }
    i++;
  }
  std::cout << "---" << std::endl;
  std::cout << std::cout.precision(3) << std::fixed << "bbox min: (" << minx << ", " << miny << ", " << minz << ")" << std::endl;
  std::cout << "Vertices" << std::endl;
  std::cout << "\tinput: "  << j["vertices"].size() << std::endl;
  std::cout << "\tmerged: " << totalmerged << std::endl;
  std::cout << "\toutput: " << m.size() << std::endl;

  // for (auto& each: newids)
  //   std::cout << "--" << each << std::endl;

  //-- update the indices
  for (auto& co : j["CityObjects"]) {
    for (auto& g : co["geometry"]) {
      if (g["type"] == "Solid") {
        for (auto& shell : g["boundaries"]) 
          for (auto& surface : shell) 
            for (auto& ring : surface) 
              for (auto& v : ring) 
                if (newids[v] != -1) 
                  v = newids[v]; 
      }
      else if ( (g["type"] == "MultiSurface") || (g["type"] == "CompositeSurface") ) {
        for (auto& surface : g["boundaries"]) 
          for (auto& ring : surface) 
            for (auto& v : ring) 
              if (newids[v] != -1) 
                v = newids[v]; 
      }
      else if ( (g["type"] == "MultiSolid") || (g["type"] == "CompositeSolid") ) {
        for (auto& solid : g["boundaries"]) 
          for (auto& shell : solid) 
            for (auto& surface : shell) 
              for (auto& ring : surface) 
                for (auto& v : ring) 
                  if (newids[v] != -1) 
                    v = newids[v];
      }            
    }
  }

  if (bConvertInt == true) { //-- convert to int and write the transform
    std::cout << "Converting to integer coordinates." << std::endl;
    std::vector<std::array<int, 3>> vout(m.size());
    for (auto& newid: newids) {
      Point3 v(j["vertices"][newid][0], j["vertices"][newid][1], j["vertices"][newid][2]);
      v.translate(-minx, -miny, -minz); 
      vout[newid] = v.get_array_int(importantdigits);
    }
    j["vertices"] = vout;
    double scalefactor = 1 / (pow(10, importantdigits));
    j["transform"]["scale"] = {scalefactor, scalefactor, scalefactor};
    j["transform"]["translate"] = {minx, miny, minz};
  }
  else {
    std::vector<std::array<double, 3>> vout(m.size());
    for (auto& newid: newids) {
      vout[newid] = j["vertices"][newid];
    }
    j["vertices"] = vout;
  }
  std::cout << "done." << std::endl;

  
  //-- write prettified JSON to another file
  std::string s = inputfile;
  std::size_t found = s.find(".json");
  if (found != std::string::npos) {
    s.insert(found, ".compress");
    std::cout << "File saved: " << s << std::endl;
    std::ofstream o(s);
    o << j << std::endl;
    // o << j.dump(2) << std::endl;
    std::cout << "size output vertices: " << j["vertices"].size() << std::endl;

    //-- size output file
    std::ifstream outputfile(s);
    std::streampos sizeo;
    begin = outputfile.tellg();
    outputfile.seekg(0, std::ios::end);
    end = outputfile.tellg();
    sizeo = end - begin;
    // std::cout << "size is: " << sizeo << " bytes.\n";
    outputfile.close();
    std::cout << "Compression factor: " << float(sizei)/float(sizeo) << std::endl;
  }
      
  return 0;
}
