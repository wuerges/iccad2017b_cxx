#pragma once

#include <base.hpp>
#include <algo.hpp>
#include <memory>
#include <iostream>
#include <vector>
#include <map>

namespace iccad {

    using Route = std::vector<PT>;

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
            }
            else if(a.x == b.x && a.z == b.z) {
                out << "V-line M" << layer << " ";
                print2D(out, a);
                out << " ";
                print2D(out, b);
            }
            else if(a.x == b.x && a.y == b.y) {
                out << "Via V" << layer  << " ";
                print2D(out, a);
            }
            else {
                throw "Route not rectangular";
            }
            return out;
        }
    }


    struct Router {
        const Treap & t;

        Router(const Treap & t):t(t) {}

        Route calculate_route(const Shape & s1, const Shape & s2) {
            return Route();
        }


    };
}