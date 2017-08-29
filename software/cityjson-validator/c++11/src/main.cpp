
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
  std::cout << "File valid according to the schema." << std::endl;

  //-- 2. Checking mandatory CityGML attributes
  std::cout << "---Mandatory attributes" << std::endl;
  json tmp;
  for (json::iterator coit = j["CityObjects"].begin(); coit != j["CityObjects"].end(); ++coit) {
    std::string cotype = coit.value()["type"];
    tmp = jschema["definitions"][cotype]["properties"]["attributes"]["properties"];
    for (json::iterator it = coit.value()["attributes"].begin(); it != coit.value()["attributes"].end(); ++it) {
      if (tmp.find(it.key()) == tmp.end()) {
        std::cout << "WARNING: attributes '" << it.key() << "' not in CityGML schema (" << cotype << " #" << coit.key() << ")" << std::endl;
      }
    }
    tmp = jschema["definitions"][cotype]["properties"]["address"]["properties"];
    for (json::iterator it = coit.value()["address"].begin(); it != coit.value()["address"].end(); ++it) {
      if (tmp.find(it.key()) == tmp.end()) {
        std::cout << "WARNING: address attributes '" << it.key() << "' not in CityGML schema (" << cotype << " #" << coit.key() << ")" << std::endl;
      }
    }
  }

  //-- BuildingParts
  for (json::iterator coit = j["CityObjects"].begin(); coit != j["CityObjects"].end(); ++coit) {
    if ( (coit.value()["type"] == "Building") && (coit.value().count("Parts") > 0) ){
      for (std::string pid : coit.value()["Parts"]) {
        if (j["CityObjects"].count(pid) == 0) {
          std::cout << "ERROR: BuildingPart #" << pid << " is not present." << std::endl;
          std::cout << "\t(Building #" << coit.key() << " references it.)" << std::endl;
        }
      }
    }
  }

  //-- BuildingInstallation
  for (json::iterator coit = j["CityObjects"].begin(); coit != j["CityObjects"].end(); ++coit) {
    if ( (coit.value()["type"] == "Building") && (coit.value().count("Installations") > 0) ){
      for (std::string pid : coit.value()["Installations"]) {
        if (j["CityObjects"].count(pid) == 0) {
          std::cout << "ERROR: BuildingInstallation #" << pid << " is not present." << std::endl;
          std::cout << "\t(Building #" << coit.key() << " references it.)" << std::endl;
        }
      }
    }
  }

  //-- Building Parts and Installations without parents
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
    std::cout << "ERROR: BuildingParts and BuildingInstallations have no parent Building:" << std::endl;
    for (std::string s : pis)
      std::cout << "\t#" << s << std::endl;
  }

  //-- metadata
  tmp = jschema["properties"]["metadata"]["properties"];
  for (json::iterator it = j["metadata"].begin(); it != j["metadata"].end(); ++it) {
    if (tmp.find(it.key()) == tmp.end()) 
      std::cout << "WARNING: Metadata '" << it.key() << "' not CityJSON schema." << std::endl;
  }

  //-- geometry/semantics arrays
  // for (json::iterator it = j["CityObjects"].begin(); it != j["CityObjects"].end(); ++it) {
  //   for (auto& g : it.value()["geometry"]) {
  //     std::cout << g["type"] << std::endl;
  //     std::vector<int> c;
  //     if (g["type"] == "Solid") {
  //       for (auto& shell : g["boundaries"]) {
  //         for (auto& surface : shell) { 
  //           // std::cout << surface << std::endl;
  //           std::vector< std::vector<int> > t = surface;
  //           c.push_back()
  //         }
  //       }
  //       // for (auto& shell : g["semantics"]) {
  //       //   for (auto& surface : shell) { 
  //       //     // std::cout << surface << std::endl;
  //       //     std::vector< std::vector<int> > t = surface;
            
  //       //   }
  //       // }
  //     }
  //     // else if ( (g["type"] == "MultiSurface") || (g["type"] == "CompositeSurface") ) {
  //     //   for (auto& surface : g["boundaries"]) { 
  //     //     std::vector< std::vector<int> > t = surface;
  //     //     write_surface(output, t);
  //     //   }
  //     // }
  //   }
  // }


  return 1;
}

bool is_valid_json_schema(json& j, json& jschema, std::stringstream& ss)
{
  //-- 1. Schema validation with http://json-schema.org
  json_validator validator(loader, [](const std::string &, const std::string &) {});
  try {
    validator.set_root_schema(jschema);
  } 
  catch (std::exception& e) {
    ss << "ERROR: CityJSON schema invalid." << std::endl;
    ss << e.what() << std::endl;
    return false;
  }
  try {
    validator.validate(j);
  } 
  catch (std::exception& e) {
    ss << "ERROR: Schema validation failed." << std::endl;
    ss << e.what() << " at offset: " << std::cin.tellg() << std::endl;
    return false;
  }
  return true;
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