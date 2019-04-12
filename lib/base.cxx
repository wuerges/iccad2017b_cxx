#include <base.hpp>

#include <iostream>
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
}
