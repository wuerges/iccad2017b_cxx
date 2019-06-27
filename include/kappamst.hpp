#pragma once

#include <base.hpp>
#include <algo.hpp>
#include <model.hpp>
#include <muf.hpp>

#include <vector>
#include <unordered_map>

namespace iccad {
  using std::vector, std::pair, std::unordered_map;

  struct KMST {

    vector<pair<Shape, Shape>> run(const Treap & treap, const Treap & obstacles,
      const vector<Shape> & shapes, const V1D & boundary) ;

private:
    MUF muf;

  };


}
