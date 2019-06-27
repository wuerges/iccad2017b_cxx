#include <router.hpp>
#include <astar.hpp>
#include <kappamst.hpp>

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
            pt.z = z_to_layer(pt.z, viaCost);
        }            
    }

    Route Router::calculate_route(const Shape & s1, const Shape & s2) 
    {
        auto pts = AStar(treap, obstacles, s1, s2, boundary).run();
        Route res(pts);
        res.simplify();
        return res;
    }


    int Router::perform_global_routing(const vector<Shape> & shapes, 
        const vector<Shape> & obs, 
        ostream & out) {
        treap.populate(shapes);
        obstacles.populate(obs);

        int result = 0;

        // for(auto s : shapes) std::cout << s << '\n';

        MST mst(num_neighboors);
        auto res = mst.run(treap, obstacles, shapes, boundary);

        // auto res = KMST(treap, obstacles, shapes, boundary).run();

        for(auto [a, b] : res) {
            auto r = calculate_route(a, b);

            result += r.length();
            
            map_z_to_layer(r);
            out << r ;
        }
        return result;
    }
}