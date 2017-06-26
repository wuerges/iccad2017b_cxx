#pragma once

namespace model 
{

struct R2D {
  int x1, x2, y1, y2;
};

struct P2D {
  int x, y;
};

struct Input {
  int viaCost, spacing, layers, shapes, vias, obst;
  R2D boundary;

  vector<vector<R2D>> shapes;
  vector<vector<P2D>> vias;
  vector<vector<R2D>> obstacles;
};

}
