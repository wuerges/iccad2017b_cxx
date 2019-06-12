#pragma once

#include <base.hpp>
#include <algo.hpp>
#include <model.hpp>
#include <vector>
#include <map>

namespace iccad {
  using std::vector, std::pair, std::map;

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
    vector<pair<Shape, Shape>> run(const Treap & treap, const Treap & obstacles,
      const vector<Shape> & shapes, const V1D & boundary) ;

    /*!
     */
    vector<pair<Shape, Shape>> bad_run(const Treap & treap, 
      const vector<Shape> & shapes);

private:
    int num_neighboors;

    map<Shape, Shape> parent;
    map<Shape, int> rank;

    Shape Find(const Shape & x) ;
    void Union(const Shape & x, const Shape & y);
  };


}
