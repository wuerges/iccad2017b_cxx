#include <astar.hpp>
#include <mst.hpp>
#include <config.hpp>

#include <set>
#include <map>
#include <memory>

namespace iccad {
using std::vector, std::pair, std::unordered_map, std::make_optional, std::unique_ptr, std::make_pair, std::make_unique;

// MST::MST(int n) : num_neighboors(n) {}


Route MST::astar_route(
    const Treap & obstacles, 
    const Treap & treap, 
    const std::optional<AStar> & astar, 
    const Shape & u, 
    const Shape & v,
    const V1D &boundary) {

    Route rt;

    if(CONFIG_FAST_ASTAR) {
        auto a = min(min(u.p1, v.p1), min(u.p2, v.p2));
        auto b = max(max(u.p1, v.p1), max(u.p2, v.p2));

        Shape window{a,b};
        if(ROUTING_WINDOW > 0) {
            window = window.expand(ROUTING_WINDOW);
        }
        Treap obstacles2;
        obstacles.visit(window, [&](const Shape * xx) {
            obstacles2.add(*xx);
            return true;
        });

        rt = AStar(obstacles2, u, v, boundary).run(u, v);          
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

  auto astar = CONFIG_FAST_ASTAR ? std::nullopt : make_optional(AStar (obstacles, shapes, obs_vector, boundary));

  set<tuple<int, Shape, Shape >> edges;
  set<tuple<int, Shape, Shape, Route, bool>> routed_edges;
  
  int connected = 0;

  MUF<Shape> muf;

  for (const Shape &u : shapes) {    
    treap.visit(u, [&](const Shape * v) {
      if (muf.Find(u) != muf.Find(*v)) {
        muf.Union(u, *v);
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

    vector<Shape> vs ;
    RTreeQueue rqueue(u, treap);
    for(int i = 0; i < LOCAL_NEIGHBOORS; ++i) {
      vs.push_back(*rqueue.pop());
    }  

    // vector<Shape> vs = treap.neighboors_diamond(u, num_neighboors);

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
          auto rt2 = AStar(obstacles, u2, v2, boundary).run(u2, v2);
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
                ? AStar( obstacles, u, v, boundary).run(u, v)
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

vector<Route> MST::run_radius_2(const Treap &treap, const Treap &obstacles,
                                    const vector<Shape> &shapes, const vector<Shape> &obs_vector,
                                    const V1D &boundary) {
                                    
                                      
  vector<Route> result;
  using std::set, std::tuple, std::get;
  auto astar = CONFIG_FAST_ASTAR ? std::nullopt : make_optional(AStar (obstacles, shapes, obs_vector, boundary));
  MUF<Shape> muf;


  set<tuple<int, Shape, Shape>> edges;
  set<tuple<int, Shape, Shape, Route, bool>> routed_edges;
  uint64_t radius1 = 0, radius2 = 100;
  int connected = 0;

  /*
  Connect adjacent shapes
   */
  for (const Shape &u : shapes) {    

    treap.visit(u, [&](const Shape * v) {
      if (muf.Find(u) != muf.Find(*v)) {
        muf.Union(u, *v);
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
                treap.visit_diamond_2(u,radius1,radius2, [&](const Shape * v) {
                  edges.insert({distance(u, *v), u, *v});
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
                    auto rt2 = AStar(obstacles, u2, v2, boundary).run(u2, v2);
                    routed_edges.insert({rt.length(), u2, v2, rt2, true});
                }
            }
        
        }
        auto [w, u, v] = *edges.begin();
        edges.erase(edges.begin());

        if (muf.Find(u) != muf.Find(v)) {
            Route rt = 
              CONFIG_2STEP_MST 
                ? AStar(obstacles, u, v, boundary).run(u, v)
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


static int total_routes = 0;
static int used_simple_route = 0;
static int astar_was_needed = 0;

unique_ptr<Route> local_route_step_2(
    const Treap & obstacles, 
    const Shape & u, 
    const Shape & v,
    const V1D &boundary) {
  
  auto window = minimumBound(u, v);
  if(ROUTING_WINDOW > 0) {
      window = window.expand(ROUTING_WINDOW);
  }
  Treap obstacles2, treap2;
  obstacles.visit(window, [&](const Shape * xx) {
      obstacles2.add(*xx);
      return true;
  });
  return make_unique<Route>(AStar(obstacles2, u, v, boundary).run(u, v));
}

unique_ptr<Route> local_route(
    const Treap & obstacles, 
    const Shape & u, 
    const Shape & v,
    const V1D &boundary) {
  return make_unique<Route>(AStar(obstacles, u, v, boundary).run(u, v));
}



template<> 
vector<Route> MST::run_mst<LOCAL_MST>(
  // const Treap & treap, const Treap & obstacles,
  const vector<Shape> & shapes,  const vector<Shape> &obs, const V1D & boundary) {
    RTree treap, obstacles;
    treap.populate(shapes);
    obstacles.populate(obs);
    return run(treap, obstacles, shapes, obs, boundary);
  }

template<> 
vector<Route> MST::run_mst<GLOBAL_MST>(
  // const Treap & treap, const Treap & obstacles,
  const vector<Shape> & shapes,  const vector<Shape> &obs, const V1D & boundary) {
    RTree treap, obstacles;
    treap.populate(shapes);
    obstacles.populate(obs);
    return run_radius_2(treap, obstacles, shapes, obs, boundary);
  }

template<> 
vector<Route> MST::run_mst<ITERATIVE_MST>(
  // const Treap & treap, const Treap & obstacles,
  const vector<Shape> & shapes,  const vector<Shape> &obs, const V1D & boundary) {
    RTree treap, obstacles;
    treap.populate(shapes);
    obstacles.populate(obs);
    return run_iterative(treap, obstacles, shapes, obs, boundary);
  }

template<> 
vector<Route> MST::run_mst<ITERATIVE_DOUBLE_QUEUE>(
  // const Treap & treap, 
  // const Treap & obstacles,
  const vector<Shape> & shapes,  
  const vector<Shape> &obs, 
  const V1D & boundary
  ) {
    RTree treap, obstacles;
    treap.populate(shapes);
    obstacles.populate(obs);

    using Edge_t = tuple<const Shape*, const Shape*, unique_ptr<Route>>;
    using Vert_t = pair<const Shape*, unique_ptr<RTreeQueue>>;

    std::multimap<int, Vert_t> vert_q;
    std::multimap<int, Edge_t> edge_q;
    int connected = 0;
    MUF<Shape> muf;
    vector<Route> result;

    /*
    Initializing queues
    */
    for(const Shape &o : shapes) {
        unique_ptr<RTreeQueue> e(new RTreeQueue(o, treap));
        int rem = 0;
        while(!e->empty() && e->peek() == 0) {
          auto v = e->pop();
          if (muf.Find(o) != muf.Find(*v)) {
            muf.Union(o, *v);
            connected++;
          }
          rem++;
        }

        for(int i = 0; i < LOCAL_NEIGHBOORS && !e->empty(); ++i) {
          const Shape *  v = e->pop();
          edge_q.emplace(distance(o, *v), Edge_t(&o, v, nullptr));
        }
        int new_key = e->peek();
        vert_q.emplace(new_key, Vert_t(&o, std::move(e)));
    }

    while (connected < shapes.size() - 1) {

      if (!vert_q.empty() && vert_q.begin()->first < edge_q.begin()->first) {
        auto vert = std::move(vert_q.begin()->second);
        vert_q.erase(vert_q.begin());

        const Shape *  u = vert.first;
        const Shape *  v = vert.second->pop();

        int new_key = vert.second->peek();
        vert_q.emplace(new_key, std::move(vert));

        const Shape &mu = muf.Find(*u);
        const Shape &mv = muf.Find(*v);
        if (mu != mv) {
          auto route = local_route(obstacles, *u, *v, boundary);
          auto dist = distance(*u, *v);
          if (route->length() <= dist) {
            muf.Union(*u, *v);
            result.push_back(*route);            
          }
          else {
            int new_len = route->length();
            edge_q.emplace(new_len, Edge_t(u, v, std::move(route)));
          }
        }
      }
      else {
        auto [u, v, route] = std::move(edge_q.begin()->second);
        edge_q.erase(edge_q.begin());

        const Shape &mu = muf.Find(*u);
        const Shape &mv = muf.Find(*v);
        if (mu != mv) {
          if(route) {
            muf.Union(*u, *v);
            connected++;
            result.push_back(*route);            
          }
          else {
            auto route = local_route(obstacles, *u, *v, boundary);
            auto dist = distance(*u, *v);
            if (route->length() <= dist) {
              muf.Union(*u, *v);
              connected++;
              result.push_back(*route);            
            }
            else {
              int new_len = route->length();
              edge_q.emplace(new_len, Edge_t(u, v, std::move(route)));
            } 
          }
        }        
      }
    }

    std::cout << "TOTAL ROUTES=" << total_routes << '\n';
    std::cout << "USED SIMPLE ROUTE=" << used_simple_route << '\n';
    std::cout << "ASTAR WAS NEEDED=" << astar_was_needed << '\n';

    return result;
}




} // namespace iccad
