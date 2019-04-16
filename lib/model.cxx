#include <model.hpp>

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <utility>
#include <cassert>

using namespace iccad;

bool iccad::operator<(const P3D & p1, const P3D & p2) {
 if(p1.x != p2.x) return p1.x < p2.x;
 if(p1.y != p2.y) return p1.y < p2.y;
 return p1.z < p2.z;
}

void Grid::add_edge(const P3D p1, const P3D p2) {
	grid.insert(p1);
	grid.insert(p2);
}

void Grid::add_shape(int layer, const V1D & v) {
	/*
	 *    v[0],v[1] ----- v[2],v[1]
	 *     |                  |
	 *     |                  |
	 *    v[0],v[3] ----- v[2],v[4]
	 */
	add_edge(P3D{v[0], v[1], layer}, P3D{v[2], v[1], layer});
	add_edge(P3D{v[0], v[1], layer}, P3D{v[0], v[3], layer});
	add_edge(P3D{v[2], v[1], layer}, P3D{v[2], v[4], layer});
	add_edge(P3D{v[0], v[3], layer}, P3D{v[2], v[4], layer});
}

void Grid::add_via(int layer_1, int layer_2, const V1D & v) {
	add_edge(P3D{v[0], v[1], layer_1}, P3D{v[0], v[1], layer_2});
}

void Grid::add_obstacle(int layer, const V1D & v) {
	assert(false && "unimplemented");
	// TODO
}

void iccad::convert(const Input & inp, Grid & g) {
	for(int i = 0; i < inp.shapes.size(); ++i) {
		for(auto & shape : inp.shapes[i]) {
			g.add_shape(i*inp.viaCost, shape);
		}
	}
	for(int i = 0; i < inp.vias.size(); ++i) {
		for(auto & via : inp.vias[i]) {
			g.add_via(i*inp.viaCost,(i+1)*inp.viaCost, via);
		}
	}

	for(int i = 0; i < inp.obstacles.size(); ++i) {
		for(auto & obs : inp.obstacles[i]) {
			// TODO this is wrong!
			g.add_shape(i*inp.viaCost, obs);
		}
	}
}

std::vector<Shape> iccad::get_routed_shapes(const Input & inp) {
  std::vector<Shape> shapes;

  for (int i = 0; i < inp.shapes.size(); i++) {
    int layer = i*inp.viaCost;
      for(auto & v : inp.shapes[i]) {
        shapes.push_back(Shape(PT(v[0], v[1], layer), PT(v[2], v[3], layer)));
      }

      // TODO add vias
  }

  return shapes;
}
