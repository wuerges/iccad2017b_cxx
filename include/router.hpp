#pragma once

#include <astar.hpp>
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