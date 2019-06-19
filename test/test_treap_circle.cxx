#include <algo.hpp>
#include <algorithm>
#include <cassert>
#include <iostream>
#include <model.hpp>
#include <parser.hpp>
#include <rapidcheck.h>
#include <set>
#include <vector>

using namespace std;
using namespace iccad;

#include "generators.hpp"

void test_treap(const PT center, int radius, const vector<Shape> &shapes) {
  if (radius <= 0) {
    RC_DISCARD("radius must be > 0");
    return;
  }
  if (shapes.size() == 0) {
    RC_DISCARD("discarding empty testcase");
    return;
  }

  Treap treap;
  treap.populate(shapes);

  int count_treap = treap.query(center, radius);
  RC_LOG() << "number of shapes in query: " << count_treap << '\n';

  int count = 0;
  for(const Shape &s : shapes) {
    if(sphere_collides(center, radius, s.a, s.b)) count++;
  }
  RC_LOG() << "number of shapes counted manually: " << count << '\n';
  RC_ASSERT(count == count_treap);
}




int main(int n, char **argv) {

  // test_treap(PT{0,0,0}, 2, {Shape{PT{0,0,0,}, {1, 1, 0}}});

     rc::check("Check that added shapes can be queried", test_treap);
               
  //   rc::check("Check that collected shapes are colliding with the query",
  //             [](const std::vector<Shape> &shapes) {
  //               test_treap_collect(shapes);
  //             });

  //   rc::check("Check that shapes that were not collected do not collide with
  //   the query",
  //             [](const std::vector<Shape> &shapes) {
  //               test_treap_collect_neg(shapes);
  //             });

//  vector<Shape> shapes;

//  shapes.push_back(Shape{PT{10, 10, 0}, PT{20, 20, 0}});
  // shapes.push_back(Shape{PT{30, 30, 0}, PT{40, 40, 0}});
  // shapes.push_back(Shape{PT{10, 10, 0}, PT{40, 40, 0}});
  // shapes.push_back(Shape{PT{0, 0, 0}, PT{20, 20, 0}});
  // shapes.push_back(Shape{PT{10, 10, 0}, PT{50, 50, 0}});

//  Treap tree;
//  tree.populate(shapes);

//  cout << "all shapes:\n";
//  for (auto s : shapes) {
//    cout << s << '\n';
//  }
//  cout << "testing radius search\n";
  // cout << tree.query(PT{40, 10, 0}, 10) << "\n";
//  for (auto s : tree.collect(PT{5, 22, 0}, 5)) {
//    cout << s << '\n';
//  }

/*
  cout << "contains?\n";
  cout << sphere_contains(PT{15, 15, 0}, 8, PT{10, 10, 0}, PT{20, 20, 0})
       << '\n';
  cout << sphere_contains(PT{15, 15, 0}, 7, PT{10, 10, 0}, PT{20, 20, 0})
       << '\n';
  cout << sphere_contains(PT{16, 14, 0}, 8, PT{10, 10, 0}, PT{20, 20, 0})

       << '\n';
*/
//  cout << "neighboor search:\n";
//  for (auto s : tree.neighboors_sphere(Shape{PT{10, 10, 0}, PT{20, 20, 0}}, 10)) {
//    cout << s << '\n';
//  } 
      // test_treap(shapes);

      return 0;
}
