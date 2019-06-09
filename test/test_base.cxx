#include <base.hpp>
#include <rapidcheck.h>


#include <iostream>
// #include <cassert>

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

void test_distance(const Shape & s1, const Shape & s2) {
    RC_LOG() << "distance is " << distance(s1, s2) << '\n';
    RC_LOG() << "collides? " << collides(s1, s2) << '\n';
    RC_ASSERT((distance(s1, s2) == 0) == collides(s1, s2));
}


int main(int argc, char ** argv) {

    PT a(10, 10, 0);
    PT b(20, 20, 0);
    PT c(30, 30, 0);
    PT d(15, 15, 0);
    PT e(0, 0, 0);

    
    cout << "Distance " << distance(c, Shape(a, b)) << '\n';
    // assert(distance(c, Shape(a, b)) == 21);
    
    cout << "Distance " << distance(d, Shape(a, b)) << '\n';
    // assert(distance(e, Shape(a, b)) == 1);

    cout << "Distance " << distance(e, Shape(a, b)) << '\n';
    // RC_ASSERT(distance(e, Shape(a, b)) == 21);

    
    cout << "Distance " << distance(c, Shape(a, c)) << '\n';
    // assert(distance(c, Shape(a, b)) == 21);
    
    cout << "Distance " << distance(d, Shape(a, c)) << '\n';
    // assert(distance(e, Shape(a, b)) == 1);

    cout << "Distance " << distance(e, Shape(a, c)) << '\n';
    // RC_ASSERT(distance(e, Shape(a, b)) == 21);



    rc::check("Check that shapes collide iff their distance is 0",
    test_distance);


    return 0;
}
