#include <router.hpp>
#include <astar.hpp>
#include <config.hpp>

#include <algorithm>

namespace iccad {

    Router::Router(int sp, int vc, V1D b, int n):spacing(sp), viaCost(vc)
        , boundary(b), num_neighboors(n) {
            boundary[0] += spacing;
            boundary[1] += spacing;
            boundary[2] -= spacing;
            boundary[3] -= spacing;
        }
    

    void Router::map_z_to_layer(Route & pts) {
        for(auto & pt : pts.route) {
            pt.coords[2] = z_to_layer(pt[2], viaCost);
        }            
    }

    Route Router::calculate_route(const Shape & s1, const Shape & s2) 
    {
        auto pts = AStar(treap, obstacles, s1, s2, boundary).run(s1, s2);
        // Route res(pts);
        pts.simplify();
        return pts;
    }


    int Router::perform_global_routing(const vector<Shape> & shapes, 
        const vector<Shape> & obs, 
        ostream & out) {
        treap.populate(shapes);
        obstacles.populate(obs);

        int result = 0;

        // for(auto s : shapes) std::cout << s << '\n';

        MST mst(num_neighboors);

        vector<Route> res = mst.run_mst<CONFIG_MST>(treap, obstacles, shapes, obs, boundary);


        // auto res = mst.run_iterative(treap, obstacles, shapes, obs, boundary);
        // auto res = CONFIG_FAST_MST ? mst.run(treap, obstacles, shapes, obs, boundary): 
        //                              mst.run_radius_2(treap, obstacles, shapes, obs, boundary);

        // auto res = KMST(treap, obstacles, shapes, boundary).run();

        for(auto r : res) {
            // auto r = calculate_route(a, b);

            result += r.length();
            
            map_z_to_layer(r);
            out << r ;
        }
        return result;
    }
}