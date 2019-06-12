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

    struct AStar {

        using index = tuple<int, int, int>;

        Treap & shapes, & obstacles;
        V1D boundary;
        vector<int> xs, ys, zs;

        AStar(Treap & sh, Treap & obs, const Shape & s1, const Shape & s2, V1D b);

        void add_shape(const Shape & s) ;

        vector<index> neighboors(index i);

        const PT make_pt(index i);

        int find(int c, const vector<int> & v) const ;

        index find(const PT p) const;

        void remove_duplicates(vector<int> & v) ;

        void fix_boundaries(vector<int> & v, int min_bound, int max_bound);

        vector<PT> run(const Shape & s, const Shape & t) ;

        vector<PT> bad_run(const PT s, const Shape & ts) ;
        
        vector<PT> run1(const Shape & shape_s, const Shape & shape_t) ;
    };


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