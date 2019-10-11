#pragma once

#include <geometry.hpp>
#include <unordered_map>

namespace iccad {
  using std::unordered_map;
  using namespace geometry;

  struct MUF {

    unordered_map<Shape, Shape> parent;
    unordered_map<Shape, int> rank;

    Shape Find(const Shape & x) ;
    void Union(const Shape & x, const Shape & y);
  };


}
