#pragma once

#include <array>
#include <cassert>
#include <iostream>

namespace iccad {
    /**
     * An array representing a 3D point.
     * Each position represents the x, y and z coordinates, in this order.
     */
    // using PT = std::array<int, 3>;

    using std::ostream;
    struct PT {
      PT(int _x, int _y, int _z): x(_x), y(_y), z(_z) {}
      PT() {}

      int x, y, z;
      int operator[](int i) const;

      friend ostream & operator<<(ostream &out, const PT & p);

      friend const bool operator<(const PT & a, const PT & b);
      friend const bool operator<=(const PT & a, const PT & b);
      friend const bool operator>(const PT & a, const PT & b);
      friend const bool operator>=(const PT & a, const PT & b);
      friend const PT min(const PT & a, const PT & b);
      friend const PT max(const PT & a, const PT & b);

      friend const int64_t manhatan(const PT & a, const PT & b);

      // void operator=(const PT & p) {
      //   x = p.x;
      //   y = p.y;
      //   z = p.z;
      // }

    };
    /**
     * Two points that represent the oposite points of a cuboid
     * The first point is the low value for the x, y and z coordinates.
     * The second point is the high value for the x, y, and z coordinates.
     */

    struct Shape {
      Shape(const PT _a, const PT _b);      
      Shape() {}
      PT a, b;
      friend const bool collides(const Shape & a, const Shape & b);
      friend std::ostream & operator<<(std::ostream & out, const Shape & s);

      friend const bool operator<(const Shape & a, const Shape & b);
      friend const bool operator!=(const Shape & a, const Shape & b);
      friend const bool operator==(const Shape & a, const Shape & b);

      friend const int distance(const Shape & a, const Shape & b);
      friend const int distance(const PT & pt, const Shape & s2);

      Shape expand(int spacing);

      // void operator=(const Shape & s) {
      //   a = s.a;
      //   b = s.b;
      // }

    };
    // using Shape = std::array<PT, 2>;




}
