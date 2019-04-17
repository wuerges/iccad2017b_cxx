#include <model.hpp>
#include <parser.hpp>
#include <iostream>
#include <algo.hpp>
#include <vector>
#include <random>
#include <algorithm>
#include <chrono>
#include <mst.hpp>

using namespace std;
using namespace std::chrono;
using namespace iccad;

int main(int argc, char ** argv) {
  Treap treap;

  Input i;
  parser::parse_file(i, argv[1]);
  vector<Shape> shapes = get_routed_shapes(i);

  cout << shapes.size() << '\n';


  MST mst;

  auto res = mst.run(shapes);

  int total = 0;

  for(auto & [a, b] : res) {
    total += distance(a, b);
  }

  cout << "total distance in tree: " << total << '\n';





  return 0;
}
