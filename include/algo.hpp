#pragma once

#include <base.hpp>
#include <memory>
#include <iostream>

namespace iccad {
    struct Node {
        Node(const Shape _x):
          x(_x), low(_x.a), high(_x.b), count(1)
          {}


        const Shape x;
        PT low, high;
        int count;

        std::unique_ptr<Node> left, right;

        void add(const Shape & shape, int level=0);
        int query(const PT l, const PT r, int level = 0);
        void print(int h=0, int level=0);
    };

}
