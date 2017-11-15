
#include <iostream>
#include <fstream>
#include "json.hpp"
#include <set>
#include <string>

using json = nlohmann::json;



void get_xml_header(std::ostream& of) {
  of << "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n";
}

void get_citygml_namespaces(std::ostream& of) {
  of << "<CityModel xmlns=\"http://www.opengis.net/citygml/2.0\"\n";
  of << "xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\"\n";
  of << "xmlns:xAL=\"urn:oasis:names:tc:ciq:xsdschema:xAL:2.0\"\n";
  of << "xmlns:xlink=\"http://www.w3.org/1999/xlink\"\n";
  of << "xmlns:gml=\"http://www.opengis.net/gml\"\n";
  of << "xmlns:bldg=\"http://www.opengis.net/citygml/building/2.0\"\n";
  of << "xmlns:wtr=\"http://www.opengis.net/citygml/waterbody/2.0\"\n";
  of << "xmlns:veg=\"http://www.opengis.net/citygml/vegetation/2.0\"\n";
  of << "xmlns:dem=\"http://www.opengis.net/citygml/relief/2.0\"\n";
  of << "xmlns:tran=\"http://www.opengis.net/citygml/transportation/2.0\"\n";
  of << "xmlns:luse=\"http://www.opengis.net/citygml/landuse/2.0\"\n";
  of << "xmlns:gen=\"http://www.opengis.net/citygml/generics/2.0\"\n";
  of << "xmlns:brg=\"http://www.opengis.net/citygml/bridge/2.0\"\n";
  of << "xmlns:app=\"http://www.opengis.net/citygml/appearance/2.0\"\n";
  of << "xmlns:tun=\"http://www.opengis.net/citygml/tunnel/2.0\"\n";
  of << "xmlns:cif=\"http://www.opengis.net/citygml/cityfurniture/2.0\"\n";
  of << "xsi:schemaLocation=\"http://www.opengis.net/citygml/2.0 ./CityGML_2.0/CityGML.xsd\">\n";
}

void create_citygml_header(std::ostream& of) {
    of << std::setprecision(3) << std::fixed;
    get_xml_header(of);
    get_citygml_namespaces(of);
    of << "<!-- Automatically converted from CityJSON (http://www.cityjson.org) -->\n";
    of << "<gml:name>my 3dfied map</gml:name>\n";
    // of << "<gml:boundedBy>";
    // of << "<gml:Envelope srsDimension=\"3\" srsName=\"urn:ogc:def:crs:EPSG::7415\">";
    // of << "<gml:lowerCorner>";
    // of << bg::get<bg::min_corner, 0>(_bbox) << " " << bg::get<bg::min_corner, 1>(_bbox) << " 0";
    // of << "</gml:lowerCorner>";
    // of << "<gml:upperCorner>";
    // of << bg::get<bg::max_corner, 0>(_bbox) << " " << bg::get<bg::max_corner, 1>(_bbox) << " 100";
    // of << "</gml:upperCorner>";
    // of << "</gml:Envelope>";
    // of << "</gml:boundedBy>\n";
}

int main(int argc, const char * argv[]) {
  const char* filename = argv[1];
  std::ifstream input(filename);
  json j;
  input >> j;

  std::ofstream of("mycitygml.gml", std::ofstream::out);
  get_xml_header(of);
  get_citygml_namespaces(of);

  of.close();
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
