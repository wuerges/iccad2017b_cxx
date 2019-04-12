#include <base.hpp>

#include <iostream>
#include <cassert>
// #include <string>
// #include <vector>
// #include <map>
// #include <utility>
// #include <cassert>

// using namespace iccad;

namespace iccad {
  std::ostream & operator<<(std::ostream & out, const Shape & s) {
    out << "Shape{"<<s.a<<", "<<s.b<<"}";
    return out;
  }
  std::ostream & operator<<(std::ostream & out, const PT & s) {
    out << "PT{"<<s.x<<", "<<s.y<<", "<<s.z<<"}";
    return out;
  }


  int PT::operator[](int i) const {
    if(i==0) return x;
    if(i==1) return y;
    if(i==2) return z;
    assert(0 && "point index out of bounds");
    return -1;
  }
  const bool operator<(const PT & a, const PT & b) {
    return a.x < b.x && a.y < b.y && a.z < b.z;
  }
  const bool operator<=(const PT & a, const PT & b) {
    return a.x <= b.x && a.y <= b.y && a.z <= b.z;
  }
  const bool operator>(const PT & a, const PT & b) {
    return b < a;
  }
  const bool operator>=(const PT & a, const PT & b) {
    return b <= a;
  }
  const PT min(const PT & a, const PT & b) {
      using std::min;
      return PT{
          min(a.x, b.x),
          min(a.y, b.y),
          min(a.z, b.z)};
  }

  const PT max(const PT & a, const PT & b) {
      using std::max;
      return PT{
          max(a.x, b.x),
          max(a.y, b.y),
          max(a.z, b.z)};
  }

  const bool collides(const Shape & s1, const Shape & s2) {
    // return
    //   RectA.X1 < RectB.X2 && RectA.X2 > RectB.X1 &&
    //   RectA.Y1 < RectB.Y2 && RectA.Y2 > RectB.Y1;

    return s1.a.x <= s2.b.x && s1.b.x >= s2.a.x &&
           s1.a.y <= s2.b.y && s1.b.y >= s2.a.y &&
           s1.a.z <= s2.b.z && s1.b.z >= s2.a.z;


    // return inside(s1, s2.a) || inside(s1, s2.b) || inside(s2, s1.a) || inside(s2, s1.b);

  }

}
