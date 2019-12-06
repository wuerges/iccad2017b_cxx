#pragma once


#include <geometry.hpp>
#include <treap.hpp>
#include <model.hpp>
#include <muf.hpp>
#include <astar.hpp>
#include <config.hpp>

#include <vector>
#include <unordered_map>
#include <memory>

namespace iccad {
  using std::vector, std::pair, std::unordered_map;

  struct MST {


    /*!
      Computes the MST, using Kruskall.
      \param n the target number of neighboors to generate adjacency.
     */
    // MST(int n);

    /*!
      \param treap A Treap containing the routed shapes.
      \param obstacles A Treap containing the obstacles.
     */
    vector<Route> run(const Treap & treap, const Treap & obstacles,
      const vector<Shape> & shapes, const vector<Shape> & obs_vector, const V1D & boundary) ;

    vector<Route> run_iterative(const Treap & treap, 
      const Treap & obstacles,
      const vector<Shape> & shapes, 
      const vector<Shape> & obs_vector, 
      const V1D & boundary) ;

    vector<Route> run_radius_2(const Treap & treap, const Treap & obstacles,
      const vector<Shape> & shapes,  const vector<Shape> &obs_vector, const V1D & boundary) ;

    template<int CFG> 
    vector<Route> run_mst(
      // const Treap & treap, const Treap & obstacles,
      const vector<Shape> & shapes,  const vector<Shape> &obs_vector, const V1D & boundary);

    
private:
    // int num_neighboors;

    Route astar_route(
      const Treap & obstacles, 
      const Treap & treap, 
      const std::optional<AStar> & astar, 
      const Shape & u, 
      const Shape & v,
      const V1D &boundary);

  };


std::unique_ptr<Route> local_route(
    const Treap & obstacles, 
    const Shape & u, 
    const Shape & v,
    const V1D &boundary);

std::unique_ptr<Route> local_route_step_2(
    const Treap & obstacles, 
    const Shape & u, 
    const Shape & v,
    const V1D &boundary);


}
