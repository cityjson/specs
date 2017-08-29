
#include <json-schema.hpp>

#include <fstream>
#include <iostream>

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
  const char* nameschema = (argc > 2) ? argv[2] : "../../../../schema/cityjson.schema.json";
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
  json document;
  const char* nameinput = (argc > 1) ? argv[1] : "/Users/hugo/temp/example2.json";
  std::ifstream input2(nameinput);
  input2 >> document;
  std::cout << "Input file: " << nameinput << std::endl;
  std::cout << "-----" << std::endl;
  try {
    validator.validate(document);
  } 
  catch (std::exception &e) {
    std::cerr << "schema validation failed\n";
    std::cerr << e.what() << " at offset: " << std::cin.tellg() << "\n";
    return EXIT_FAILURE;
  }

  //-- 2. More than just with the schema


  //-- 3. "Conformance requirements"
  std::cout << "Document is valid" << std::endl;
  return 1;
}
