//
//  MultiSurface.h
//  val3dity
//
//  Created by Hugo Ledoux on 25/10/16.
//
//

#ifndef MultiSurface_h
#define MultiSurface_h

// #include "definitions.h"
#include "Primitive.h"
#include "Surface.h"

#include <string>


class MultiSurface : public Primitive 
{
public:
              MultiSurface(std::string id = ""); 
              ~MultiSurface(); 

  bool        is_empty();
  std::string get_report_xml();
  Primitive3D get_type();


  int           number_faces();
  bool          set_surface(Surface* s);
  Surface*      get_surface();

protected:
  Surface* _surface;
};


#endif /* MultiSurface_h */
