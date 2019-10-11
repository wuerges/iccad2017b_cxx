#pragma once


#include <geometry.hpp>
#include <treap.hpp>
#include <model.hpp>
#include <muf.hpp>
#include <astar.hpp>

#include <vector>
#include <unordered_map>

namespace iccad {
  using std::vector, std::pair, std::unordered_map;

  struct MST {


    /*!
      Computes the MST, using Kruskall.
      \param n the target number of neighboors to generate adjacency.
     */
    MST(int n);

    /*!
      \param treap A Treap containing the routed shapes.
      \param obstacles A Treap containing the obstacles.
     */
    vector<Route> run(const Treap & treap, const Treap & obstacles,
      const vector<Shape> & shapes, const vector<Shape> & obs_vector, const V1D & boundary) ;
    vector<Route> run_radius_2(const Treap & treap, const Treap & obstacles,
      const vector<Shape> & shapes,  const vector<Shape> &obs_vector, const V1D & boundary) ;


private:
    int num_neighboors;

    MUF muf;

  };


}
