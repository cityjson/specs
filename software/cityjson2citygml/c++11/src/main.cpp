
#include <iostream>
#include <fstream>
#include "json.hpp"
#include <set>
#include <string>

using json = nlohmann::json;

json j;
json jschema;


void xml_header() {
  std::cout << "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n";
}

void citygml_header() {
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


void metadata() {
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


void surface(std::vector<std::vector<int>> &onesurface) {
  std::cout << "<gml:surfaceMember>" << std::endl;
  std::cout << "<gml:Polygon>" << std::endl;
  std::cout << "<gml:exterior>" << std::endl;
  std::cout << "<gml:LinearRing>" << std::endl;
  for (auto& v: onesurface[0]) {
    std::cout << "<gml:pos>";
    std::cout << j["vertices"][v][0] << " ";
    std::cout << j["vertices"][v][1] << " ";
    std::cout << j["vertices"][v][2];
    std::cout << "</gml:pos>" << std::endl;
  }
  //-- repeat first vertex
  auto firstv = onesurface[0][0];
  std::cout << "<gml:pos>";
  std::cout << j["vertices"][firstv][0] << " ";
  std::cout << j["vertices"][firstv][1] << " ";
  std::cout << j["vertices"][firstv][2];
  std::cout << "</gml:pos>" << std::endl;
  std::cout << "</gml:LinearRing>" << std::endl;
  std::cout << "</gml:exterior>" << std::endl;
  if (onesurface.size() > 1) {
    for (int i = 1; i < onesurface.size(); i++) {
      std::cout << "<gml:interior>" << std::endl;
      std::cout << "<gml:LinearRing>" << std::endl;
      for (auto& v: onesurface[i]) {
        std::cout << "<gml:pos>";
        std::cout << j["vertices"][v][0] << " ";
        std::cout << j["vertices"][v][1] << " ";
        std::cout << j["vertices"][v][2];
        std::cout << "</gml:pos>" << std::endl;
      }
      //-- repeat first vertex
      auto firstv = onesurface[i][0];
      std::cout << "<gml:pos>";
      std::cout << j["vertices"][firstv][0] << " ";
      std::cout << j["vertices"][firstv][1] << " ";
      std::cout << j["vertices"][firstv][2];
      std::cout << "</gml:pos>" << std::endl;
      std::cout << "</gml:LinearRing>" << std::endl;
      std::cout << "</gml:interior>" << std::endl;
    }
  }
  std::cout << "</gml:Polygon>" << std::endl;
  std::cout << "</gml:surfaceMember>" << std::endl;
}


void shell(json& jsh) {
  std::cout << "<gml:CompositeSurface>" << std::endl;
  for (auto& polygon : jsh) { 
    std::vector<std::vector<int>> t = polygon;
    surface(t);
  }
  std::cout << "</gml:CompositeSurface>" << std::endl;
}


void solid(json& js) {
  std::cout << "<gml:Solid>" << std::endl;
  int numshell = 0;
  for (auto& sh : js["boundaries"]) {
    if (numshell == 0) 
      std::cout << "<gml:exterior>" << std::endl;
    else
      std::cout << "<gml:interior>" << std::endl;
    shell(sh);
    if (numshell == 0) 
      std::cout << "</gml:exterior>" << std::endl;
    else
      std::cout << "</gml:interior>" << std::endl;
    numshell++;
  }
  std::cout << "</gml:Solid>" << std::endl;
}


void solid_w_semantics(json& js, json& jsem) {
  //-- 1. sem surfaces first
  int i = 0, j = 0;
  for (auto& sh : js["boundaries"]) {
    j = 0;
    for (auto& polygon : sh) { 
      std::cout << "===== " << jsem["values"][i][j] << std::endl;
      if (jsem["values"][i][j].is_null() == false) {
        int sem = jsem["values"][i][j];
        std::cout << jsem["surfaces"][sem] << std::endl;
      }
      else
        std::cout << "null" << std::endl;
      j++;
    }
    i++;
  }
}



void multisurface(json& js) {
  std::cout << "<gml:MultiSurface>" << std::endl;
  for (auto& polygon : js["boundaries"]) {
    std::vector<std::vector<int>> t = polygon;
    surface(t);
  }
  std::cout << "</gml:MultiSurface>" << std::endl;
}


void building(std::string id, json& jb) {
  std::cout << "<cityObjectMember>" << std::endl;
  std::cout << "<bldg:Building gml:id=\"" << id << "\">" << std::endl;
  //-- 1. attributes
  json tmp;
  std::string cotype = jb["type"];
  tmp = jschema["definitions"][cotype]["properties"]["attributes"]["properties"];
  for (json::iterator it = jb["attributes"].begin(); it != jb["attributes"].end(); ++it) {
    if (tmp.find(it.key()) != tmp.end()) {
      std::cout << "<" << it.key() << ">";
      std::cout << it.value();
      std::cout << "</" << it.key() << ">" << std::endl;
    }
    else {
      // std::cout << "ATTRIBUTE GENERICS" << std::endl;
      std::cout << "<gen:stringAttribute name=\"" << it.key() << "\">" << std::endl;
      std::cout << "<gen:value>" << it.value() << "</gen:value>" << std::endl;
      std::cout << "</gen:stringAttribute>" << std::endl;
    }
  }
  // 2. each geoms
  for (auto& g : jb["geometry"]) {
    int lod = g["lod"].get<int>();
    std::cout << "<bldg:lod" << lod << g["type"].get<std::string>() << ">" << std::endl;
    if (g.count("semantics") != 0) {
      if (g["type"] == "Solid")
        solid_w_semantics(g, g["semantics"]);
    }
    else {
      if (g["type"] == "Solid")
        solid(g);
      if (g["type"] == "MultiSurface")
        multisurface(g);
    }
    std::cout << "</bldg:lod" << lod << g["type"].get<std::string>() << ">" << std::endl;
  }


  // 3. BuildingParts?
  // 4. BuildingInstallations?



  std::cout << "</bldg:Building>" << std::endl;
  std::cout << "</cityObjectMember>" << std::endl;
}


int main(int argc, const char * argv[]) {
  //-- the input file
  const char* filename = argv[1];
  std::ifstream input(filename);
  input >> j;
  //-- the schema
  std::string v = j["version"];
  if (v.find("http://www.cityjson.org/version/") != std::string::npos)
      v = v.substr(v.size() - 3);
  std::string nameschema = "../../../../schema/cityjson-v" + v.substr(0, 1) + v.substr(2) + ".schema.json";
  std::ifstream inputs(nameschema);
  // std::cout << "Schema: " << nameschema << std::endl << std::endl;
  if (inputs.is_open() == true) {
    inputs >> jschema;
  }
  else {
    std::cout << "ERROR:   cannot find schema v" << v << std::endl << std::endl;  
    return 0;
  }


  std::cout << std::setprecision(3) << std::fixed;
  xml_header();
  std::cout << "<!-- Automatically converted from CityJSON (http://www.cityjson.org) -->\n";
  citygml_header();
  metadata();
  
  for (json::iterator coit = j["CityObjects"].begin(); coit != j["CityObjects"].end(); ++coit) {
    if (coit.value()["type"] == "Building") {
      // std::cout << "BUILDING" << std::endl;
      building(coit.key(), coit.value());
    }
  }

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
