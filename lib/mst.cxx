#include <astar.hpp>
#include <mst.hpp>

#include <set>

namespace iccad {
using std::vector, std::pair, std::map;

MST::MST(int n) : num_neighboors(n) {}

Shape MST::Find(const Shape &x) {
  if (parent.find(x) == parent.end()) {
    parent[x] = x;
  }

  if (parent[x] == x) {
    return x;
  }
  return parent[x] = Find(parent[x]);
}

void MST::Union(const Shape &x, const Shape &y) {
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

vector<pair<Shape, Shape>> MST::run(const Treap &treap, const Treap &obstacles,
                                    const vector<Shape> &shapes,
                                    const V1D &boundary) {
  vector<pair<Shape, Shape>> result;
  using std::set, std::tuple;

  set<tuple<int, Shape, Shape, bool>> edges;

  int connected = 0;

  for (const Shape &u : shapes) {
    vector<Shape> vs = treap.collect(u.a, u.b);
    for (auto v : vs) {
      if (Find(u) != Find(v)) {
        Union(u, v);
        connected++;
      }
    }
  }

  for (const Shape &u : shapes) {
    // vector<Shape> vs = treap.neighboors(u, num_neighboors);
    // vector<Shape> vs = treap.neighboors_sphere(u, num_neighboors);
    vector<Shape> vs = treap.neighboors_diamond(u, num_neighboors);

    for (Shape &v : vs) {
      if (distance(u, v) > 0) {
        edges.insert({distance(u, v), u, v, false});
      }
    }
  }
  // sort_by_distance(edges);

  while (!edges.empty()) {
    // std::cout << "early break?: "<< connected << " " << result.size() << " <= " << shapes.size() << "\n";
    if(connected == shapes.size() - 1) {
      break;
    }
    auto [w, u, v, calc] = *edges.begin();
    edges.erase(edges.begin());

    if (Find(u) != Find(v)) {
      auto a = min(min(u.a, v.a), min(u.b, v.b));
      auto b = max(max(u.a, v.a), max(u.b, v.b));

      if (!calc && obstacles.query(a, b) > 0) {
        int new_d = AStar(treap, obstacles, u, v, boundary).run().length();
        edges.insert({new_d, u, v, true});
      } else {
        Union(u, v);
        // if(distance(u, v) > 0)
        // std::cout << "added to result\n";
        connected++;
        result.push_back({u, v});
      }
    }
  }

  //

  return result;
}

vector<pair<Shape, Shape>> MST::bad_run(const Treap &treap,
                                        const vector<Shape> &shapes) {

  vector<pair<Shape, Shape>> result;

  for (const Shape &u : shapes) {
    vector<Shape> vs = treap.collect(u.a, u.b);
    for (auto v : vs) {
      result.push_back({u, v});

      std::cout << "Distance: " << distance(u, v) << " -> " << u << v << "\n";
    }
  }
  return result;
}

} // namespace iccad
