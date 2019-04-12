#pragma once

#include <array>
#include <cassert>

namespace iccad {
    /**
     * An array representing a 3D point.
     * Each position represents the x, y and z coordinates, in this order.
     */
    // using PT = std::array<int, 3>;
    struct PT {
      int x, y, z;
      int operator[](int i) const {
        if(i==0) return x;
        if(i==1) return y;
        if(i==2) return z;
        assert(0 && "point index out of bounds");
        return -1;
      }
      bool operator<(const PT & o) const {
        return x < o.x && y < o.y && z < o.z;
      }
      bool operator<=(const PT & o) const {
        return x <= o.x && y <= o.y && z <= o.z;
      }
      bool operator>(const PT & o) const {
        return o < *this;
      }
      bool operator>=(const PT & o) const {
        return o <= *this;
      }
    };
    /**
     * Two points that represent the oposite points of a cuboid
     * The first point is the low value for the x, y and z coordinates.
     * The second point is the high value for the x, y, and z coordinates.
     */

    struct Shape {
      PT a, b;
    };
    // using Shape = std::array<PT, 2>;


    PT min(PT a, PT b) {
        using std::min;
        return PT{
            min(a[0], b[0]),
            min(a[1], b[1]),
            min(a[2], b[2])};
    }

    PT max(PT a, PT b) {
        using std::max;
        return PT{
            max(a[0], b[0]),
            max(a[1], b[1]),
            max(a[2], b[2])};
    }


}
