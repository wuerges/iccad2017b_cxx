#include <algo.hpp>
#include <algorithm>
#include <cassert>
#include <iostream>
#include <model.hpp>
#include <parser.hpp>
#include <rapidcheck.h>
#include <set>
#include <vector>

#include "generators.hpp"

using namespace std;
using namespace iccad;

void test_treap(const Shape & shape, const vector<Shape> &shapes, int i1, int i2) {

    int b = max(abs(i1), abs(i2))+1;
    int a = min(abs(i1), abs(i2));

    Treap treap;
    treap.populate(shapes);


    auto shapes_a = treap.collect_diamond_2(shape, 0, a);
    auto shapes_b = treap.collect_diamond_2(shape, a, b);
    auto shapes_all = treap.collect_diamond(shape, b);

    sort(shapes_a.begin(), shapes_a.end());
    sort(shapes_b.begin(), shapes_b.end());

    for(auto s : shapes_all) {

        auto f_a = lower_bound(shapes_a.begin(), shapes_a.end(), s) != shapes_a.end();
        auto f_b = lower_bound(shapes_b.begin(), shapes_b.end(), s) != shapes_b.end();

        RC_ASSERT( (f_a && !f_b) || (f_b && !f_a) );
    }
}

int main(int n, char **argv) {

  rc::check("Check that added shapes can be queried", test_treap);
  return 0;
}
