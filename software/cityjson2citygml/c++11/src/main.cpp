
#include <iostream>
#include <fstream>
#include "json.hpp"
#include <set>
#include <string>

using json = nlohmann::json;

json j;


void get_xml_header() {
  std::cout << "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n";
}

void get_citygml_namespaces() {
  //-- boiler template for all files
  std::cout << "<CityModel xmlns=\"http://www.opengis.net/citygml/2.0\"\n";
  std::cout << "xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\"\n";
  std::cout << "xmlns:xAL=\"urn:oasis:names:tc:ciq:xsdschema:xAL:2.0\"\n";
  std::cout << "xmlns:xlink=\"http://www.w3.org/1999/xlink\"\n";
  std::cout << "xmlns:gml=\"http://www.opengis.net/gml\"\n";
  //-- if appearances
  if (j.count("appearance") != 0)
    std::cout << "xmlns:app=\"http://www.opengis.net/citygml/appearance/2.0\"\n";
  //-- collect all CO types
  std::set<std::string> d;
  for (auto& co : j["CityObjects"]) {
    std::string tmp = co["type"];
    d.insert(tmp);
  }
  //-- if there then put the XSD module
  if (d.count("Building") != 0)
    std::cout << "xmlns:bldg=\"http://www.opengis.net/citygml/building/2.0\"\n";
  if (d.count("WaterBody") != 0)
    std::cout << "xmlns:wtr=\"http://www.opengis.net/citygml/waterbody/2.0\"\n";
  if ( (d.count("PlantCover") != 0) || (d.count("SolitaryVegetationObject") != 0) )
    std::cout << "xmlns:veg=\"http://www.opengis.net/citygml/vegetation/2.0\"\n";
  if (d.count("TINRelief") != 0)
    std::cout << "xmlns:dem=\"http://www.opengis.net/citygml/relief/2.0\"\n";
  if ( (d.count("Road") != 0) || (d.count("Railway") != 0) || (d.count("TransportSquare") != 0) )
    std::cout << "xmlns:tran=\"http://www.opengis.net/citygml/transportation/2.0\"\n";
  if (d.count("LandUse") != 0)
    std::cout << "xmlns:luse=\"http://www.opengis.net/citygml/landuse/2.0\"\n";
  if (d.count("GenericCityObject") != 0)
    std::cout << "xmlns:gen=\"http://www.opengis.net/citygml/generics/2.0\"\n";
  if (d.count("Bridge") != 0)
    std::cout << "xmlns:brg=\"http://www.opengis.net/citygml/bridge/2.0\"\n";
  if (d.count("Tunnel") != 0)
    std::cout << "xmlns:tun=\"http://www.opengis.net/citygml/tunnel/2.0\"\n";
  if (d.count("CityFurniture") != 0)
    std::cout << "xmlns:cif=\"http://www.opengis.net/citygml/cityfurniture/2.0\"\n";
  std::cout << "xsi:schemaLocation=\"http://www.opengis.net/citygml/2.0 ./CityGML_2.0/CityGML.xsd\">\n";
}


void get_citygml_metadata() {
  if (j.count("metadata") != 0) {
    json jm = j["metadata"];
    if (jm.count("bbox") != 0) {
      std::cout << "<gml:boundedBy>" << std::endl;
      std::cout << "<gml:Envelope srsDimension=\"3\">" << std::endl;
      std::cout << "<gml:lowerCorner>" << std::endl;
      std::cout << jm["bbox"][0] << " " << jm["bbox"][1] << " " << jm["bbox"][2] << std::endl;
      std::cout << "</gml:lowerCorner>" << std::endl;
      std::cout << "<gml:upperCorner>" << std::endl;
      std::cout << jm["bbox"][3] << " " << jm["bbox"][4] << " " << jm["bbox"][5] << std::endl;
      std::cout << "</gml:upperCorner>" << std::endl;
      std::cout << "</gml:Envelope>" << std::endl;
      std::cout << "</gml:boundedBy>" << std::endl;
    }
    if (jm.count("datasetTitle") != 0) {
      std::cout << "<gml:name>" << jm["datasetTitle"].get<std::string>() << "</gml:name>" << std::endl;
    }
  }
}


int main(int argc, const char * argv[]) {
  const char* filename = argv[1];
  std::ifstream input(filename);
  input >> j;

  std::cout << std::setprecision(3) << std::fixed;
  get_xml_header();
  get_citygml_namespaces();
  std::cout << "<!-- Automatically converted from CityJSON (http://www.cityjson.org) -->\n";
  get_citygml_metadata();
  std::cout << "</CityModel>" << std::endl;


  // //-- CityJSON version
  // std::cout << "CityJSON version: " << j["version"] << std::endl;
  
  // //-- CityObjects
  // std::cout << "===== CityObjects =====" << std::endl;
  // std::cout << "Total : " << j["CityObjects"].size() << std::endl;
  // std::set<std::string> d;
  // for (auto& co : j["CityObjects"]) {
  //   std::string tmp = co["type"];
  //   d.insert(tmp);
  // }
  // std::cout << "Types:" << std::endl;
  // for (auto& each : d)
  //   std::cout << "  " << each << std::endl; 
  // d.clear();
  // for (auto& co : j["CityObjects"]) {
  //   for (auto& g : co["geometry"]) {
  //     std::string tmp = g["type"];
  //     d.insert(tmp);
  //   }
  // }
  // std::cout << "Geometries present:" << std::endl;
  // for (auto& each : d)
  //   std::cout << "  " << each << std::endl; 
  // d.clear();

  // //-- metadata
  // std::cout << "===== Metadata =====" << std::endl;
  // if (j.count("metadata") == 0)
  //   std::cout << "  none" << std::endl;
  // else {
  //   for (json::iterator it = j["metadata"].begin(); it != j["metadata"].end(); ++it) {
  //     if (it.key() == "crs")
  //       std::cout << "  crs: EPSG:" << j["metadata"]["crs"]["epsg"] << std::endl;
  //     else  
  //       std::cout << "  " << it.key() << std::endl;
  //   }
  // }

  // //-- vertices
  // std::cout << "===== Vertices =====" << std::endl;
  // std::cout << "Total: " << j["vertices"].size() << std::endl;

  // //-- appearance
  // std::cout << "===== Appearance =====" << std::endl;
  // if (j.count("appearance") == 0)
  //   std::cout << "  none" << std::endl;
  // else {
  //   if (j["appearance"].count("textures") > 0)
  //     std::cout << "  textures: " << j["appearance"]["textures"].size() << std::endl;
  //   if (j["appearance"].count("materials") > 0)
  //     std::cout << "  materials: " << j["appearance"]["materials"].size() << std::endl;    
  // }
    
    
// write prettified JSON to another file
//    std::ofstream o("/Users/hugo/projects/cityjson/pretty.json");
//    o << std::setw(4) << j << std::endl;
    
    return 0;
}
