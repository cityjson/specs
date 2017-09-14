
#include "Solid.h"
#include "definitions.h"
#include "input.h"


Solid::Solid(std::string id)
{
  _id = id;
}


Solid::~Solid()
{}

Surface* Solid::get_oshell()
{
  return _shells[0];
}

Primitive3D Solid::get_type() 
{
  return SOLID;
}

void Solid::set_oshell(Surface* sh)
{
  if (_shells.empty())
    _shells.push_back(sh);
  else
    _shells[0] = sh;
}


const std::vector<Surface*>& Solid::get_shells()
{
  return _shells;
}


void Solid::add_ishell(Surface* sh)
{
  _shells.push_back(sh);
}


bool Solid::is_empty()
{
  for (auto& sh : _shells)
  {
    if (sh->is_empty() == true)
      return true;
  }
  return false;
}


int Solid::num_ishells()
{
  return (_shells.size() - 1);
}

int Solid::num_faces()
{
  int total = 0;
  for (auto& sh : _shells)
    total += sh->number_faces();
  return total;
}

int Solid::num_vertices()
{
  int total = 0;
  for (auto& sh : _shells)
    total += sh->number_vertices();
  return total;
}

