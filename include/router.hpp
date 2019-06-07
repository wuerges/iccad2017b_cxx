#pragma once

#include <base.hpp>
#include <algo.hpp>
#include <mst.hpp>
#include <memory>
#include <iostream>
#include <vector>
#include <map>

namespace iccad {

    using Route = std::vector<PT>;
    using std::vector;

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


    struct Router {
        
        Route calculate_route(const Treap & treap, 
            const Shape & s1, const Shape & s2) {
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