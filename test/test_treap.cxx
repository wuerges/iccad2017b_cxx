#include <model.hpp>
#include <parser.hpp>
#include <iostream>
#include <algo.hpp>
#include <vector>
#include <cassert>
#include <rapidcheck.h>
#include <algorithm>

using namespace std;
using namespace iccad;


namespace rc {

  template<>
  struct Arbitrary<PT> {
    static Gen<PT> arbitrary() {
      return gen::build<PT>(
        gen::set(&PT::x, gen::inRange(0, 100000)),
        gen::set(&PT::y, gen::inRange(0, 100000)),
        gen::set(&PT::z, gen::inRange(0, 7)));
    }
  };

  template<>
  struct Arbitrary<Shape> {
    static Gen<Shape> arbitrary() {
      return gen::map(gen::arbitrary<array<PT, 2>>(),
        [](array<PT, 2> pts) {
          return Shape{
            PT{min(pts[0][0], pts[1][0]), min(pts[0][1], pts[1][1]), min(pts[0][2], pts[1][2])},
            PT{max(pts[0][0], pts[1][0]), max(pts[0][1], pts[1][1]), max(pts[0][2], pts[1][2])},
          };
        });
    }
  };

}

int test_treap(const vector<Shape> & shapes)
{
    unique_ptr<Node> root(new Node({0, 0, 0, 0, 1}));

    for (const Shape & s : shapes) {
      root->add(s);
    }

    cout << root->count << '\n';
    root->print();
    //

    for (const Shape & s : shapes) {
      unsigned count = root->query(s.a, s.b);
      RC_ASSERT(count > 0);
    }
    return 0;
}


int main(int n, char**argv) {

  rc::check("Check that added shapes can be queried",
            [](const std::vector<Shape> &shapes) {
              test_treap(shapes);
            });

  // vector<Shape> shapes;
  //
  // shapes.push_back(Shape{PT{10, 10, 0}, PT{20, 20, 0}});
  // shapes.push_back(Shape{PT{30, 30, 0}, PT{40, 40, 0}});
  // shapes.push_back(Shape{PT{10, 10, 0}, PT{40, 40, 0}});
  // shapes.push_back(Shape{PT{0, 0, 0}, PT{20, 20, 0}});
  // shapes.push_back(Shape{PT{10, 10, 0}, PT{50, 50, 0}});
  // test_treap(shapes);




  return 0;
}
