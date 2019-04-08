#include <model.hpp>
#include <parser.hpp>
#include <iostream>
#include <algo.hpp>
#include <vector>

using namespace std;
using namespace iccad;

int test_treap()
{
    vector<pair<int, int>> inputs = {{1, 2}, {5, 7}, {2, 8}, {3, 4}};

    unique_ptr<Node> root(new Node({0, 0, 0, 0, 1}));

    for(auto [a, b] :inputs) {
        root->add(Shape{PT{a, b, 0}, PT{a+1, b+1, 0}});
    }

    cout << root->count << '\n';
    root->print();

    for(int a = 0; a < 10; ++a) {
        for(int b = a; b < 10; ++b) {
            int q = root->query({a, b, 0});
            cout << "query "<<a<<","<<b<<" = " << q << "\n\n";
        }
    }
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

  Grid g;
  convert(i, g);
  generate_hannan(g);


  test_treap();



  return 0;
}
