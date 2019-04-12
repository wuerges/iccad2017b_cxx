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
  out << "Shape{{,,}, {,,}}";
  return out;

}
}
