

#ifndef Surface_h
#define Surface_h

#include "definitions.h"
#include <string>
#include <vector>
#include <set>
#include <unordered_map>


class Surface
{
public:
  Surface  (int id = -1, double tol_snap = 0.0);
  ~Surface ();

  int    number_vertices();
  int    number_faces();
  int    get_id();

  bool   is_empty();
  int    add_point(Point3 p);
  void   add_face(std::vector< std::vector<int> > f, std::string id = "");

  
private:
  int                                     _id;
  std::vector<Point3>                     _lsPts;
  std::unordered_map< std::string, int >  _dPts;
  std::vector< std::vector< std::vector<int> > >    _lsFaces;
  std::vector<std::string>                _lsFacesID;
  double                                  _tol_snap;

};


#endif /* Surface_h */
