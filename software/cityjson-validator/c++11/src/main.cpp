
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
  std::cout << "Schema: " << nameschema << std::endl;
  json jschema;
  input >> jschema;
  
  json j;
  const char* nameinput = (argc > 1) ? argv[1] : "/Users/hugo/temp/example2.json";
  std::ifstream input2(nameinput);
  input2 >> j;
  std::stringstream ss;
  if (is_valid_json_schema(j, jschema, ss) == false) {
    std::cout << ss.str() << std::endl;
    return 1;
  }


  //-- 2. More than just with the schema
  std::cout << "---" << std::endl;
  json tmp;
  
  tmp = jschema["properties"]["metadata"]["properties"];
  for (json::iterator it = j["metadata"].begin(); it != j["metadata"].end(); ++it) {
    if (tmp.find(it.key()) == tmp.end()) 
      std::cout << "Metadata " << it.key() << " not CityJSON" << std::endl;
  }

  // tmp = jschema["definitions"]["Building"]["properties"]["attributes"]["properties"];
  for (json::iterator coit = j["CityObjects"].begin(); coit != j["CityObjects"].end(); ++coit) {
    std::string cotype = coit.value()["type"];
    tmp = jschema["definitions"][cotype]["properties"]["attributes"]["properties"];
    for (json::iterator it = coit.value()["attributes"].begin(); it != coit.value()["attributes"].end(); ++it) {
      if (tmp.find(it.key()) == tmp.end()) {
        std::cout << "Attributes '" << it.key() << "' not CityGML compliant." << std::endl;
        std::cout << "\tCO: " << cotype << std::endl;
      }
    }
    if (coit.value().count("Parts") > 0) {
      for (auto& pid : coit.value()["Parts"]) {
        if (j["CityObjects"].count(pid) == 0)
        std::cout << pid << " doesnt exist." << std::endl;
      }
    }
  }

  //-- 3. "Conformance requirements"
  std::cout << "Document is valid" << std::endl;
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