#include <model.hpp>
#include <parser.hpp>
#include <iostream>
#include <algo.hpp>
#include <vector>
#include <cassert>
#include <rapidcheck.h>
#include <algorithm>
#include <set>
#include <array>

namespace rc {
  using namespace std;
  using namespace iccad;

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
