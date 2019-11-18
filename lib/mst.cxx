#include <astar.hpp>
#include <mst.hpp>
#include <config.hpp>

#include <set>
#include <map>
#include <memory>

namespace iccad {
using std::vector, std::pair, std::unordered_map, std::make_optional, std::unique_ptr, std::make_pair, std::make_unique;

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
        obstacles.visit(window, [&](const Shape * xx) {
            obstacles2.add(*xx);
            return true;
        });
        treap.visit(window, [&](const Shape * xx) {
            treap2.add(*xx);
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

vector<Route> MST::run_radius_2(const Treap &treap, const Treap &obstacles,
                                    const vector<Shape> &shapes, const vector<Shape> &obs_vector,
                                    const V1D &boundary) {
                                    
                                      
  vector<Route> result;
  using std::set, std::tuple, std::get;
  auto astar = CONFIG_FAST_ASTAR ? std::nullopt : make_optional(AStar (treap, obstacles, shapes, obs_vector, boundary));
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

struct Edge {
    const Shape* u, *v;
    const unique_ptr<RTreeQueue> queue;
    unique_ptr<Route> route;
    int step;

    Edge(const Shape *s1, const Shape * s2): u(s1), v(s2), step(0) {}    
    Edge(const Shape & c, const RTree & t): queue(new RTreeQueue(c, t)) {}
};

vector<Route> MST::run_iterative(const Treap & treap, 
  const Treap & obstacles,
  const vector<Shape> & shapes, 
  const vector<Shape> & obs_vector, 
  const V1D & boundary) {

    std::multimap<int, unique_ptr<Edge>> krusk;
    int connected = 0;
    MUF<const Shape*> muf;
    vector<Route> result;
    
    /*
    Connect adjacent shapes
    */
    for (const Shape &u : shapes) {

        treap.visit(u, [&](const Shape * v) {
            if (muf.Find(&u) != muf.Find(v)) {
                muf.Union(&u, v);
                connected++;
            }
            return true;
        });
    }

    /*
    Initializing queues
    */
    for(const Shape &o : shapes) {
        unique_ptr<Edge> e(new Edge(o, treap));
        krusk.emplace(e->queue->peek(), std::move(e));
    }

    while (connected < shapes.size() - 1) {
        printf("while %d/%d\n", connected, shapes.size()-1);
        auto work = std::move(krusk.begin()->second);
        krusk.erase(krusk.begin());

        if(work->queue) {
            printf("poping new edge\n");
            // this is a rtree queue
            auto u = work->queue->center;
            auto v = work->queue->pop();
            while(distance(u, *v) == 0 || muf.Find(&u) == muf.Find(v)) {
                v = work->queue->pop();
            }
            krusk.emplace(distance(u, *v), new Edge(&u, v));
            krusk.emplace(work->queue->peek(), std::move(work));
        }        
        else if(muf.Find(work->u) != muf.Find(work->v)) {
            printf("regular edge\n");
            // this is a regular edge
            
            if (work->step == 0) {
                printf("step == 0\n");
                auto rt = AStar(treap, obstacles, *work->u, *work->v, boundary).run(*work->u, *work->v);                
                work->route = make_unique<Route>(std::move(rt));
                work->step++;
                printf("going to emplace: %d %d %d\n", work->route->length(), work->step, 10);
                krusk.emplace(work->route->length(), std::move(work));
            }
            else {
                printf("step > 0\n");
            // the route has already been calculated
                muf.Union(work->u, work->v);
                result.push_back(*work->route);
                connected++;
            }            
        }
    }

    printf("reached result\n");
    return result;
}


} // namespace iccad
