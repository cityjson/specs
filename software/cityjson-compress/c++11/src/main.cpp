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
#include <set>
#include <vector>
#include <string>
#include <cstdlib>

#include "nlohmann-json/json.hpp"
#include "tclap/CmdLine.h"

#include "Point3.h"


bool bConvertInt = false;

using json = nlohmann::json;

int main(int argc, const char * argv[]) {


try {  
  TCLAP::CmdLine cmd("Allowed options", ' ', "");

  TCLAP::UnlabeledValueArg<std::string>   inputfile(
                                              "inputfile", 
                                              "input CityJSON file", 
                                              true, 
                                              "", 
                                              "string");
  TCLAP::SwitchArg      intvertex("",
                                  "intvertex",
                                  "convert vertices to integer",
                                  false);
  TCLAP::SwitchArg      prettified("",
                                  "prettified",
                                  "prettified JSON",
                                  false);
  TCLAP::ValueArg<int>  tolerance("", 
                                  "tolerance",
                                  "tolerance for merging duplicates and converting to int",
                                  false,
                                  3,
                                  "integer");
  
  cmd.add(inputfile);
  cmd.add(intvertex);
  cmd.add(tolerance);
  cmd.parse( argc, argv );
  
  std::string ifile = inputfile.getValue();
  int importantdigits = tolerance.getValue();
  std::cout << "Input file: " << ifile << std::endl;
  std::cout << "Number of digits kept after the dot: " << importantdigits << std::endl;
  std::ifstream input(ifile);

  
  //-- size input file
  std::streampos sizei;
  std::streampos begin, end;
  begin = input.tellg();
  input.seekg(0, std::ios::end);
  end = input.tellg();
  sizei = end - begin;
  input.seekg(0);
  
  json j;
  input >> j;
  input.close();

  std::cout << "Number input vertices: " << j["vertices"].size() << std::endl;

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

  //-- merge close vertices (based on a tolerance)
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

  std::vector<bool> usedids (vertices.size(), false);
  //-- update the indices
  for (auto& co : j["CityObjects"]) {
    for (auto& g : co["geometry"]) {
      if (g["type"] == "Solid") {
        for (auto& shell : g["boundaries"]) 
          for (auto& surface : shell) 
            for (auto& ring : surface) 
              for (auto& v : ring) 
                if (newids[v] != -1) {
                  v = newids[v]; 
                  usedids[v] = true;
                }
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
  std::cout << "ici" << std::endl;
  //-- purge unused ones
  int shift = 0;
  for (auto& newid: newids) {
    std::cout << newid << std::endl;
    std::cout << usedids[newid] << std::endl;
    if (usedids[newid] == true) {
      newid -= shift;
    }
    else {
      newid = -1;
      shift++;
    }
  }
  std::cout << "là" << std::endl;
  int newtotal = 0;
  for (auto& newid: newids) 
    if (newid != -1)
      newtotal++;
  std::cout << "newtotal: " << newtotal << std::endl;

  if (intvertex.getValue() == true) { //-- convert to int and write the transform
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
    std::vector<std::array<double, 3>> vout(newtotal);
    for (auto& newid: newids) {
      std::cout << newid << std::endl; 
      if (newid != -1)
        vout[newid] = j["vertices"][newid];
    }
    j["vertices"] = vout;
  }

    
  std::string s = ifile;
  std::size_t found = s.find(".json");
  if (found != std::string::npos) {
    s.insert(found, ".compress");
    std::cout << "File saved: " << s << std::endl;
    std::ofstream o(s);
    if (prettified.getValue() == true)
      o << j.dump(2) << std::endl;
    else
      o << j << std::endl;
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
  catch (TCLAP::ArgException &e)
  { 
    std::cerr << "error: " << e.error() << " for arg " << e.argId() << std::endl; 
  }
}
