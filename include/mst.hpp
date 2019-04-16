#pragma once

#include <base.hpp>
#include <algo.hpp>
#include <memory>
#include <iostream>
#include <vector>

namespace iccad {

  void sort_by_distance(vector<pair<Shape, Shape>> & edges) {}

    struct MST {

      map<Shape, Shape> parent;
      map<Shape, int> rank;

      Shape Find(const Shape & x) {
        if(parent[x] == x) {
          return x;
        }
        return parent[x] = Find(parent[x]);
      }

      void Union(const Shape & x, const Shape & y) {
        Shape xr = Find(x);
        Shape yr = Find(y);

        if(rank[xr] > rank[yr]) {
          parent[yr] = xr;
        }
        else if (rank[xr] < rank[yr]){
          parent[xr] = yr;
        }
        else {
          rank[xr]++;
          parent[yr] = xr;
        }
      }

      vector<Shape> shapes;
      vector<pair<Shape, Shape>> result;
      Treap  treap;

      void run() {

        vector<pair<Shape, Shape>> edges;

        for(Shape & u : shapes) {
          vector<Shape> vs = treap.neighboors(u, 10);
          for(Shape & v : vs) {
            edges.push_back(u, v);
          }
        }
        sort_by_distance(edges);

        for(auto & [u,v] : edges) {
          if(Find(u) != Find(v)) {
            Union(u, v);
            result.push_back({u, v});
          }
        }
      }
    };


}
