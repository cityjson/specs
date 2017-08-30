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
#include <fstream>
#include <iostream>
#include <sstream>
#include <set>

using nlohmann::json;
using nlohmann::json_uri;
using nlohmann::json_schema_draft4::json_validator;


static void loader(const json_uri &uri, json &schema);
bool is_valid_json_schema(json& j, json& jschema, std::stringstream& ss);
bool is_valid_citygml_attributes(json& j, json& jschema);
bool is_valid_metadata(json& j, json& jschema);
bool is_valid_semantics(json& j);
bool is_valid_building_parts(json& j);
bool is_valid_building_installations(json& j);
bool is_valid_building_pi_parent(json& j);


int main(int argc, char *argv[]) {
  const char* nameschema = (argc > 2) ? argv[2] : "../../../../schema/cityjson-v02.schema.json";
  std::ifstream input(nameschema);
  json jschema;
  input >> jschema;
  
  json j;
  const char* nameinput = (argc > 1) ? argv[1] : "/Users/hugo/temp/example2.json";
  std::ifstream input2(nameinput);
  input2 >> j;
  std::cout << "Input file: " << nameinput << std::endl;
  std::cout << "Schema: " << nameschema << std::endl << std::endl;
  std::stringstream ss;
  if (is_valid_json_schema(j, jschema, ss) == false) {
    std::cout << ss.str() << std::endl;
    return 1;
  }
  bool isValid = true;
  if (is_valid_building_parts(j) == false)
    isValid = false;
  if (is_valid_building_installations(j) == false)
    isValid = false;
  if (is_valid_building_pi_parent(j) == false)
    isValid = false;
  if (is_valid_semantics(j) == false)
    isValid = false;
  bool woWarnings = true;
  if (is_valid_metadata(j, jschema) == false)
    woWarnings = false;
  if (is_valid_citygml_attributes(j, jschema) == false)
    woWarnings = false;

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


bool is_valid_building_pi_parent(json& j) {
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

bool is_valid_building_parts(json& j) {
  bool isValid = true;
  for (json::iterator coit = j["CityObjects"].begin(); coit != j["CityObjects"].end(); ++coit) {
    if ( (coit.value()["type"] == "Building") && (coit.value().count("Parts") > 0) ){
      for (std::string pid : coit.value()["Parts"]) {
        if (j["CityObjects"].count(pid) == 0) {
          isValid = false;
          std::cout << "ERROR:   BuildingPart #" << pid << " is not present." << std::endl;
          std::cout << "\t(Building #" << coit.key() << " references it.)" << std::endl;
        }
      }
    }
  }
  return isValid;
}


bool is_valid_building_installations(json& j) {
  bool isValid = true;
  for (json::iterator coit = j["CityObjects"].begin(); coit != j["CityObjects"].end(); ++coit) {
    if ( (coit.value()["type"] == "Building") && (coit.value().count("Installations") > 0) ){
      for (std::string pid : coit.value()["Installations"]) {
        if (j["CityObjects"].count(pid) == 0) {
          isValid = false;
          std::cout << "ERROR:   BuildingInstallation #" << pid << " is not present." << std::endl;
          std::cout << "\t(Building #" << coit.key() << " references it.)" << std::endl;
        }
      }
    }
  }
  return isValid;
}


bool is_valid_citygml_attributes(json& j, json& jschema) {
  bool isValid = true;
  json tmp;
  for (json::iterator coit = j["CityObjects"].begin(); coit != j["CityObjects"].end(); ++coit) {
    std::string cotype = coit.value()["type"];
    tmp = jschema["definitions"][cotype]["properties"]["attributes"]["properties"];
    for (json::iterator it = coit.value()["attributes"].begin(); it != coit.value()["attributes"].end(); ++it) {
      if (tmp.find(it.key()) == tmp.end()) {
        isValid = false;
        std::cout << "WARNING: attributes '" << it.key() << "' not in CityGML schema (" << cotype << " #" << coit.key() << ")" << std::endl;
      }
    }
    tmp = jschema["definitions"][cotype]["properties"]["address"]["properties"];
    for (json::iterator it = coit.value()["address"].begin(); it != coit.value()["address"].end(); ++it) {
      if (tmp.find(it.key()) == tmp.end()) {
        isValid = false;
        std::cout << "WARNING: address attributes '" << it.key() << "' not in CityGML schema (" << cotype << " #" << coit.key() << ")" << std::endl;
      }
    }
  }
  return isValid;
}

bool is_valid_json_schema(json& j, json& jschema, std::stringstream& ss)
{
  //-- 1. Schema validation with http://json-schema.org
  json_validator validator(loader, [](const std::string &, const std::string &) {});
  try {
    validator.set_root_schema(jschema);
  } 
  catch (std::exception& e) {
    ss << "ERROR:   CityJSON schema invalid." << std::endl;
    ss << e.what() << std::endl;
    return false;
  }
  try {
    validator.validate(j);
  } 
  catch (std::exception& e) {
    ss << "ERROR:   Schema validation failed." << std::endl;
    ss << e.what() << " at offset: " << std::cin.tellg() << std::endl;
    return false;
  }
  return true;
}

bool is_valid_metadata(json& j, json& jschema) {
  bool isValid = true;
  json tmp = jschema["properties"]["metadata"]["properties"];
  for (json::iterator it = j["metadata"].begin(); it != j["metadata"].end(); ++it) {
    if (tmp.find(it.key()) == tmp.end()) {
      isValid = false;
      std::cout << "WARNING: Metadata '" << it.key() << "' not CityJSON schema." << std::endl;
    }
  }

  for (json::iterator coit = j["CityObjects"].begin(); coit != j["CityObjects"].end(); ++coit) {
    if (coit.value()["geometry"].size() == 0) {
      isValid = false;
      std::string cotype = coit.value()["type"];
      std::cout << "WARNING: " << cotype << " #" << coit.key() << " has no geometry." << std::endl;
    }
  }
  return isValid;
}


bool is_valid_semantics(json& j) {
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
          std::cout << "ERROR:   " << cotype << " #" << coit.key() << " has arrays with different structure for 'boundaries' and 'semantics'." << std::endl;
        }
      }
      else if ( (g["type"] == "MultiSurface") || (g["type"] == "CompositeSurface") ) {
        if (g["boundaries"].size() != g["semantics"].size()) {
          isValid = false;
          std::string cotype = coit.value()["type"];
          std::cout << "ERROR:   " << cotype << " #" << coit.key() << " has arrays with different structure for 'boundaries' and 'semantics'." << std::endl;  
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