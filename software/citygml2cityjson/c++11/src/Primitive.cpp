//
//  Primitive.cpp
//  val3dity
//
//  Created by Hugo Ledoux on 25/10/16.
//
//

#include "Primitive.h"
#include "input.h"
#include <iostream>


Primitive::Primitive() {
}

Primitive::~Primitive() {
}

std::string  Primitive::get_id()
{
  return _id;
}

void Primitive::set_id(std::string id)
{
  _id = id;
}

