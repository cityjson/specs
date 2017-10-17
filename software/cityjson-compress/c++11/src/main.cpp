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
#include <boost/program_options.hpp>

#include "json.hpp"
#include "Point3.h"

using json = nlohmann::json;

void remove_unused_vertices(json& j);
void merge_duplicate_vertices(json& j, int importantdigits);
void convert_vertices_to_integer(json& j, int importantdigits);
void save_to_file(json& j, std::string ifile, std::streampos& sizei);

int main(int argc, const char * argv[]) {
  
  int importantdigits;
  std::string ifile; 
  bool bv2int = false;
  try {
    namespace po = boost::program_options;
    po::options_description pomain("Allowed options");
    pomain.add_options()
    ("help", "View all options")
    ("tolerance", po::value<int>(&importantdigits)->default_value(3), "number of digit to keep for merging duplicates")
    ("v2int", "Convert vertices to integer (Transform/Quantize)")
    ;
    po::options_description pohidden("Hidden options");
    pohidden.add_options()
    ("inputfile", po::value<std::string>(&ifile), "Input CityJSON file")
    ;        
    po::positional_options_description popos;
    popos.add("inputfile", -1);
    
    po::options_description poall;
    poall.add(pomain).add(pohidden);
    po::variables_map vm;
    po::store(po::command_line_parser(argc, argv).
              options(poall).positional(popos).run(), vm);
    po::notify(vm);
    
    if (vm.count("help")) {
      std::cout << "=== cjcompress help ===" << std::endl;
      std::cout << pomain << std::endl;
      return 0;
    }
    if (vm.count("inputfile") == 0) {
      std::cerr << "Error: one input CityJSON file must be specified." << std::endl;
      std::cout << std::endl << pomain << std::endl;
      return 0;  
    }
    if (vm.count("v2int")) 
      bv2int = true;
  } 
  catch(std::exception& e) {
    std::cerr << "Error: " << e.what() << "\n";
    return 1;
  }  
  
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

  size_t noinputvertices = j["vertices"].size();
  merge_duplicate_vertices(j, importantdigits);
  remove_unused_vertices(j);
  std::cout << "Number duplicates/unused vertices removed: " << (noinputvertices - j["vertices"].size()) << std::endl;
  if (bv2int == true)
    convert_vertices_to_integer(j, importantdigits);
  save_to_file(j, ifile, sizei);
  return 0;
}


void convert_vertices_to_integer(json& j, int importantdigits) {
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
  std::cout << "Converting to integer coordinates." << std::endl;
  std::vector<std::array<int, 3>> vout; 
  for (auto& v : j["vertices"]) {
    Point3 p(v[0], v[1], v[2]);
    p.translate(-minx, -miny, -minz);
    vout.push_back(p.get_array_int(importantdigits));
  }
  j["vertices"] = vout;
  double scalefactor = 1 / (pow(10, importantdigits));
  j["transform"]["scale"] = {scalefactor, scalefactor, scalefactor};
  j["transform"]["translate"] = {minx, miny, minz};
}



void save_to_file(json& j, std::string ifile, std::streampos& sizei) {
  std::streampos begin, end;
  std::size_t found = ifile.find(".json");
  if (found != std::string::npos) {
    ifile.insert(found, ".compress");
    std::cout << "File saved: " << ifile << std::endl;
    std::ofstream o(ifile);
    o << j << std::endl;
    //-- size output file
    std::ifstream outputfile(ifile);
    std::streampos sizeo;
    begin = outputfile.tellg();
    outputfile.seekg(0, std::ios::end);
    end = outputfile.tellg();
    sizeo = end - begin;
    // std::cout << "size is: " << sizeo << " bytes.\n";
    outputfile.close();
    std::cout << "Compression factor: " << float(sizei)/float(sizeo) << std::endl;
  }
}


void merge_duplicate_vertices(json& j, int importantdigits) {
  std::vector<Point3> vertices;
  for (auto& v : j["vertices"]) {
    Point3 tmp(v[0], v[1], v[2]);
    vertices.push_back(tmp);
  }
  std::map<std::string,unsigned long> hash;
  std::vector<unsigned long> newids (vertices.size(), 1);
  unsigned long i = 0;
  unsigned long totalmerged = 0;
  for (auto& v : vertices) {
    auto it = hash.find(v.get_key(importantdigits));
    if (it == hash.end()) {
      newids[i] = i;
      hash[v.get_key(importantdigits)] = int(hash.size());
    }
    else {
      newids[i] = it->second;
      totalmerged++;
    }
    i++;
  }
  std::cout << "Merged " << totalmerged << " duplicate vertices." << std::endl;
  //-- update IDs for the faces
  for (auto& co : j["CityObjects"]) {
    for (auto& g : co["geometry"]) {
      if (g["type"] == "Solid") {
        for (auto& shell : g["boundaries"])
          for (auto& surface : shell)
            for (auto& ring : surface)
              for (auto& v : ring)
                v = newids[v];
      }
      else if ( (g["type"] == "MultiSurface") || (g["type"] == "CompositeSurface") ) {
        for (auto& surface : g["boundaries"])
          for (auto& ring : surface)
            for (auto& v : ring)
              v = newids[v];  
      }
      else if ( (g["type"] == "MultiSolid") || (g["type"] == "CompositeSolid") ) {
        for (auto& solid : g["boundaries"])
          for (auto& shell : solid)
            for (auto& surface : shell)
              for (auto& ring : surface)
                for (auto& v : ring)
                  v = newids[v];
      }
    }
  }
}


void remove_unused_vertices(json& j) {
  std::map<int,int> oldnewids;
  std::vector<int> newvertices;
  for (auto& co : j["CityObjects"]) 
    for (auto& g : co["geometry"]) 
      if (g["type"] == "Solid") {
        for (auto& shell : g["boundaries"]) 
          for (auto& surface : shell) 
            for (auto& ring : surface) 
              for (auto& v : ring) 
                if (oldnewids.find(v) == oldnewids.end()) {
                  oldnewids[v] = int(newvertices.size());
                  newvertices.push_back(v);
                }
      }
      else if ( (g["type"] == "MultiSurface") || (g["type"] == "CompositeSurface") ) {
        for (auto& surface : g["boundaries"])
          for (auto& ring : surface)
            for (auto& v : ring)
              if (oldnewids.find(v) == oldnewids.end()) {
                oldnewids[v] = int(newvertices.size());
                newvertices.push_back(v);
              }  
      }
      else if ( (g["type"] == "MultiSolid") || (g["type"] == "CompositeSolid") ) {
        for (auto& solid : g["boundaries"])
          for (auto& shell : solid)
            for (auto& surface : shell)
              for (auto& ring : surface)
                for (auto& v : ring)
                  if (oldnewids.find(v) == oldnewids.end()) {
                   oldnewids[v] = int(newvertices.size());
                    newvertices.push_back(v);
                  }
      }      

  //-- update the faces ids
  for (auto& co : j["CityObjects"]) {
    for (auto& g : co["geometry"]) 
      if (g["type"] == "Solid") {
        for (auto& shell : g["boundaries"]) 
          for (auto& surface : shell) 
            for (auto& ring : surface) 
              for (auto& v : ring) 
                v = oldnewids[v];
      }
      else if ( (g["type"] == "MultiSurface") || (g["type"] == "CompositeSurface") ) {
        for (auto& surface : g["boundaries"])
          for (auto& ring : surface)
            for (auto& v : ring)
              v = oldnewids[v];  
      }
      else if ( (g["type"] == "MultiSolid") || (g["type"] == "CompositeSolid") ) {
        for (auto& solid : g["boundaries"])
          for (auto& shell : solid)
            for (auto& surface : shell)
              for (auto& ring : surface)
                for (auto& v : ring)
                  v = oldnewids[v];
      }      
  }
  //-- replace the vertices
  std::vector<std::array<double, 3>> vout; 
  for (auto& v : newvertices) 
    vout.push_back(j["vertices"][v]);
  j["vertices"] = vout;
}





