#include <router.hpp>
#include <astar.hpp>

#include <algorithm>

namespace iccad {

    void simplify(Route & r) {
        for(int i = 2; i < r.size(); ++i) {
            PT & a = r[i-2], & b = r[i-1], & c = r[i];
            
            if(a.x == b.x && b.x == c.x && a.z == b.z && b.z == c.z) {
                b.y = a.y;
            }
            if(a.y == b.y && b.y == c.y && a.z == b.z && b.z == c.z) {
                b.x = a.x;
            }
            if(a.x == b.x && b.x == c.x && a.y == b.y && b.y == c.y) {
                b.z = a.z;
            }
        }
        r.erase(unique(r.begin(), r.end()), r.end());
    }

    ostream & print2D(ostream &out, const PT & p) {
        return out << "(" << p.x << "," << p.y << ")";        
    }

    ostream & operator<<(ostream &out, const Route & r) {
        for(int i = 1; i < r.size(); ++i) {
            const PT & a = r[i-1], & b = r[i];
            int layer = std::min(a.z, b.z);
            if(a.y == b.y && a.z == b.z) {
                out << "H-line M" << layer << " ";
                print2D(out, a);
                out << " ";
                print2D(out, b);
                out << '\n';
            }
            else if(a.x == b.x && a.z == b.z) {
                out << "V-line M" << layer << " ";
                print2D(out, a);
                out << " ";
                print2D(out, b);
                out << '\n';
            }
            else if(a.x == b.x && a.y == b.y) {
                int beg = std::min(a.z, b.z);
                int end = std::max(a.z, b.z);
                for(int zi = beg; zi < end; ++zi) {
                    out << "Via V" << zi  << " ";
                    print2D(out, a);
                    out << '\n';
                }
            }
            else {
                std::cerr << "Route not rectangular\n";
                std::cerr << a << " " << b << '\n';
            }
        }
        return out;
    }

    Router::Router(int sp, int vc, V1D b, int n):spacing(sp), viaCost(vc)
        , boundary(b), num_neighboors(n) {
            boundary[0] += spacing;
            boundary[1] += spacing;
            boundary[2] -= spacing;
            boundary[3] -= spacing;
        }
    

    void Router::map_z_to_layer(Route & pts) {
        for(auto & pt : pts) {
            pt.z = z_to_layer(pt.z, viaCost);
        }            
    }

    Route Router::calculate_route(const Shape & s1, const Shape & s2) 
    {
        auto pts = AStar(treap, obstacles, s1, s2, boundary).run();
        Route res(pts);
        simplify(res);
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
        auto res = mst.run(treap, obstacles, shapes);

        for(auto [a, b] : res) {
            auto r = calculate_route(a, b);

            for(int i = 1; i < r.size();++i) {
                result += manhatan(r[i], r[i-1]);
            }
            
            map_z_to_layer(r);
            out << r ;
        }
        return result;
    }
}