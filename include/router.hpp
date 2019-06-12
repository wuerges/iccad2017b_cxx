#pragma once

#include <base.hpp>
#include <algo.hpp>
#include <model.hpp>
#include <mst.hpp>
#include <memory>
#include <iostream>
#include <vector>
#include <map>
#include <set>

namespace iccad {

    using Route = std::vector<PT>;
    using std::vector, std::tuple, std::set;
    void simplify(Route & r);
    ostream & print2D(ostream &out, const PT & p);
    ostream & operator<<(ostream &out, const Route & r);    


    struct Router {

        int spacing, viaCost;
        V1D boundary;
        int num_neighboors;
        Treap  treap, obstacles;

        Router(int sp, int vc, V1D b, int n);

        void map_z_to_layer(Route & pts);

        Route calculate_route(const Shape & s1, const Shape & s2) ;


        int perform_global_routing(const vector<Shape> & shapes, 
            const vector<Shape> & obs, 
            ostream & out) ;
    };
}