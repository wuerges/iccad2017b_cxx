#pragma once

#include <base.hpp>
#include <memory>
#include <iostream>

namespace iccad {
    using std::unique_ptr, std::make_unique,
          std::min, std::max,
          std::cout;

    struct Node {
        Shape x;
        int low, high, count;

        unique_ptr<Node> left, right;

        void add(const Shape & shape, int level=0) {
            int a = x[0][level % 3];
            int b = x[1][level % 3];
            int l = shape[0][level % 3];
            int r = shape[1][level % 3];
            if(r < a) {
                if(left) {
                    left->add(shape, level+1);
                }
                else {
                    left = make_unique<Node>(Node{l, r, l, r, 1});
                }
                low = min(low, left->low);
            }
            else {
                if(right) {
                    right->add(shape, level+1);
                }
                else {
                    right = make_unique<Node>(Node{l, r, l, r, 1});
                }
                high = max(high, right->high);
            }
            count = 1 + (left?left->count:0) + (right?right->count:0);
        }

        int query(const Shape & shape, int level = 0) {

            int a = x[0][level % 3];
            int b = x[1][level % 3];
            int l = shape[0][level % 3];
            int r = shape[1][level % 3];

            printf("query(%d, %d) -> (%d, %d)\n", l, r, a, b);

            if(l <= low && r >= high) {
                return count;
            }
            if(l > high || r < low) {
                return 0;
            }

            bool hits = (l <= b && r >= b) || (l <= a && r >= a);

            if(hits) { printf("(%d ,%d)\n", a, b); }

            return (hits?1:0) 
                + (left?left->query(shape, level+1):0) 
                + (right?right->query(shape, level+1):0);
        }

        void print(int h=0, int level=0) {
            int a = x[0][level % 3];
            int b = x[1][level % 3];

            if(right) right->print(h+2, level+1);
            for(int i = 0; i < h; ++i) { cout << ' '; }
            printf("{%d %d %d %d %d}\n", a, b, low, high, count);
            if(left) left->print(h+2, level+1);
        }
    };

}
