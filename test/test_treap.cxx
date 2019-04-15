#include <model.hpp>
#include <parser.hpp>
#include <iostream>
#include <algo.hpp>
#include <vector>
#include <cassert>
#include <rapidcheck.h>
#include <algorithm>
#include <set>

using namespace std;
using namespace iccad;


namespace rc {

  const auto genPT = gen::container<array<int, 3>>(gen::inRange(0, 100000));

  template<>
  struct Arbitrary<PT> {
    static Gen<PT> arbitrary() {
      return gen::map(genPT,
        [](array<int, 3> vs) {
          return PT(vs[0], vs[1], 0);
        });
    }
  };


  template<>
  struct Arbitrary<Shape> {
    static Gen<Shape> arbitrary() {
      return gen::map(gen::container<array<array<int, 3>, 2>>(genPT),
        [](array<array<int, 3>, 2> pts) {
          return Shape(
            PT(min(pts[0][0], pts[1][0]), min(pts[0][1], pts[1][1]), 0 ), // min(pts[0][2], pts[1][2])),
            PT(max(pts[0][0]+1, pts[1][0]), max(pts[0][1]+1, pts[1][1]), 0 ) //max(pts[0][2], pts[1][2]))
          );
        });
    }
  };

}

int test_treap(const vector<Shape> & shapes)
{
    if(shapes.size() == 0) return 0;
    unique_ptr<Node> root(new Node(shapes[0]));

    for (size_t i = 1; i < shapes.size(); i++) {
      root->add(shapes[i]);
    }

    // cout << root->count << '\n';
    // root->print();
    //

    for (const Shape & s : shapes) {
      unsigned count = root->query(s.a, s.b);
      // cout << count << " ";
      // if(count > 1 && root->count < 10)  {
      //   cout << "-----------------\n";
      //   root->print();
      //   cout << "shape: " <<s<<" count: " << count<< "\n";
      // }
      RC_ASSERT(count > 0);
    }
    // cout << '\n';
    return 0;
}

int test_treap_collect(const vector<Shape> & shapes)
{
    if(shapes.size() == 0) return 0;
    unique_ptr<Node> root(new Node(shapes[0]));

    for (size_t i = 1; i < shapes.size(); i++) {
      root->add(shapes[i]);
    }

    for (const Shape & s : shapes) {
      vector<Shape> res;
      root->collect(res, s.a, s.b);

      for(const Shape & rs : res) {
        RC_ASSERT(collides(rs, s));
      }

    }

    return 0;
}

int test_treap_collect_neg(const vector<Shape> & shapes)
{
    if(shapes.size() == 0) return 0;
    unique_ptr<Node> root(new Node(shapes[0]));


    for (size_t i = 1; i < shapes.size(); i++) {
      root->add(shapes[i]);
    }

    for (const Shape & s : shapes) {
      set<Shape> everything;
      for(auto & si : shapes) {
        everything.insert(si);
      }

      vector<Shape> res;
      root->collect(res, s.a, s.b);

      RC_LOG() << "\n\n// Query\n" << s << '\n';



      RC_LOG() << "// collected shapes \n";
      for(const Shape & rs : res) {
        RC_LOG() << rs <<'\n';
        everything.erase(rs);
      }

      RC_LOG() << "// uncollected shapes \n";
      for(const Shape & si : everything) {
        RC_LOG() << si << '\n';
      }

      for(const Shape & si : everything) {
        RC_ASSERT(!collides(s, si));
      }
    }
    //
    // cout << "// Unmatched shapes"
    // for(const Shape & s : everything) {
    //
    //   RC_LOG() << s << '\n';
    // }
    //
    //

    return 0;
}


int main(int n, char**argv) {

  rc::check("Check that added shapes can be queried",
            [](const std::vector<Shape> &shapes) {
              test_treap(shapes);
            });

  rc::check("Check that collected shapes are colliding with the query",
            [](const std::vector<Shape> &shapes) {
              test_treap_collect(shapes);
            });

  rc::check("Check that shapes that were not collected do not collide with the query",
            [](const std::vector<Shape> &shapes) {
              test_treap_collect_neg(shapes);
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
