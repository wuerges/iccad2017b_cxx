#include <geometry.hpp>
#include <parser.hpp>
#include <iostream>
#include <treap.hpp>
#include <vector>
#include <random>
#include <algorithm>
#include <chrono>

using namespace std;
using namespace std::chrono;
using namespace iccad;

size_t count_shapes(const Input & inp) {
  size_t c = 0;
  for (int i = 0; i < inp.shapes.size(); i++) {
      for(auto & v : inp.shapes[i]) {
        ++c;
      }
  }
  return c;
}

int test_treap(size_t slice, const Input & inp)
{

    vector<Shape> shapes;

    size_t slice_count = 0;

    for (int i = 0; i < inp.shapes.size(); i++) {
      if(slice_count >= slice) break;
      int layer = i*inp.viaCost;
        for(auto & v : inp.shapes[i]) {
          if(slice_count >= slice) break;
          shapes.push_back(Shape(PT(v[0], v[1], layer), PT(v[2], v[3], layer)));
          ++slice_count;
        }
    }

    unique_ptr<Node> root = make_unique<Node>(shapes[0]);
    for (size_t i = 1; i < shapes.size(); i++) {
      root->add(shapes[i]);
    }

    int result = 0;
    for(auto s : shapes) {
      if(root->hits(s.a, s.b)) {
        result++;
      }
    }
    // cout << result << "\n";

    return result;
}


int main(int n, char**argv) {

  // std::cout << "Parsing file " << argv[1] << '\n';

  Input i;
  // try {
    parser::parse_file(i, argv[1]);

  // }
  // catch (...) {
  //   std::cerr << "ERROR: PARSE FAILED\n";
  // }

  // std::cout << "Via Cost: " << i.viaCost << '\n';
  // std::cout << "sp: " << i.spacing << '\n';
  // std::cout << "boundary:" << i.boundary[0] << ' '
	//   << i.boundary[1] << ' '
	//   << i.boundary[2] << ' '
	//   << i.boundary[3] << '\n';
  // std::cout << i.shapes.size() << '\n';
  // std::cout << i.vias.size() << '\n';
  // std::cout << i.obstacles.size() << '\n';

  shuffle(i.shapes.begin(), i.shapes.end(), std::default_random_engine(0));

  // Grid g;
  // convert(i, g);
  // generate_hannan(g);

  size_t n_shapes = count_shapes(i);


  for(int slice = 100; slice < n_shapes+500; slice+= 500) {
    auto t1 = std::chrono::high_resolution_clock::now();
    size_t result = test_treap(slice, i);
   // size_t result = test_treap(n_shapes, i);
    auto t2 = std::chrono::high_resolution_clock::now();
    auto duration = duration_cast<microseconds>( t2 - t1 ).count();
    cout << slice << ", " << duration <<","<< result<<'\n';
  }

  return 0;
}
