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
    if(a == 0) {
        RC_DISCARD("a > 0");
    }

    Treap treap;
    treap.populate(shapes);
    auto shapes_all = treap.collect_diamond(shape, b);
    if(shapes_all.empty()) {
        RC_DISCARD("Empty shapes.");
    }
    RC_LOG() << "Collected Shapes:\n";
    for(auto s : shapes_all) {
        RC_LOG() << s << '\n';
    }

    auto shapes_a = treap.collect_diamond_2(shape, 0, a);
    RC_LOG() << "Collected Shapes for (0, a]:\n";
    for(auto s : shapes_a) {
        RC_LOG() << s << '\n';
    }
    auto shapes_b = treap.collect_diamond_2(shape, a, b);
    RC_LOG() << "Collected Shapes for (a,b]:\n";
    for(auto s : shapes_b) {
        RC_LOG() << s << '\n';
    }

    sort(shapes_a.begin(), shapes_a.end());
    sort(shapes_b.begin(), shapes_b.end());

    for(auto s : shapes_all) {

        auto f_a = binary_search(shapes_a.begin(), shapes_a.end(), s);
        auto f_b = binary_search(shapes_b.begin(), shapes_b.end(), s);

        RC_LOG() << "found in a : " << f_a << '\n';
        RC_LOG() << "found in b : " << f_b << '\n';
        RC_LOG() << "a=" << a << " b=" << b << '\n';
        RC_LOG() << "center=" << shape << '\n';

        RC_ASSERT( (f_a && !f_b) || (f_b && !f_a) );
    }
}

int main(int n, char **argv) {

  rc::check("Check that added shapes can be queried", test_treap);
  return 0;
}
