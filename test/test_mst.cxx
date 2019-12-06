#include <geometry.hpp>
#include <parser.hpp>
#include <iostream>
#include <treap.hpp>
#include <vector>
#include <random>
#include <algorithm>
#include <chrono>
#include <mst.hpp>

using namespace std;
using namespace std::chrono;
using namespace iccad;

int main(int argc, char ** argv) {

  Input i;
  parser::parse_file(i, argv[1]);
  vector<Shape> shapes = get_routed_shapes(i);
  vector<Shape> obs_vector;

  cout << shapes.size() << '\n';


  MST mst;
  Treap treap, obstacles;
  treap.populate(shapes);
  auto res = mst.run(treap, obstacles, shapes, obs_vector, i.boundary);

  int total = 0;

  for(auto & r : res) {
    total += r.length();
  }

  cout << "total distance in tree: " << total << '\n';





  return 0;
}
