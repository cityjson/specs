//
//  Surface.cpp
//  val3dity
//
//  Created by Hugo Ledoux on 25/10/16.
//
//

#include "Surface.h"
#include "input.h"
#include <sstream>

using namespace std;


Surface::Surface(int id, double tol_snap)
{
  _id = id;
  if (tol_snap < 1e-8)
    _tol_snap = -1;
  else
    _tol_snap = tol_snap;
}

Surface::~Surface()
{
  // TODO: clear memory properly
  _lsPts.clear();
}

int Surface::get_id()
{
  return _id;
}

bool Surface::is_empty()
{
  return (_lsPts.empty() || _lsFaces.empty());
}



int Surface::add_point(Point3 p)
{
  _vertices_added += 1;
  auto it = _dPts.find(get_coords_key(&p));
  if (it == _dPts.end()) 
  {
    _lsPts.push_back(p);
    _dPts[get_coords_key(&p)] = (_lsPts.size() - 1); 
    return (_lsPts.size() - 1);
  }
  return it->second;
}


void Surface::add_face(std::vector< std::vector<int> > f, std::string id)
{
  _lsFaces.push_back(f);
  if (id.empty() == true)
    _lsFacesID.push_back(std::to_string(_lsFaces.size()));
  else
    _lsFacesID.push_back(id);
}


int Surface::number_vertices()
{
  return _lsPts.size();
}


int Surface::number_faces()
{
  return _lsFaces.size();
}




