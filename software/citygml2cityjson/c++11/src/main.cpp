

#include <fstream>
#include <iostream>
#include <sstream>
#include <set>
#include <map>
#include "input.h"



///////////////


int main(int argc, char *argv[]) {
  const char* tmp = (argc > 1) ? argv[1] : "../data/b1_lod2_cs_w_sem.gml";
  
  std::string nameinput = tmp;
  std::cout << "Input file: " << nameinput << std::endl;

  read_file_gml(nameinput);

  return 1;
}

