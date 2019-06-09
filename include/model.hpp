#pragma once

#include <vector>
#include <set>
#include <utility>
#include <base.hpp>

namespace iccad
{

/**
 * Represents either a point, with size 2
 * or a rectangle with size 4.
 */
using V1D = std::vector<int>;

struct Input {

  int viaCost, spacing, layers, n_shapes, n_vias, n_obst;
  V1D boundary;

  std::vector<std::vector<V1D>> shapes;
  std::vector<std::vector<V1D>> vias;
  std::vector<std::vector<V1D>> obstacles;
};

struct P3D {
  int x,y,z;
};
bool operator<(const P3D & p1, const P3D & p2);

/**
 * Stores the shapes as points
 */
struct Grid {
  // grid is a set with all the points for all the shapes, vias and obstacles
  std::set<P3D> grid;

  // keypoints is a set with all the points that must be connected
  std::set<P3D> keypoints;

  // keypoints is a set with all the hannan points
  std::set<P3D> hannan;

	void add_shape(int layer, const V1D & v);
	void add_via(int l1, int l2, const V1D & v);
	void add_obstacle(int layer, const V1D & v);

private:
	void add_edge(const P3D p1, const P3D p2);
};

/**
 * Stores the shape for windowing query
 */
struct Shapes {

  // This should build a tree-like structure to query the shapes to be routed,
  // or obstacles to be avoided


  void add_shape(int layer, const V1D & v);
  void add_via(int l1, int l2, const V1D & v);
  void add_obstacle(int layer, const V1D & v);
};


int layer_to_z(int i, int viaCost);
int z_to_layer(int y, int viaCost);

void convert(const Input & i, Grid & g);
void generate_hannan(Grid & g);

std::vector<Shape> get_routed_shapes(const Input & i);
std::vector<Shape> get_obstacles(const Input & i);

}
