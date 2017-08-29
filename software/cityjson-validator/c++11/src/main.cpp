
#include <json-schema.hpp>

#include <fstream>
#include <iostream>
#include <set>

using nlohmann::json;
using nlohmann::json_uri;
using nlohmann::json_schema_draft4::json_validator;

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

int main(int argc, char *argv[]) {
  const char* nameschema = (argc > 2) ? argv[2] : "../../../../schema/cityjson-v02.schema.json";
  std::ifstream input(nameschema);
  json jschema;
  input >> jschema;
  json_validator validator(loader, [](const std::string &, const std::string &) {});
  std::cout << "Schema: " << nameschema << std::endl;
  try {
    validator.set_root_schema(jschema);
  } 
  catch (std::exception &e) {
    std::cerr << "setting root schema failed\n";
    std::cerr << e.what() << "\n";
  }

  //-- 1. Schema validation with http://json-schema.org
  json j;
  const char* nameinput = (argc > 1) ? argv[1] : "/Users/hugo/temp/example2.json";
  std::ifstream input2(nameinput);
  input2 >> j;
  std::cout << "Input file: " << nameinput << std::endl;
  std::cout << "-----" << std::endl;
  try {
    validator.validate(j);
  } 
  catch (std::exception &e) {
    std::cerr << "schema validation failed\n";
    std::cerr << e.what() << " at offset: " << std::cin.tellg() << "\n";
    return EXIT_FAILURE;
  }

  std::cout << "---" << std::endl;
  json tmp;
  
  tmp = jschema["properties"]["metadata"]["properties"];
  for (json::iterator it = j["metadata"].begin(); it != j["metadata"].end(); ++it) {
    if (tmp.find(it.key()) == tmp.end()) 
      std::cout << "Metadata " << it.key() << " not CityJSON" << std::endl;
  }

  //-- 2. More than just with the schema
  tmp = jschema["definitions"]["Building"]["properties"]["attributes"]["properties"];
  // for (json::iterator it = tmp.begin(); it != tmp.end(); ++it) 
  //   possible.insert(it.key());
  if (tmp.find("hugo") != tmp.end()) 
    std::cout << "hugo present" << std::endl;
  if (tmp.find("usage") != tmp.end()) 
    std::cout << "usage present" << std::endl;

  for (auto& co : j["CityObjects"]) {
    if (co["type"] == "Building") {
      for (json::iterator it = co["attributes"].begin(); it != co["attributes"].end(); ++it) {
        if (tmp.find(it.key()) == tmp.end()) 
          std::cout << "Attributes " << it.key() << " not CityGML" << std::endl;
      }
      if (co.find("Parts") != co.end()) {
        for (auto& pid : co["Parts"]) {
          if (j["CityObjects"].count(pid) == 0)
          std::cout << pid << " doesnt exist." << std::endl;
        }
      }
    }
  }

  //-- 3. "Conformance requirements"
  std::cout << "Document is valid" << std::endl;
  return 1;
}
