#pragma once

#include <geometry.hpp>
#include <unordered_map>

namespace iccad {

using std::unordered_map;
using namespace geometry;

template<typename T>
struct MUF {

  unordered_map<T, T> parent;
  unordered_map<T, int> rank;

  T Find(const T & x) ;
  void Union(const T & x, const T & y);
};


template<typename T>
T MUF<T>::Find(const T &x) {
  if (parent.find(x) == parent.end()) {
    parent[x] = x;
  }

  if (parent[x] == x) {
    return x;
  }
  return parent[x] = Find(parent[x]);
}

template<typename T>
void MUF<T>::Union(const T &x, const T &y) {
  T xr = Find(x);
  T yr = Find(y);

  if (rank[xr] > rank[yr]) {
    parent[yr] = xr;
  } else if (rank[xr] < rank[yr]) {
    parent[xr] = yr;
  } else {
    rank[xr]++;
    parent[yr] = xr;
  }
}

}
