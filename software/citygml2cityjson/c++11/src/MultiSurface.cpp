//
//  MultiSurface.cpp
//  val3dity
//
//  Created by Hugo Ledoux on 25/10/16.
//
//

#include "MultiSurface.h"
#include "Primitive.h"
#include "input.h"


MultiSurface::MultiSurface(std::string id) {
  _id = id;
}

MultiSurface::~MultiSurface() {
}



Primitive3D MultiSurface::get_type() 
{
  return MULTISURFACE;
}




bool MultiSurface::is_empty() {
  return _surface->is_empty();
}



int MultiSurface::number_faces() 
{
  return _surface->number_faces();
}


bool MultiSurface::set_surface(Surface* s) 
{
  _surface = s;
  return true;
}



Surface* MultiSurface::get_surface() 
{
  return _surface;
}

