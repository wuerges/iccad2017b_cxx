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

// #include "generators.hpp"

void test_col_segment(const PT center, int radius,  const int64_t y, const int z, const int64_t x1, const int64_t x2, bool expected) {

    bool result = sphere_collides(center, radius, y, z, x1, x2);
    RC_LOG() << "expected " << expected << '\n';
    RC_LOG() << "got " << result << '\n';
    RC_ASSERT(result == expected);
    
}

void test_collision(const PT center, int radius, const Shape & shape, bool expected) {

    bool result = sphere_collides(center, radius, shape.a, shape.b);

    RC_LOG() << "expected " << expected << '\n';
    RC_LOG() << "got " << result << '\n';
    
    RC_ASSERT(result == expected);
}

int main(int n, char **argv) {

    rc::check("test1", [](){ test_col_segment(PT{0, 0, 0}, 2, 0, 0, -1, 1, false); });
    rc::check("test2", [](){ test_collision(PT{0, 0, 0}, 2, Shape{PT{3, 0, 0}, PT{4, 1, 0}}, false); });
    rc::check("test3", [](){ test_collision(PT{1, 2, 0}, 2, Shape{PT{3, 0, 0}, PT{4, 1, 0}}, false); });
}