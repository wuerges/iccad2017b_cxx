#include <model.hpp>
#include <parser.hpp>
#include <iostream>
#include <algo.hpp>
#include <vector>
#include <random>
#include <algorithm>

using namespace std;
using namespace iccad;

int test_treap(const Input & inp)
{

    vector<Shape> shapes;

    for (int i = 0; i < inp.shapes.size(); i++) {
        int layer = i*inp.viaCost;
        for(auto & v : inp.shapes[i]) {
          shapes.push_back(Shape(PT(v[0], v[1], layer), PT(v[2], v[3], layer)));
          // cout << "back: " << shapes.back() << '\n';
          // cout << "a: " << PT(v[0], v[1], layer) << " b: " << PT(v[2], v[3], layer) << "\n";
          // cout << "shape:" << Shape(PT(v[0], v[1], layer), PT(v[2], v[3], layer)) << "\n\n";
        }
    }

    unique_ptr<Node> root = make_unique<Node>(shapes[0]);
    for (size_t i = 1; i < shapes.size(); i++) {
      root->add(shapes[i]);
    }


    cout << root->count << '\n';
    // root->print();

    for(auto s : shapes) {
      int result = 100000;
      result = root->query(s.a, s.b);
      cout << result << " ";
    }
    cout << "\n";

    // for (int i = 0; i < inp.shapes.size(); i++) {
    //     int layer = i*inp.viaCost;
    //     for(auto & v : inp.shapes[i]) {
    //         Shape shape{
    //                 PT{v[0], v[1], layer},
    //                 PT{v[2], v[3], layer}};
    //
    //         int result = 100000;
    //         int xsize = inp.boundary[2] - inp.boundary[0];
    //         int ysize = inp.boundary[3] - inp.boundary[1];
    //         int zsize = inp.n_vias*inp.viaCost;
    //         while(result > 1000) {
    //             result = root->query(shape.a, shape.b);
    //         }
    //         cout << result << " ";
    //     }
    // }
    // cout << "\n";

    //
    // for(int a = 0; a < 10; ++a) {
    //     for(int b = a; b < 10; ++b) {
    //         //int q = root->query({a, b, 0});
    //         //cout << "query "<<a<<","<<b<<" = " << q << "\n\n";
    //     }
    // }
    return 0;
}


int main(int n, char**argv) {

  std::cout << "Parsing file " << argv[1] << '\n';

  Input i;
  // try {
    parser::parse_file(i, argv[1]);
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


  shuffle(i.shapes.begin(), i.shapes.end(), std::default_random_engine(0));


  // Grid g;
  // convert(i, g);
  // generate_hannan(g);


  test_treap(i);



  return 0;
}
