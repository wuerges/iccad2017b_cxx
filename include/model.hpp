#pragma once

#include <vector>
#include <map>
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

typedef std::tuple<unsigned int, unsigned int, unsigned int> P3D;

struct Grid {
	std::map<P3D, std::vector<P3D>> graph;
	void add_shape(unsigned int layer, const V1D & v);
	void add_via(unsigned int l1, unsigned int l2, const V1D & v);
	void add_obstacle(unsigned int layer, const V1D & v);

	void add_edge(const P3D p1, const P3D p2);
};

void convert(const Input & i, Grid & g);

}
