

#include <fstream>
#include <iostream>
#include <sstream>
#include <set>
#include <map>

#include "pugixml/pugixml.hpp"
#include "nlohmann-json/json.hpp"


void read_file_gml(std::string &ifile);
void get_namespaces(pugi::xml_node& root, std::string& vcitygml);
void build_dico_xlinks(pugi::xml_document& doc, std::map<std::string, pugi::xpath_node>& dallpoly);

///////////////

//-- XML namespaces map
std::map<std::string, std::string> NS; 


int main(int argc, char *argv[]) {
  const char* tmp = (argc > 1) ? argv[1] : "../data/b1_lod2_cs_w_sem.gml";
  
  std::string nameinput = tmp;
  std::cout << "Input file: " << nameinput << std::endl;

  read_file_gml(nameinput);

  return 1;
}


void read_file_gml(std::string &ifile) {
  std::cout << "Reading file: " << ifile << std::endl;
  pugi::xml_document doc;
  if (!doc.load_file(ifile.c_str())) 
  {
    std::cout << "Input file not found." << std::endl;
    return;
  }
  //-- parse namespace
  pugi::xml_node ncm = doc.first_child();
  std::string vcitygml;
  get_namespaces(ncm, vcitygml); //-- results in global variable NS in this unit
  if (NS.count("gml") == 0)
  {
    std::cout << "Input file does not have the GML namespace." << std::endl;
    return;
  }
  //-- build dico of xlinks for <gml:Polygon>
  std::map<std::string, pugi::xpath_node> dallpoly;
  build_dico_xlinks(doc, dallpoly);
  std::cout << "CityGML input file" << std::endl;
  // process_gml_file_city_objects(doc, dPrimitives, dallpoly, errs, tol_snap);
}

void get_namespaces(pugi::xml_node& root, std::string& vcitygml) {
  vcitygml = "";
  for (pugi::xml_attribute attr = root.first_attribute(); attr; attr = attr.next_attribute()) {
    std::string name = attr.name();
    if (name.find("xmlns") != std::string::npos) {
      // std::cout << attr.name() << "=" << attr.value() << std::endl;
      std::string value = attr.value();
      std::string sns;
      if (value.find("http://www.opengis.net/citygml/0") != std::string::npos) {
        sns = "citygml";
        vcitygml = "v0.4";
      }
      else if (value.find("http://www.opengis.net/citygml/1") != std::string::npos) {
        sns = "citygml";
        vcitygml = "v1.0";
      }
      else if (value.find("http://www.opengis.net/citygml/2") != std::string::npos) {
        sns = "citygml";
        vcitygml = "v2.0";
      }
      else if (value.find("http://www.opengis.net/gml") != std::string::npos)
        sns = "gml";
      else if (value.find("http://www.opengis.net/citygml/building") != std::string::npos)
        sns = "building";
      else if (value.find("http://www.opengis.net/citygml/relief") != std::string::npos)
        sns = "dem";
      else if (value.find("http://www.opengis.net/citygml/vegetation") != std::string::npos)
        sns = "veg";
      else if (value.find("http://www.opengis.net/citygml/waterbody") != std::string::npos)
        sns = "wtr";
      else if (value.find("http://www.opengis.net/citygml/landuse") != std::string::npos)
        sns = "luse";
      else if (value.find("http://www.opengis.net/citygml/transportation") != std::string::npos)
        sns = "tran";      
      else if (value.find("http://www.opengis.net/citygml/cityfurniture") != std::string::npos)
        sns = "frn";      
      else if (value.find("http://www.opengis.net/citygml/appearance") != std::string::npos)
        sns = "app";      
      else if (value.find("http://www.w3.org/1999/xlink") != std::string::npos)
        sns = "xlink";
      else
        sns = "";
      if (sns != "") {
        size_t pos = name.find(":");
        if (pos == std::string::npos) 
          NS[sns] = "";
        else 
          NS[sns] = name.substr(pos + 1) + ":";
      }    
    }
  }
}

void build_dico_xlinks(pugi::xml_document& doc, std::map<std::string, pugi::xpath_node>& dallpoly) {
  std::string s = "//" + NS["gml"] + "Polygon" + "[@" + NS["gml"] + "id]";
  pugi::xpath_node_set nallpoly = doc.select_nodes(s.c_str());
  if (nallpoly.size() > 0)
   std::cout << "XLinks found, resolving them..." << std::flush;
  for (pugi::xpath_node_set::const_iterator it = nallpoly.begin(); it != nallpoly.end(); ++it)
    dallpoly[it->node().attribute("gml:id").value()] = *it;
  //-- for <gml:OrientableSurface>
  s = "//" + NS["gml"] + "OrientableSurface" + "[@" + NS["gml"] + "id" + "]";
  pugi::xpath_node_set nallosurf = doc.select_nodes(s.c_str());
  for (pugi::xpath_node_set::const_iterator it = nallosurf.begin(); it != nallosurf.end(); ++it)
    dallpoly[it->node().attribute("gml:id").value()] = *it;
  //-- checking xlinks validity now, not to be bitten later
  s = "//" + NS["gml"] + "surfaceMember" + "[@" + NS["xlink"] + "href" + "]";
  pugi::xpath_node_set nsmxlink = doc.select_nodes(s.c_str());
  for (pugi::xpath_node_set::const_iterator it = nsmxlink.begin(); it != nsmxlink.end(); ++it) 
  {
    std::string k = it->node().attribute("xlink:href").value();
    if (k[0] == '#')
      k = k.substr(1);
    if (dallpoly.count(k) == 0) 
    {
      std::string r = "One XLink couldn't be resolved (";
      r += it->node().attribute("xlink:href").value();
      r += ")";
      std::cout << r << std::endl;
      return;
    }
  }
  if (nallpoly.size() > 0)
    std::cout << "done." << std::endl;
}
