/*

 +-----------------------------------------------------+
 |                                                     |
 |     _____ _ _              _  _____  ____  _   _    |
 |    / ____(_) |            | |/ ____|/ __ \| \ | |   |
 |   | |     _| |_ _   _     | | (___ | |  | |  \| |   |
 |   | |    | | __| | | |_   | |\___ \| |  | | . ` |   |
 |   | |____| | |_| |_| | |__| |____) | |__| | |\  |   |
 |    \_____|_|\__|\__, |\____/|_____/ \____/|_| \_|   |
 |                  __/ |                              |
 |                 |___/                               |
 |                                                     |
 |                                                     |
 +-----------------------------------------------------+
 +-----------------------------------------------------+
 |                +-+-+-+-+-+-+-+-+-+-+                |
 |                |c|j|c|o|m|p|r|e|s|s|                |
 |                +-+-+-+-+-+-+-+-+-+-+                |
 +-----------------------------------------------------+

 Copyright © 2017-2018 Hugo Ledoux. All rights reserved.
 
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

int  duplicate_vertices(json& j, int importantdigits);
int  duplicate_vertices_int(json& j);
int  remove_orphan_vertices(json& j);
void save_to_file(json& j, std::string ifile, std::streampos& sizei);
void tokenize(const std::string& str, std::vector<std::string>& tokens);

int main(int argc, const char * argv[]) {
  
  int importantdigits;
  std::string ifile; 
  try {
    namespace po = boost::program_options;
    po::options_description pomain("Allowed options");
    pomain.add_options()
    ("help", "View all options")
    ("tolerance", po::value<int>(&importantdigits)->default_value(3), "number of digit to keep for merging duplicates")
    ("no_duplicates", "Do *not* remove the duplicate vertices")
    ("no_orphans", "Do *not* remove the orphans")
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

  // if vertices already on integers
  if (j.count("transform") > 0) {
    int noduplicates = duplicate_vertices_int(j);
    std::cout << "Number duplicates vertices removed: " << noduplicates << std::endl;
  }
  else {
    int noduplicates = duplicate_vertices(j, importantdigits);
    std::cout << "Number duplicates vertices removed: " << noduplicates << std::endl;
  }
  int noorphans = remove_orphan_vertices(j);
  std::cout << "Number unused vertices removed: " << noorphans << std::endl;
  save_to_file(j, ifile, sizei);
  return 0;
}



void save_to_file(json& j, std::string ifile, std::streampos& sizei) {
  std::streampos begin, end;
  std::size_t found = ifile.find(".json");
  if (found != std::string::npos) {
    ifile.insert(found, ".compress");
    // ifile.insert(found, ".test");
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


void update_to_new_ids(json& j, std::vector<unsigned long> &newids) {
  for (auto& co : j["CityObjects"]) {
    for (auto& g : co["geometry"]) {
      if (g["type"] == "GeometryInstance") {
        g["boundaries"][0] = newids[g["boundaries"][0]];
      }
      else if (g["type"] == "Solid") {
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


int duplicate_vertices_int(json& j) {
  size_t inputsize = j["vertices"].size();
  //-- read points and translate now (if int)
  // std::vector<Point3> vertices;
  // for (auto& v : j["vertices"]) {
  //   std::vector<double> t = v;
  //   Point3 tmp(t[0], t[1], t[2]);
  //   tmp.translate(-minx, -miny, -minz);
  //   vertices.push_back(tmp);
  // }
  
  std::map<std::string,unsigned long> hash;
  std::vector<unsigned long> newids (j["vertices"].size(), 0);
  std::vector<std::string> newvertices;
  unsigned long i = 0;
  for (auto& v : j["vertices"]) {
    std::string thekey = std::to_string(int(v[0])) + " "; 
    thekey += std::to_string(int(v[1])) + " ";
    thekey += std::to_string(int(v[2]));
    // std::cout << thekey << std::endl;
    // return 0;
    // std::string thekey = v.get_key(importantdigits);
    auto it = hash.find(thekey);
    if (it == hash.end()) {
      unsigned long newid = (unsigned long)(hash.size());
      newids[i] = newid;
      hash[thekey] = newid;
      newvertices.push_back(thekey);
    }
    else {
      newids[i] = it->second;
    }
    i++;
  }
  //-- update IDs for the faces
  update_to_new_ids(j, newids);
  //-- replace the vertices
  std::vector<std::array<int, 3>> vout;
  for (std::string& s : newvertices) {
    std::vector<std::string> ls;
    tokenize(s, ls);
    std::array<int,3> t;
    t[0] = std::stoi(ls[0]);
    t[1] = std::stoi(ls[1]);
    t[2] = std::stoi(ls[2]);
    vout.push_back(t);
  }
  j["vertices"] = vout;
  return (inputsize - j["vertices"].size());
}


int duplicate_vertices(json& j, int importantdigits) {
  size_t inputsize = j["vertices"].size();
  //-- find bbox
  double minx = 1e9;
  double miny = 1e9;
  double minz = 1e9;
  for (auto& v : j["vertices"]) {
    if (v[0] < minx)
      minx = v[0];
    if (v[1] < miny)
      miny = v[1];
    if (v[2] < minz)
      minz = v[2];
  }
  //-- read points and translate now (if int)
  std::vector<Point3> vertices;
  for (auto& v : j["vertices"]) {
    std::vector<double> t = v;
    Point3 tmp(t[0], t[1], t[2]);
    tmp.translate(-minx, -miny, -minz);
    vertices.push_back(tmp);
  }
  
  std::map<std::string,unsigned long> hash;
  std::vector<unsigned long> newids (vertices.size(), 0);
  std::vector<std::string> newvertices;
  unsigned long i = 0;
  for (auto& v : vertices) {
    std::string thekey = v.get_key(importantdigits);
    auto it = hash.find(thekey);
    if (it == hash.end()) {
      unsigned long newid = (unsigned long)(hash.size());
      newids[i] = newid;
      hash[thekey] = newid;
      newvertices.push_back(thekey);
    }
    else {
      newids[i] = it->second;
    }
    i++;
  }
  //-- update IDs for the faces
  update_to_new_ids(j, newids);
  //-- replace the vertices
  std::vector<std::array<int, 3>> vout;
  for (std::string& s : newvertices) {
    std::vector<std::string> ls;
    tokenize(s, ls);
    for (auto& each : ls) {
      std::size_t found = each.find(".");
      each.erase(found, 1);
    }
    // std::cout << ls[0] << std::endl;
    std::array<int,3> t;
    t[0] = std::stoi(ls[0]);
    t[1] = std::stoi(ls[1]);
    t[2] = std::stoi(ls[2]);
    vout.push_back(t);
  }
  j["vertices"] = vout;
  double scalefactor = 1 / (pow(10, importantdigits));
  j["transform"]["scale"] = {scalefactor, scalefactor, scalefactor};
  j["transform"]["translate"] = {minx, miny, minz};
  return (inputsize - j["vertices"].size());
}


void tokenize(const std::string& str, std::vector<std::string>& tokens) {
  std::string::size_type lastPos = str.find_first_not_of(" ", 0);
  std::string::size_type pos     = str.find_first_of(" ", lastPos);
  while (std::string::npos != pos || std::string::npos != lastPos) {
    tokens.push_back(str.substr(lastPos, pos - lastPos));
    lastPos = str.find_first_not_of(" ", pos);
    pos = str.find_first_of(" ", lastPos);
  }
}

//-- orphans
int remove_orphan_vertices(json& j) {
  size_t inputsize = j["vertices"].size();
  std::map<int,int> oldnewids;
  std::vector<int> newvertices;
  for (auto& co : j["CityObjects"]) 
    for (auto& g : co["geometry"]) 
      if (g["type"] == "GeometryInstance") {
        int v = g["boundaries"][0];
        if (oldnewids.find(v) == oldnewids.end()) {
          oldnewids[v] = int(newvertices.size());
          newvertices.push_back(v);
        }
      }
      else if (g["type"] == "Solid") {
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
      if (g["type"] == "GeometryInstance") {
        g["boundaries"][0] = oldnewids[g["boundaries"][0]];
      }
      else if (g["type"] == "Solid") {
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
  std::vector<std::array<int, 3>> vout;
  for (int& v : newvertices) {
    std::array<int,3> t;
    t[0] = j["vertices"][v][0];
    t[1] = j["vertices"][v][1];
    t[2] = j["vertices"][v][2];
    vout.push_back(t);
  }
  j["vertices"] = vout;
  return (inputsize - j["vertices"].size());
}

