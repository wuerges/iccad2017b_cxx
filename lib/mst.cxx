#include <astar.hpp>
#include <mst.hpp>
#include <config.hpp>

#include <set>

namespace iccad {
using std::vector, std::pair, std::unordered_map, std::make_optional;

MST::MST(int n) : num_neighboors(n) {}


vector<Route> MST::run(const Treap &treap, const Treap &obstacles,
                                    const vector<Shape> &shapes, const vector<Shape> & obs_vector, 
                                    const V1D &boundary) {
  vector<Route> result;
  using std::set, std::tuple, std::optional, std::get;
  const int64_t INF = 1e8;

  auto astar = CONFIG_FAST_ASTAR ? std::nullopt : make_optional(AStar (treap, obstacles, shapes, obs_vector, boundary));

  set<tuple<int, Shape, Shape >> edges;
  set<tuple<int, Shape, Shape, Route>> routed_edges;
  
  int connected = 0;

  for (const Shape &u : shapes) {
    vector<Shape> vs = treap.collect(u.a, u.b);
    for (auto v : vs) {
      if (muf.Find(u) != muf.Find(v)) {
        muf.Union(u, v);
        connected++;
        if(connected == shapes.size() - 1) {
          return result;
        }
      }
    }
  }

  for (const Shape &u : shapes) {
    // vector<Shape> vs = treap.neighboors(u, num_neighboors);
    // vector<Shape> vs = treap.neighboors_sphere(u, num_neighboors);
    vector<Shape> vs = treap.neighboors_diamond(u, num_neighboors);

    for (Shape &v : vs) {
      if (distance(u, v) > 0) {
        edges.insert({distance(u, v), u, v});
      }
    }
  }
  // sort_by_distance(edges);

  while (!edges.empty()) {
    int wt = edges.empty() ? INF : get<0>(*edges.begin());


    while(!routed_edges.empty() && get<0>(*routed_edges.begin()) <= wt) {
      auto [_, u2, v2, rt] = *routed_edges.begin();
      routed_edges.erase(routed_edges.begin());
      if(muf.Find(u2) != muf.Find(v2)) {
        muf.Union(u2, v2);
        result.push_back(rt);
        connected++;        

        if(connected == shapes.size() - 1) return result;
      }
    }

    auto [w, u, v] = *edges.begin();
    edges.erase(edges.begin());

    // std::cout << "early break?: "<< connected << " " << result.size() << " <= " << shapes.size() << "\n";
    

    if (muf.Find(u) != muf.Find(v)) {
      auto a = min(min(u.a, v.a), min(u.b, v.b));
      auto b = max(max(u.a, v.a), max(u.b, v.b));

      Route rt;
      if(CONFIG_FAST_ASTAR) {
        // Treap obstacles2, treap2;
        // obstacles2.populate(obstacles.collect(a, b));
        // treap2.populate(treap.collect(a, b));
        rt = AStar(treap, obstacles, u, v, boundary).run(u, v);
        // new_d = rt.length();
      }
      else {
        rt = astar->run(u, v);
        // new_d = rt.length();
      }
      rt.simplify();

      if (obstacles.query(a, b) > 0) {
        // int new_d;
        // int new_d = AStar(treap, obstacles, u, v, boundary).run().length();
        routed_edges.insert({rt.length(), u, v, rt});
      } else {
        muf.Union(u, v);
        // if(distance(u, v) > 0)
        // std::cout << "added to result\n";
        connected++;
        result.push_back(rt);
        if(connected == shapes.size() - 1) return result;
      }
    }
  }

  //

  return result;
}

vector<Route> MST::run_radius_2(const Treap &treap, const Treap &obstacles,
                                    const vector<Shape> &shapes, const vector<Shape> &obs_vector,
                                    const V1D &boundary) {
                                    
                                      
  vector<Route> result;
  using std::set, std::tuple, std::get;
  auto astar = CONFIG_FAST_ASTAR ? std::nullopt : make_optional(AStar (treap, obstacles, shapes, obs_vector, boundary));


  set<tuple<int, Shape, Shape>> edges;
  set<tuple<int, Shape, Shape, Route>> routed_edges;
  int64_t a = 0, b = 20;
  int connected = 0;

  /*
  Connect adjacent shapes
   */
  for (const Shape &u : shapes) {
    vector<Shape> vs = treap.collect(u.a, u.b);
    for (auto v : vs) {
      if (muf.Find(u) != muf.Find(v)) {
        muf.Union(u, v);
        connected++;
      }
    }
  }
  const int64_t INF = 1e8;

  while (connected < shapes.size() - 1) {
    // std::cout << "connected: " << connected << "/" << shapes.size() << " a=" << a << " b=" << b<< '\n';

    while (edges.empty() && b < INF) {
      for(auto u : shapes) {
        for(auto v : treap.collect_diamond_2(u, a, b)) {
          edges.insert({distance(u, v), u, v});
        }
      }
      a = b;
      b *= 2;
    }
    // if(edges.empty()) {
    //   return result;
    // }
    
    int wt = edges.empty() ? INF : get<0>(*edges.begin());

    while(!routed_edges.empty() && get<0>(*routed_edges.begin()) <= wt) {
      auto [_, u2, v2, rt] = *routed_edges.begin();
      routed_edges.erase(routed_edges.begin());
      if(muf.Find(u2) != muf.Find(v2)) {
        muf.Union(u2, v2);
        result.push_back(rt);
        connected++;        

        if(connected == shapes.size() - 1) return result;
      }
    
    }
    auto [w, u, v] = *edges.begin();
    edges.erase(edges.begin());

    if (muf.Find(u) != muf.Find(v)) {
      auto a = min(min(u.a, v.a), min(u.b, v.b));
      // a.x-=100;
      // a.y-=100;
      auto b = max(max(u.a, v.a), max(u.b, v.b));
      // b.x+=100;
      // b.y+=100;

      Route rt;

      if(CONFIG_FAST_ASTAR) {
        // Treap obstacles2, treap2;
        // obstacles2.populate(obstacles.collect(a, b));
        // treap2.populate(treap.collect(a, b));
        rt = AStar(treap, obstacles, u, v, boundary).run(u, v);          
      }
      else {
        rt = astar->run(u, v);
      }
      rt.simplify();
      // int new_d = AStar(treap, obstacles, u, v, boundary).run(u, v).length();

      if (obstacles.query(a, b) > 0) {
        routed_edges.insert({rt.length(), u, v, rt});
      } else {
        muf.Union(u, v);
        connected++;
        result.push_back(rt);
        if(connected == shapes.size() - 1) return result;
      }
    }
  }

  //

  return result;
}

} // namespace iccad
