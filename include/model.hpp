#pragma once

#include <vector>

namespace model 
{

struct R2D {
  int x1, x2, y1, y2;
};

struct P2D {
  int x, y;
};

struct Input {

  int viaCost, spacing, layers, n_shapes, n_vias, n_obst;
  R2D boundary;

  std::vector<std::vector<R2D>> shapes;
  std::vector<std::vector<P2D>> vias;
  std::vector<std::vector<R2D>> obstacles;
};

}
