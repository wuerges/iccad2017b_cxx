#include <base.hpp>

#include <iostream>
#include <cassert>
#include <array>
#include <cmath>
// #include <string>
// #include <vector>
// #include <map>
// #include <utility>
// #include <cassert>

// using namespace iccad;

namespace iccad {
  std::ostream & operator<<(std::ostream & out, const Shape & s) {
    out << "Shape{"<<s.a<<", "<<s.b<<"}";
    // out << "Polygon(";
    // out << "(" << s.a.x << "," << s.a.y << "),";
    // out << "(" << s.b.x << "," << s.a.y << "),";
    // out << "(" << s.b.x << "," << s.b.y << "),";
    // out << "(" << s.a.x << "," << s.b.y << ")";
    // out << ")";
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

  constexpr double sq(double a) { return a*a; }

  const int64_t manhatan(const PT & a, const PT & b) {
    return abs(a.x - b.x) + abs(a.y - b.y) + abs(a.z - b.z);;
  }


  const bool operator<(const Shape & a, const Shape & b) {
    std::array<int, 6> va = {a.a.x, a.a.y, a.a.z, a.b.x, a.b.y, a.b.z };
    std::array<int, 6> vb = {b.a.x, b.a.y, b.a.z, b.b.x, b.b.y, b.b.z };
    return va < vb;
    // return
    //   a.a.x < b.a.x ||
    //   a.a.y < b.a.y ||
    //   a.a.z < b.a.z ||
    //   a.b.x < b.b.x ||
    //   a.b.y < b.b.y ||
    //   a.b.z < b.b.z;
  }

  const bool operator==(const Shape & a, const Shape & b) {
    std::array<int, 6> va = {a.a.x, a.a.y, a.a.z, a.b.x, a.b.y, a.b.z };
    std::array<int, 6> vb = {b.a.x, b.a.y, b.a.z, b.b.x, b.b.y, b.b.z };
    return va == vb;
  }
  const bool operator!=(const Shape & a, const Shape & b) {
    return ! (a == b);
  }

  int dist(int a, int b, int aw, int bw) {
    return std::max(abs(a - b) - (aw + bw), 0);
  }
  

  const int distance(const Shape & s1, const Shape & s2) {
    return dist((s1.a.x+s1.b.x) / 2, (s2.a.x+s2.b.x) / 2, abs(s1.b.x - s1.a.x)/2,  abs(s2.b.x - s2.a.x)/2)
          + dist((s1.a.y+s1.b.y) / 2, (s2.a.y+s2.b.y) / 2, abs(s1.b.y - s1.a.y)/2, abs(s2.b.y - s2.a.y)/2)
          + abs(s1.a.z - s2.b.z);
  }

  const int distance(const PT & pt, const Shape & s2) {
    return distance(Shape(pt, pt), s2);
    // return dist(pt.x, s2.a.x, 0, s2.b.x - s2.a.x)
    //       + dist(pt.y, s2.a.y, 0, s2.b.y - s2.a.y)
    //       + abs(pt.z - s2.b.z);
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
      return PT(
          min(a.x, b.x),
          min(a.y, b.y),
          min(a.z, b.z));
  }

  const PT max(const PT & a, const PT & b) {
      using std::max;
      return PT(
          max(a.x, b.x),
          max(a.y, b.y),
          max(a.z, b.z));
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

  Shape::Shape(const PT a_, const PT b_): a(min(a_,b_)), b(max(a_,b_))  {}

  Shape Shape::expand(int spacing) {
    return Shape(PT(a.x - spacing, a.y - spacing, a.z), PT(b.x - spacing, b.y - spacing, b.z));
  }
}
