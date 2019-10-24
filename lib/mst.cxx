#include <astar.hpp>
#include <mst.hpp>
#include <config.hpp>

#include <set>

namespace iccad {
using std::vector, std::pair, std::unordered_map, std::make_optional;

MST::MST(int n) : num_neighboors(n) {}


Route MST::astar_route(
    const Treap & obstacles, 
    const Treap & treap, 
    const std::optional<AStar> & astar, 
    const Shape & u, 
    const Shape & v,
    const V1D &boundary) {

    Route rt;

    if(CONFIG_FAST_ASTAR) {
        auto a = min(min(u.a, v.a), min(u.b, v.b));
        auto b = max(max(u.a, v.a), max(u.b, v.b));

        Shape window{a,b};
        if(ROUTING_WINDOW > 0) {
            window = window.expand(ROUTING_WINDOW);
        }
        Treap obstacles2, treap2;
        obstacles.visit(window, [&](auto & xx) {
            obstacles2.add(xx);
            return true;
        });
        treap.visit(window, [&](auto & xx) {
            treap2.add(xx);
            return true;
        });

        rt = AStar(treap2, obstacles2, u, v, boundary).run(u, v);          
    }
    else {
        rt = astar->run(u, v);
    }
    rt.simplify();
    return rt;
}


vector<Route> MST::run(const Treap &treap, const Treap &obstacles,
                                    const vector<Shape> &shapes, const vector<Shape> & obs_vector, 
                                    const V1D &boundary) {
  vector<Route> result;
  using std::set, std::tuple, std::optional, std::get;
  const int64_t INF = 1e8;

  auto astar = CONFIG_FAST_ASTAR ? std::nullopt : make_optional(AStar (treap, obstacles, shapes, obs_vector, boundary));

  set<tuple<int, Shape, Shape >> edges;
  set<tuple<int, Shape, Shape, Route, bool>> routed_edges;
  
  int connected = 0;

  for (const Shape &u : shapes) {    
    treap.visit(u, [&](auto & v) {
      if (muf.Find(u) != muf.Find(v)) {
        muf.Union(u, v);
        connected++;
      }
      return true;
    });
  }

  if(connected >= shapes.size() -1 ) {
    return result;
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
      auto [_, u2, v2, rt, calc] = *routed_edges.begin();
      routed_edges.erase(routed_edges.begin());
      if(muf.Find(u2) != muf.Find(v2)) {
        if(calc) {
          muf.Union(u2, v2);
          result.push_back(rt);
          connected++;        
          if(connected == shapes.size() - 1) return result;
        }
        else {
          auto rt2 = AStar(treap, obstacles, u2, v2, boundary).run(u2, v2);
          routed_edges.insert({rt.length(), u2, v2, rt2, true});
        }
      }
    }

    auto [w, u, v] = *edges.begin();
    edges.erase(edges.begin());

    // std::cout << "early break?: "<< connected << " " << result.size() << " <= " << shapes.size() << "\n";
    

    if (muf.Find(u) != muf.Find(v)) {
        Route rt = 
              CONFIG_2STEP_MST 
                ? AStar(treap, obstacles, u, v, boundary).run(u, v)
                : astar_route(obstacles, treap, astar, u, v, boundary);
       
        if(rt.length() > w) {
            auto rx = CONFIG_2STEP_MST ? rt : Route();
            routed_edges.insert({rt.length(), u, v, rx, CONFIG_2STEP_MST});        
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

vector<Route> MST::run_radius_3(const Treap &treap, const Treap &obstacles,
                                    const vector<Shape> &shapes, const vector<Shape> &obs_vector,
                                    const V1D &boundary) {
                                    
                                      
  vector<Route> result;
  using std::set, std::tuple, std::get;
  auto astar = CONFIG_FAST_ASTAR ? std::nullopt : make_optional(AStar (treap, obstacles, shapes, obs_vector, boundary));


  set<tuple<int, Shape, Shape>> edges;
  set<tuple<int, Shape, Shape, Route, bool>> routed_edges;
  uint64_t radius1 = 0, radius2 = 100;
  int connected = 0;

  /*
  Connect adjacent shapes
   */
  for (const Shape &u : shapes) {    

    treap.visit(u, [&](auto & v) {
      if (muf.Find(u) != muf.Find(v)) {
        muf.Union(u, v);
        connected++;
      }
      return true;
    });
  }
  const uint64_t INF = 1e8;

  while (connected < shapes.size() - 1) {
        // std::cout << "connected: " << connected << "/" << shapes.size() << " a=" << a << " b=" << b<< '\n';

        while (edges.empty() && radius2 < INF) {
            for(auto u : shapes) {
                // for(auto v : treap.collect_diamond_2(u, a, b)) {
                //   edges.insert({distance(u, v), u, v});
                // }
                treap.visit_diamond_2(u,radius1,radius2, [&](auto & v) {
                    edges.insert({distance(u, v), u, v});
                    return true;
                });
            }
            radius1 = radius2;
            radius2 *= 2;
        }
        // if(edges.empty()) {
        //   return result;
        // }
        
        int wt = edges.empty() ? INF : get<0>(*edges.begin());

        while(!routed_edges.empty() && get<0>(*routed_edges.begin()) <= wt) {
            auto [_, u2, v2, rt, calc] = *routed_edges.begin();
            routed_edges.erase(routed_edges.begin());
            if(muf.Find(u2) != muf.Find(v2)) {
                if(calc) {
                    muf.Union(u2, v2);
                    result.push_back(rt);
                    connected++;        
                    if(connected == shapes.size() - 1) return result;
                }
                else {
                    auto rt2 = AStar(treap, obstacles, u2, v2, boundary).run(u2, v2);
                    routed_edges.insert({rt.length(), u2, v2, rt2, true});
                }
            }
        
        }
        auto [w, u, v] = *edges.begin();
        edges.erase(edges.begin());

        if (muf.Find(u) != muf.Find(v)) {
            Route rt = 
              CONFIG_2STEP_MST 
                ? AStar(treap, obstacles, u, v, boundary).run(u, v)
                : astar_route(obstacles, treap, astar, u, v, boundary);

            if(rt.length() > w) {
            // if (obstacles.hits(a, b)) {
                auto rx = CONFIG_2STEP_MST ? rt : Route();
                routed_edges.insert({rt.length(), u, v, rx, CONFIG_2STEP_MST});
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

vector<Route> MST::run_radius_2(const Treap &treap, const Treap &obstacles,
                                    const vector<Shape> &shapes, const vector<Shape> &obs_vector,
                                    const V1D &boundary) {
                                    
                                      
    vector<Route> result;
    using std::set, std::tuple, std::get;
    auto astar = CONFIG_FAST_ASTAR ? std::nullopt : make_optional(AStar (treap, obstacles, shapes, obs_vector, boundary));

    // using item = tuple<int, const Shape, const Shape, const Route, int>;
    using item = tuple<int, const Shape, const Shape, const Route, int>;

    set<item> edges;
    uint64_t radius1 = 0, radius2 = 100;
    int connected = 0;

    /*
    Connect adjacent shapes
    */
    for (const Shape &u : shapes) {    

        treap.visit(u, [&](auto & v) {
            if (muf.Find(u) != muf.Find(v)) {
                muf.Union(u, v);
                connected++;
            }
            return true;
        });
    }
    const uint64_t INF = 1e8;

    const Route dummy;

    int frontier = 0;

    while (connected < shapes.size() - 1) {
        if(radius2 > INF) {
            std::cout << "radius2="<<radius2<< '\n';
        }
        ASSERT(radius2 < INF);

        while(edges.empty() || frontier > radius2 ) {
            for(auto & u : shapes) {
                treap.visit_diamond_2(u,radius1,radius2, [&](auto & v) {                    
                    edges.insert(item{distance(u, v), u, v, dummy, 0});
                    return true;
                });
            }
            radius1 = radius2;
            radius2 *= 2;
        }

        auto [_, u, v, rt, calc] = *edges.begin();
        edges.erase(edges.begin());

        if(muf.Find(u) != muf.Find(v)) {

            if(calc == 2) {
                muf.Union(u, v);
                result.push_back(rt);
                connected++;        
                if(connected >= shapes.size() - 1) return result;
            }
            else if(calc == 1) {
                auto rt3 = AStar(treap, obstacles, u, v, boundary).run(u, v);
                frontier = std::max(frontier, rt3.length());
                edges.insert({rt3.length(), u, v, rt3, 2});
            }
            else {
                if(CONFIG_2STEP_MST) {
                    auto rt3 = AStar(treap, obstacles, u, v, boundary).run(u, v);
                    frontier = std::max(frontier, rt3.length());
                    edges.insert({rt3.length(), u, v, rt3, 2});
                }
                else {
                    auto rt2 = astar_route(obstacles, treap, astar, u, v, boundary);
                    frontier = std::max(frontier, rt2.length());
                    edges.insert({rt2.length(), u, v, dummy, 1});
                }
            }
        }        
    }

    return result;
}

} // namespace iccad
