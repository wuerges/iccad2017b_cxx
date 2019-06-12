#pragma once

#include <algo.hpp>
#include <base.hpp>
#include <model.hpp>

#include <tuple>
#include <vector>

namespace iccad {

    using std::tuple, std::vector;

struct AStar {

        using index = tuple<int, int, int>;

        Treap & shapes, & obstacles;
        V1D boundary;
        vector<int> xs, ys, zs;

        AStar(Treap & sh, Treap & obs, const Shape & s1, const Shape & s2, V1D b);



        vector<PT> run(const Shape & s, const Shape & t) ;


private:
        vector<index> neighboors(index i);
        const PT make_pt(index i);
        int find(int c, const vector<int> & v) const ;
        index find(const PT p) const;

        void remove_duplicates(vector<int> & v) ;
        void fix_boundaries(vector<int> & v, int min_bound, int max_bound);
        void add_shape(const Shape & s) ;
        vector<PT> bad_run(const PT s, const Shape & ts) ;
        vector<PT> run1(const Shape & shape_s, const Shape & shape_t) ;
    };

}