#include <astar.hpp>
#include <mst.hpp>
#include <config.hpp>

#include <set>
#include <map>
#include <memory>

namespace iccad {
using std::vector, std::pair, std::unordered_map, std::make_optional, std::unique_ptr, std::make_pair, std::make_unique;

struct Edge {
    const Shape* u;
    const Shape* v;
    const unique_ptr<RTreeQueue> queue;
    unique_ptr<Route> route;
    int step;

    Edge(const Shape *s1, const Shape * s2): u(s1), v(s2), queue(nullptr), step(0) {}    
    Edge(const Shape  *c, const RTree & t): u(c), queue(new RTreeQueue(*c, t)) {}

    friend ostream & operator<<(ostream& out, const Edge& e) {
      if(e.queue) {
        out << "EdgeQueue{" << e.queue->center << ","<< e.queue->peek() << "}";
      }
      else {
        out << "Edge{" << *e.u <<","<< *e.v<< "}";
      }
      return out;
    }
};

template<>
vector<Route> MST::run_mst<ITERATIVE_MST_3STEP>(
//     const Treap & treap, 
//   const Treap & obstacles,
  const vector<Shape> & shapes, 
  const vector<Shape> & obs_vector, 
  const V1D & boundary) {
    RTree treap, obstacles;
    treap.populate(shapes);
    obstacles.populate(obs_vector);

    std::multimap<int, unique_ptr<Edge>> krusk;
    int connected = 0;
    MUF<const Shape*> muf;
    vector<Route> result;

    /*
    Initializing queues
    */
    for(const Shape &o : shapes) {
        unique_ptr<Edge> e(new Edge(&o, treap));
        int rem = 0;
        while(!e->queue->empty() && e->queue->peek() == 0) {
          auto v = e->queue->pop();
          if (muf.Find(&o) != muf.Find(v)) {
            muf.Union(&o, v);
            connected++;
          }
          rem++;
        }

        for(int i = 0; i < LOCAL_NEIGHBOORS && !e->queue->empty(); ++i) {
          const auto v = e->queue->pop();
          krusk.emplace(distance(o, *v), new Edge(&o, v));
        }  
        int new_key = e->queue->peek();
        krusk.emplace(new_key, std::move(e));
    }

    while (connected < shapes.size() - 1 && krusk.size() > 0) {
        // std::cout << "---------------------\n";
        // for(auto & [k,ed] :krusk) {
        //   std::cout << "while: krusk[" << k << " = " << *ed << '\n';
        // }
        int old_length = krusk.begin()->first;
        auto work = std::move(krusk.begin()->second);
        krusk.erase(krusk.begin());

        if(work->queue) {
          // printf("work->queue()\n");

          if(!work->queue->empty()) {
            const auto u = work->u;
            const auto v = work->queue->pop();
            int new_key = work->queue->peek();

            const Shape * mu = muf.Find(u);
            const Shape * mv = muf.Find(v);
            if(mu != mv) { 
              // adds new edge
              unique_ptr<Edge> routed(new Edge(u,v));
              routed->route = local_route_step_2(obstacles, *u, *v, boundary);
              routed->step++;

              int route_length = routed->route->length();
              if(route_length == old_length) {
                muf.Union(u, v);
                result.push_back(*routed->route);
                connected++;
              }
              else {
                krusk.emplace(route_length, std::move(routed));
              }
            }

            krusk.emplace(new_key, std::move(work));
          }
        }
        else {
          const Shape *mu = muf.Find(work->u);
          const Shape *mv = muf.Find(work->v);
          const Shape *u = work->u;
          const Shape *v = work->v;
            
          if(mu != mv) {
              if (work->step == 0) {
                  work->route = local_route_step_2(obstacles, *u, *v, boundary);
                  work->step++;
                  int route_length = work->route->length();
                  if(route_length == old_length) {
                    muf.Union(u, v);
                    result.push_back(*work->route);
                    connected++;
                  }
                  else {
                    krusk.emplace(route_length, std::move(work));
                  }

              }
              else {
                  muf.Union(u, v);
                  result.push_back(*work->route);
                  connected++;
              }            
          }
        }
    }

    // std::cout << "TOTAL ROUTES=" << total_routes << '\n';
    // std::cout << "USED SIMPLE ROUTE=" << used_simple_route << '\n';
    // std::cout << "ASTAR WAS NEEDED=" << astar_was_needed << '\n';

    return result;
}



} // namespace iccad
