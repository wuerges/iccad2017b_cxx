#pragma once

#include <base.hpp>
#include <algo.hpp>
#include <model.hpp>
#include <muf.hpp>

#include <vector>
#include <unordered_map>

namespace iccad {
  using std::vector, std::pair, std::tuple, std::unordered_map;
  using link = pair<int, Shape>;


  struct KMST {

      const Treap & treap;
      const Treap & obstacles;
      const vector<Shape> & shapes; 
      const V1D & boundary;

    KMST (const Treap & treap, const Treap & obstacles,
      const vector<Shape> & shapes, const V1D & boundary) ;


    vector<pair<Shape, Shape>> run() ;

private:
    unordered_map<Shape, vector<link>> adj;
    unordered_map<Shape, int> adj_i;
    unordered_map<Shape, int> radius;
    
    MUF muf;

    tuple<bool, int, Shape> fetch_next(const Shape & u);
    int calc_dist(const Shape & u, const Shape & v);
  };


}
