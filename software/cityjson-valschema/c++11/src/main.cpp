/*
      _ _          _                              
     (_) |        (_)                             
  ___ _| |_ _   _  _ ___  ___  _ __               
 / __| | __| | | || / __|/ _ \| '_ \              
| (__| | |_| |_| || \__ \ (_) | | | |             
 \___|_|\__|\__, || |___/\___/|_| |_|             
             __/ |/ |                             
            |___/__/                              
           | |        | |                         
__   ____ _| |___  ___| |__   ___ _ __ ___   __ _ 
\ \ / / _` | / __|/ __| '_ \ / _ \ '_ ` _ \ / _` |
 \ V / (_| | \__ \ (__| | | |  __/ | | | | | (_| |
  \_/ \__,_|_|___/\___|_| |_|\___|_| |_| |_|\__,_|
                                                  
  cityjson-valschema
  Created by Hugo Ledoux on 2017-08-30
  Copyright © 2017 Hugo Ledoux. All rights reserved.

*/

#include <json-schema.hpp>
#include "json/json.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <set>

using nlohmann::json;
using nlohmann::json_uri;
using nlohmann::json_schema_draft4::json_validator;


static void loader(const json_uri &uri, json &schema);
bool json_schema(json& j, json& jschema, std::stringstream& ss);
bool duplicate_keys(const char* nameinput);
bool citygml_attributes(json& j, json& jschema);
bool metadata(json& j, json& jschema);
bool geometry_empty(json& j, json& jschema);
bool semantics(json& j);
bool building_parts(json& j);
bool building_installations(json& j);
bool building_pi_parent(json& j);


int main(int argc, char *argv[]) {
  bool isValid = true;
  bool woWarnings = true;
  json j;
  const char* nameinput = (argc > 1) ? argv[1] : "../../../../example-datasets/dummy-values/example2.json";
  std::ifstream input2(nameinput);
  input2 >> j;
  std::cout << "Input file: " << nameinput << std::endl;
  //-- make sure it's a CityJSON file
  if (!( (j["type"] == "CityJSON") || (j["type"] == "CityModel") ) ) {
    std::cout << "ERROR:   not a CityJSON file" << std::endl;  
    isValid = false;
  }
  //-- look for version and fetch the schema file
  json jschema;
  if (argc > 2) {
    const char* nameschema = argv[2];
    std::cout << "Schema: " << nameschema << std::endl << std::endl;
    std::ifstream input(nameschema);
    input >> jschema;
  }
  else {
    std::string v = j["version"];
    if (v.find("http://www.cityjson.org/version/") != std::string::npos)
        v = v.substr(v.size() - 3);
    std::string nameschema = "../../../../schema/cityjson-v" + v.substr(0, 1) + v.substr(2) + ".schema.json";
    std::ifstream input(nameschema);
    std::cout << "Schema: " << nameschema << std::endl << std::endl;
    if (input.is_open() == true) {
      input >> jschema;
    }
    else {
      std::cout << "ERROR:   cannot find schema v" << v << std::endl << std::endl;  
      // std::cout << "File is VALID";
      return 0;
    }
  }

  std::stringstream ss;
  //-- validate the schema
  if ( (isValid == true) && (json_schema(j, jschema, ss) == false) ){
    std::cout << ss.str() << std::endl;
    isValid = false;
  }
  //-- check for duplicate keys, which is impossible with schema and/or my parsing library
  if ( (isValid == true) && (duplicate_keys(nameinput) == false) ) {
    std::cout << "ERROR:   Duplicate IDs (keys) for the City Objects" << std::endl;  
    isValid = false;
  }
  if (isValid == true) {
    if (building_parts(j) == false)
      isValid = false;
    if (building_installations(j) == false)
      isValid = false;
    if (building_pi_parent(j) == false)
      isValid = false;
    if (semantics(j) == false)
      isValid = false;
    if (geometry_empty(j, jschema) == false)
      woWarnings = false;
    if (metadata(j, jschema) == false)
      woWarnings = false;
    if (citygml_attributes(j, jschema) == false)
      woWarnings = false;
  }

  std::cout << std::endl;
  if (isValid == true) {
    std::cout << "File is VALID";
    if (woWarnings == false)
      std::cout << ", but has warnings." << std::endl;
    else
      std::cout << "." << std::endl;
  }
  else
    std::cout << "File is INVALID";
    std::cout << "" << std::endl;

  return 1;
}


bool duplicate_keys(const char* nameinput) {
  Json::CharReaderBuilder builder;
  builder["rejectDupKeys"] = true;
  JSONCPP_STRING errs;
  Json::Value j;   // 'root' will contain the root value after parsing.
  // const char* nameinput = "/Users/hugo/projects/cityjson-new/example-datasets/dummy-values/example3.json";
  std::ifstream input(nameinput);
  bool ok = parseFromStream(builder, input, &j, &errs);
  return ok;
}


bool building_pi_parent(json& j) {
  bool isValid = true;
  std::set<std::string> pis;
  for (json::iterator coit = j["CityObjects"].begin(); coit != j["CityObjects"].end(); ++coit) 
    if ( (coit.value()["type"] == "BuildingPart") || (coit.value()["type"] == "BuildingInstallation") ) 
      pis.insert(coit.key());
  for (json::iterator coit = j["CityObjects"].begin(); coit != j["CityObjects"].end(); ++coit) {
    if (coit.value()["type"] == "Building") {
      for (std::string pid : coit.value()["Parts"]) {
        if (pis.count(pid) > 0)
          pis.erase(pid);
      }
      for (std::string pid : coit.value()["Installations"]) {
        if (pis.count(pid) > 0)
          pis.erase(pid);
      }
    }
  }
  if (pis.size() > 0) {
    isValid = false;
    std::cout << "ERROR:   BuildingParts and BuildingInstallations have no parent Building:" << std::endl;
    for (std::string s : pis)
      std::cout << "\t#" << s << std::endl;
  }
  return isValid;
}

bool building_parts(json& j) {
  bool isValid = true;
  for (json::iterator coit = j["CityObjects"].begin(); coit != j["CityObjects"].end(); ++coit) {
    if ( (coit.value()["type"] == "Building") && (coit.value().count("Parts") > 0) ){
      for (std::string pid : coit.value()["Parts"]) {
        if (j["CityObjects"].count(pid) == 0) {
          isValid = false;
          std::cout << "ERROR:   BuildingPart #" << pid << " is not present" << std::endl;
          std::cout << "\t(Building #" << coit.key() << " references it)" << std::endl;
        }
      }
    }
  }
  return isValid;
}


bool building_installations(json& j) {
  bool isValid = true;
  for (json::iterator coit = j["CityObjects"].begin(); coit != j["CityObjects"].end(); ++coit) {
    if ( (coit.value()["type"] == "Building") && (coit.value().count("Installations") > 0) ){
      for (std::string pid : coit.value()["Installations"]) {
        if (j["CityObjects"].count(pid) == 0) {
          isValid = false;
          std::cout << "ERROR:   BuildingInstallation #" << pid << " is not present" << std::endl;
          std::cout << "\t(Building #" << coit.key() << " references it)" << std::endl;
        }
      }
    }
  }
  return isValid;
}


bool citygml_attributes(json& j, json& jschema) {
  bool isValid = true;
  json tmp;
  std::map<std::string,std::vector<std::string>> thewarnings;
  for (json::iterator coit = j["CityObjects"].begin(); coit != j["CityObjects"].end(); ++coit) {
    std::string cotype = coit.value()["type"];
    tmp = jschema["definitions"][cotype]["properties"]["attributes"]["properties"];
    for (json::iterator it = coit.value()["attributes"].begin(); it != coit.value()["attributes"].end(); ++it) {
      if (tmp.find(it.key()) == tmp.end()) {
        isValid = false;
        std::stringstream ss;
        ss << "WARNING: attributes '" << it.key() << "' not in CityGML schema";
        if (thewarnings.find(ss.str()) == thewarnings.end()) {
          thewarnings[ss.str()] = {coit.key()};
        }
        else
          thewarnings[ss.str()].push_back(coit.key());
      }
    }
    tmp = jschema["definitions"][cotype]["properties"]["address"]["properties"];
    for (json::iterator it = coit.value()["address"].begin(); it != coit.value()["address"].end(); ++it) {
      if (tmp.find(it.key()) == tmp.end()) {
        isValid = false;
        std::stringstream ss;
        ss << "WARNING: address attributes '" << it.key() << "' not in CityGML schema"; 
        if (thewarnings.find(ss.str()) == thewarnings.end())
          thewarnings[ss.str()] = {coit.key()};
        else
          thewarnings[ss.str()].push_back(coit.key());
      }
    }
  }
  for (auto& each : thewarnings) {
    std::cout << each.first;
    if (each.second.size() <= 3) {
      std::cout << " (";
      for (auto& i : each.second) 
        std::cout << " #" << i << " ";
      std::cout << ")" << std::endl;
    }
    else {
      std::cout << " (" << each.second.size() << " CityObjects have this warning)" << std::endl; 
    }
  }
  return isValid;
}


bool json_schema(json& j, json& jschema, std::stringstream& ss)
{
  //-- 1. Schema validation with http://json-schema.org
  json_validator validator(loader, [](const std::string &, const std::string &) {});
  try {
    validator.set_root_schema(jschema);
  } 
  catch (std::exception& e) {
    ss << "ERROR:   CityJSON schema invalid" << std::endl;
    ss << e.what() << std::endl;
    return false;
  }
  try {
    validator.validate(j);
  } 
  catch (std::exception& e) {
    ss << "ERROR:   Schema validation failed" << std::endl;
    ss << e.what() << " at offset: " << std::cin.tellg() << std::endl;
    return false;
  }
  return true;
}

bool metadata(json& j, json& jschema) {
  bool isValid = true;
  json tmp = jschema["properties"]["metadata"]["properties"];
  for (json::iterator it = j["metadata"].begin(); it != j["metadata"].end(); ++it) {
    if (tmp.find(it.key()) == tmp.end()) {
      isValid = false;
      std::cout << "WARNING: Metadata '" << it.key() << "' not CityJSON schema." << std::endl;
    }
  }
  return isValid;
}

bool geometry_empty(json& j, json& jschema) {
  bool isValid = true;
  for (json::iterator coit = j["CityObjects"].begin(); coit != j["CityObjects"].end(); ++coit) {
    if (coit.value()["geometry"].size() == 0) {
      isValid = false;
      std::string cotype = coit.value()["type"];
      std::cout << "WARNING: " << cotype << " #" << coit.key() << " has no geometry." << std::endl;
    }
  }
  return isValid;
}


bool semantics(json& j) {
  bool isValid = true;
  for (json::iterator coit = j["CityObjects"].begin(); coit != j["CityObjects"].end(); ++coit) {
    for (auto& g : coit.value()["geometry"]) {
      if (g.count("semantics") == 0) 
        continue;
      int i = 0, j = 0, k = 0;
      int is = 0, js = 0, ks = 0;
      if (g["type"] == "Solid") {
        for (auto& shell : g["boundaries"]) {
          j++;
          i += shell.size();
        }
        for (auto& shell : g["semantics"]) {
          js++;
          is += shell.size();
        }
        if ( (i != is) || (j != js) ) {
          isValid = false;
          std::string cotype = coit.value()["type"];
          std::cout << "ERROR:   " << cotype << " #" << coit.key() << " has arrays with different structure for 'boundaries' and 'semantics'" << std::endl;
        }
      }
      else if ( (g["type"] == "MultiSurface") || (g["type"] == "CompositeSurface") ) {
        if (g["boundaries"].size() != g["semantics"].size()) {
          isValid = false;
          std::string cotype = coit.value()["type"];
          std::cout << "ERROR:   " << cotype << " #" << coit.key() << " has arrays with different structure for 'boundaries' and 'semantics'" << std::endl;  
        }
      }
      else if ( (g["type"] == "MultiSolid") || (g["type"] == "CompositeSolid") ) {
        for (auto& solid : g["boundaries"]) {
          k++;
          for (auto& shell : solid) {
            j++;
            i += shell.size();
          }
        }
        for (auto& solid : g["semantics"]) {
          ks++;
          for (auto& shell : solid) {
            js++;
            is += shell.size();
          }
        }        
        if ( (i != is) || (j != js) || (k != ks) ) {
          isValid = false;
          std::string cotype = coit.value()["type"];
          std::cout << "ERROR:   " << cotype << " #" << coit.key() << " has arrays with different structure for 'boundaries' and 'semantics'." << std::endl;
        }
      }                  
    }
  }
  return isValid;
}


static void loader(const json_uri &uri, json &schema) {
  std::fstream lf("." + uri.path());
  if (!lf.good())
    throw std::invalid_argument("could not open " + uri.url() + " tried with " + uri.path());
  try {
    lf >> schema;
  } 
  catch (std::exception &e) {
    throw e;
  }
}