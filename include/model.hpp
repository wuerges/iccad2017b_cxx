#pragma once

#include <vector>
#include <utility>

namespace model 
{

typedef std::pair<unsigned int, unsigned int> P2D;
typedef std::pair<P2D, P2D> R2D;

struct Input {

  int viaCost, spacing, layers, n_shapes, n_vias, n_obst;
  R2D boundary;

  std::vector<std::vector<R2D>> shapes;
  std::vector<std::vector<P2D>> vias;
  std::vector<std::vector<R2D>> obstacles;
};

}
