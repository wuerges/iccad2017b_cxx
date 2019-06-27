#include <muf.hpp>

#include <unordered_map>

namespace iccad {

using std::unordered_map;

Shape MUF::Find(const Shape &x) {
  if (parent.find(x) == parent.end()) {
    parent[x] = x;
  }

  if (parent[x] == x) {
    return x;
  }
  return parent[x] = Find(parent[x]);
}

void MUF::Union(const Shape &x, const Shape &y) {
  Shape xr = Find(x);
  Shape yr = Find(y);

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
