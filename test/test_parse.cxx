#include <model.hpp>
#include <parser.hpp>
#include <iostream>

int main(int n, char**argv) {

  std::cout << "Parsing file " << argv[1] << '\n';

  model::Input i;
  model::parser::parse_file(i, argv[1]);

  std::cout << "Via Cost: " << i.viaCost << '\n';
  return 0;
}
