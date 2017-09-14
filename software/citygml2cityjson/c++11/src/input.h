#ifndef VAL3DITY_INPUT_DEFINITIONS_H
#define VAL3DITY_INPUT_DEFINITIONS_H

/*
 val3dity - Copyright (c) 2011-2017, Hugo Ledoux.  All rights reserved.
 
 Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions are met:
     * Redistributions of source code must retain the above copyright
       notice, this list of conditions and the following disclaimer.
     * Redistributions in binary form must reproduce the above copyright
       notice, this list of conditions and the following disclaimer in the
       documentation and/or other materials provided with the distribution.
     * Neither the name of the authors nor the
       names of its contributors may be used to endorse or promote products
       derived from this software without specific prior written permission.

 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 DISCLAIMED. IN NO EVENT SHALL HUGO LEDOUX BE LIABLE FOR ANY
 DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
*/


#include "Primitive.h"
#include "Surface.h"
#include "MultiSurface.h"
#include "Solid.h"
// #include "CompositeSurface.h"
// #include "CompositeSolid.h"
// #include "MultiSolid.h"
#include <fstream>
#include <string>
#include "pugixml/pugixml.hpp"
#include "nlohmann-json/json.hpp"


  
struct citygml_objects_walker: pugi::xml_tree_walker {
  std::vector<pugi::xml_node> lsNodes;
  virtual bool for_each(pugi::xml_node &node) 
  {
    const char *nodeType = node.name();
    const char *namespaceSeparator = strchr(nodeType, ':');
    if (namespaceSeparator != NULL) {
      nodeType = namespaceSeparator+1;
    }
    // if (strcmp(nodeType, "AuxiliaryTrafficArea") == 0 ||
    //     strcmp(nodeType, "Bridge") == 0 ||
    //     strcmp(nodeType, "Building") == 0 ||
    //     // strcmp(nodeType, "BuildingPart") == 0 ||
    //     strcmp(nodeType, "CityFurniture") == 0 ||
    //     strcmp(nodeType, "GenericCityObject") == 0 ||
    //     strcmp(nodeType, "LandUse") == 0 ||
    //     strcmp(nodeType, "PlantCover") == 0 ||
    //     strcmp(nodeType, "Railway") == 0 ||
    //     strcmp(nodeType, "ReliefFeature") == 0 ||
    //     strcmp(nodeType, "Road") == 0 ||
    //     strcmp(nodeType, "SolitaryVegetationObject") == 0 ||
    //     strcmp(nodeType, "TrafficArea") == 0 ||
    //     strcmp(nodeType, "Tunnel") == 0 ||
    //     strcmp(nodeType, "WaterBody") == 0) {
    if (strcmp(nodeType, "Building") == 0)
      lsNodes.push_back(node);
    } return true;
  }
};


struct primitives_walker: pugi::xml_tree_walker
{
  std::vector<pugi::xml_node> lsNodes;
  int depthprim = 999;
  virtual bool for_each(pugi::xml_node& node)
  { 
    const char *nodeType = node.name();
    const char *namespaceSeparator = strchr(nodeType, ':');
    if (namespaceSeparator != NULL) {
      nodeType = namespaceSeparator+1;
    }
    if (depth() == depthprim)
    {
      // std::cout << "back to reading mode" << node.name() << std::endl;
      depthprim = 999;
    }
    if ((strcmp(nodeType, "boundedBy") == 0))
      depthprim = depth();
    if ( (depth() < depthprim) && 
         ( (strcmp(nodeType, "Solid") == 0) ||
           // (strcmp(nodeType, "MultiSolid") == 0) ||
           // (strcmp(nodeType, "CompositeSolid") == 0) ||
           (strcmp(nodeType, "MultiSurface") == 0) ||
           (strcmp(nodeType, "boundedBy") == 0) ))
           // (strcmp(nodeType, "CompositeSurface") == 0) ) )
    {
      // std::cout << "----" << node.name() << std::endl;
      // std::cout << "depth " << depth() << std::endl;
      depthprim = depth();
      lsNodes.push_back(node);
    }
    return true; 
  }
};

//--

void              read_file_gml(std::string &ifile);
void              get_namespaces(pugi::xml_node& root, std::string& vcitygml);


std::vector<int>  process_gml_ring(const pugi::xml_node& n, Surface* sh);
Surface*          process_gml_surface(const pugi::xml_node& n, int id, std::map<std::string, pugi::xpath_node>& dallpoly, double tol_snap);
MultiSurface*     process_gml_multisurface(const pugi::xml_node& nms, std::map<std::string, pugi::xpath_node>& dallpoly, double tol_snap);
Solid*            process_gml_solid(const pugi::xml_node& nsolid, std::map<std::string, pugi::xpath_node>& dallpoly, double tol_snap);

void              build_dico_xlinks(pugi::xml_document& doc, std::map<std::string, pugi::xpath_node>& dallpoly);
void              process_gml_file_city_objects(pugi::xml_document& doc, std::map<std::string, std::vector<Primitive*> >& dPrimitives, std::map<std::string, pugi::xpath_node>& dallpoly, double tol_snap);

std::string       localise(std::string s);
std::string       remove_xml_namespace(const char* input);


#endif
