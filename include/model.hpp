#pragma once

#include <vector>
#include <utility>

namespace model 
{

/**
 * Represents either a point, with size 2
 * or a rectangle with size 4.
 */
typedef std::vector<unsigned int> V1D;

struct Input {

  int viaCost, spacing, layers, n_shapes, n_vias, n_obst;
  V1D boundary;

  std::vector<std::vector<V1D>> shapes;
  std::vector<std::vector<V1D>> vias;
  std::vector<std::vector<V1D>> obstacles;
};

}
