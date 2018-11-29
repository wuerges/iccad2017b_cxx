#include <model.hpp>
#include <parser.hpp>
#include <iostream>

int main(int n, char**argv) {

  std::cout << "Parsing file " << argv[1] << '\n';

  model::Input i;
  // try {
    model::parser::parse_file(i, argv[1]);
  // }
  // catch (...) {
  //   std::cerr << "ERROR: PARSE FAILED\n";
  // }

  std::cout << "Via Cost: " << i.viaCost << '\n';
  std::cout << "sp: " << i.spacing << '\n';
  std::cout << "boundary:" << i.boundary[0] << ' '
	  << i.boundary[1] << ' '
	  << i.boundary[2] << ' '
	  << i.boundary[3] << '\n';
  std::cout << i.shapes.size() << '\n';
  std::cout << i.vias.size() << '\n';
  std::cout << i.obstacles.size() << '\n';

  model::Grid g;
  convert(i, g);
  return 0;
}
