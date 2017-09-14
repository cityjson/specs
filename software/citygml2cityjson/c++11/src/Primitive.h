//
//  Primitive.hpp
//  val3dity
//
//  Created by Hugo Ledoux on 25/10/16.
//
//

#ifndef Primitive_h
#define Primitive_h

#include "definitions.h"
#include <map>
#include <vector>
#include <set>
#include <string>


class Primitive
{
public:
  Primitive  ();
  ~Primitive ();

  virtual bool          is_empty() = 0;
  virtual Primitive3D   get_type() = 0;


  std::string           get_id();
  void                  set_id(std::string id);

protected:
  std::string   _id;


};


#endif /* Primitive_h */
