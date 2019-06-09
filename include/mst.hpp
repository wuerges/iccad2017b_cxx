#pragma once

#include <base.hpp>
#include <algo.hpp>
#include <memory>
#include <iostream>
#include <algorithm>
#include <vector>
#include <map>




namespace iccad {
  using std::vector, std::pair, std::map, std::sort;
  void sort_by_distance(vector<pair<Shape, Shape>> & edges) {
    sort(edges.begin(), edges.end(), [&](auto & p1, auto & p2) {
      return distance(p1.first, p1.second) < distance(p2.first, p2.second);
    });
  }

    struct MST {

      map<Shape, Shape> parent;
      map<Shape, int> rank;

      Shape Find(const Shape & x) {
        if(parent.find(x) == parent.end()) {
          parent[x] = x;
        }

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


      vector<pair<Shape, Shape>> run(const Treap & treap, 
        const vector<Shape> & shapes) {
        vector<pair<Shape, Shape>> result;

        vector<pair<Shape, Shape>> edges;

        // for(const Shape & u : shapes) {
        //   vector<Shape> vs = treap.collect(u.a, u.b);
        //   for(auto v : vs) {
        //     Union(u,v);
        //   }
        // }

        for(const Shape & u : shapes) {
          vector<Shape> vs = treap.neighboors(u, 100);

          // vector<pair<Shape, Shape>> n_edges;
  
          for(Shape & v : vs) {
            // n_edges.emplace_back(u, v);
            // sort_by_distance(n_edges);

            // using std::cout;
            // cout << "\n\n\nDistances: \n";

            // for(auto [ue, ve] : n_edges) {
            //   cout << ue << ve << " -> " << distance(ue, ve) << '\n';
            // }


            edges.push_back({u, v});
          }
        }
        sort_by_distance(edges);

        for(auto & [u,v] : edges) {
          if(distance(u, v) == 0) {
            Union(u, v);
          }
          if(Find(u) != Find(v)) {
            Union(u, v);
            // if(distance(u, v) > 0)
              result.push_back({u, v});
          }
        }
        return result;
      }

      vector<pair<Shape, Shape>> bad_run(const Treap & treap, 
        const vector<Shape> & shapes) {

        vector<pair<Shape, Shape>> result;
        
        for(const Shape & u : shapes) {
          vector<Shape> vs = treap.collect(u.a, u.b);
          for(auto v : vs) {
            result.push_back({u,v});

            std::cout << "Distance: " << distance(u, v) << " -> " << u << v << "\n";
          }


        }
        return result;
      }
    };


}
