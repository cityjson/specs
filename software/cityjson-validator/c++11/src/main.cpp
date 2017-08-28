
#include <json-schema.hpp>

#include <fstream>
#include <iostream>

using nlohmann::json;
using nlohmann::json_uri;
using nlohmann::json_schema_draft4::json_validator;



static void loader(const json_uri &uri, json &schema)
{
  std::fstream lf("." + uri.path());
  if (!lf.good())
    throw std::invalid_argument("could not open " + uri.url() + " tried with " + uri.path());

  try {
    lf >> schema;
  } catch (std::exception &e) {
    throw e;
  }
}

int main(int argc, char *argv[])
{

  const char* nameschema = argv[2];
  std::ifstream input(nameschema);
  json jschema;
  input >> jschema;


  // 2) create the validator and
  json_validator validator(loader, [](const std::string &, const std::string &) {});

  try {
    // insert this schema as the root to the validator
    // this resolves remote-schemas, sub-schemas and references via the given loader-function
    validator.set_root_schema(jschema);
  } catch (std::exception &e) {
    std::cerr << "setting root schema failed\n";
    std::cerr << e.what() << "\n";
  }

  std::cout << "-----here" << std::endl;
  
  // 3) do the actual validation of the document
  json document;
  const char* nameinput = argv[1];
  std::ifstream input2(nameinput);
  input2 >> document;

  try {
    // std::cin >> document;
    validator.validate(document);
  } catch (std::exception &e) {
    std::cerr << "schema validation failed\n";
    std::cerr << e.what() << " at offset: " << std::cin.tellg() << "\n";
    return EXIT_FAILURE;
  }

  std::cerr << "document is valid\n";

  return EXIT_SUCCESS;
}
