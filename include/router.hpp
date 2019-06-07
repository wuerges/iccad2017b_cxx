#pragma once

#include <base.hpp>
#include <algo.hpp>
#include <mst.hpp>
#include <memory>
#include <iostream>
#include <vector>
#include <map>
#include <set>

namespace iccad {

    using Route = std::vector<PT>;
    using std::vector, std::tuple, std::set;

    ostream & print2D(ostream &out, const PT & p) {
        return out << "(" << p.x << "," << p.y << ")";        
    }

    ostream & operator<<(ostream &out, const Route & r) {
        for(int i = 1; i < r.size(); ++i) {
            const PT & a = r[i-1], & b = r[i];
            int layer = 0;
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
                out << "Via V" << layer  << " ";
                print2D(out, a);
                out << '\n';
            }
            else {
                std::cerr << "Route not rectangular\n";
                std::cerr << a << " " << b << '\n';
            }
        }
        return out;
    }

    struct AStar {

        using index = tuple<int, int, int>;

        const Treap & shapes, & obstacles;
        vector<int> xs;
        vector<int> ys;
        vector<int> zs;

        AStar(const Treap & sh, const Treap & obs, const Shape & s1, const Shape & s2)
        :shapes(sh), obstacles(obs) {
            add_shape(s1);
            add_shape(s2);

            // TODO search for obstacles             
        }

        void add_shape(const Shape & s) {
            xs.push_back(s.a.x);
            xs.push_back(s.b.x);
            ys.push_back(s.a.y);
            ys.push_back(s.b.y);
            zs.push_back(s.a.y);
            zs.push_back(s.b.y);
        }

        vector<index> neighboors(index i) {
            vector<index> result;
            // TODO
            return result;

        }
    };


    struct Router {
        
        Route calculate_route(const Treap & treap, const Shape & s1, const Shape & s2) 
        {
                // TODO


            return Route({s1.a, s2.b});
        }


        void perform_global_routing(const vector<Shape> & shapes) {
            Treap  treap;
            treap.populate(shapes);

            MST mst;
            auto res = mst.run(treap, shapes);

            for(auto [a, b] : res) {
                auto r = calculate_route(treap, a, b);
                std::cout << r ;
            }
        }

    };
}