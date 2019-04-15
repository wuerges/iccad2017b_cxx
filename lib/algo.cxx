#include <base.hpp>
#include <algo.hpp>

#include <iostream>

namespace iccad {

  using std::unique_ptr, std::make_unique,
        std::cout;


  void Node::add(const Shape & shape, int level) {
      int a = x.a[level % 3];
      int b = x.b[level % 3];
      int l = shape.a[level % 3];
      int r = shape.b[level % 3];
      if(r < a) {
          if(left) {
              left->add(shape, level+1);
          }
          else {
              left = make_unique<Node>(shape);
          }
      }
      else {
          if(right) {
              right->add(shape, level+1);
          }
          else {
              right = make_unique<Node>(shape);
          }
      }
      if(left) {
          low = min(low, left->low);
          high = max(high, left->high);
      }
      if(right) {
          low = min(low, right->low);
          high = max(high, right->high);
      }
      count = 1 + (left?left->count:0) + (right?right->count:0);
  }

  int Node::query(const PT l, const PT r, int level) {

      //int a = x[0][level % 3];
      //int b = x[1][level % 3];
      //int l = shape[0][level % 3];
      //int r = shape[1][level % 3];

      // for (size_t i = 0; i < level; i++) cout << "  ";
      // cout << "query("<<l<<","<<r<<") x="<<x<<"\n";
      // printf("query(%d, %d) -> (%d, %d)\n", l, r, a, b);

      if(l <= low && r >= high) {
          // for (size_t i = 0; i < level; i++) cout << "  ";
          // cout << "caso1: "<< count << "\n";
          return count;
      }
      if(l > high || r < low) {
        // for (size_t i = 0; i < level; i++) cout << "  ";
        // cout << "caso2\n";
          return 0;
      }

      bool hits = collides(x, Shape{l, r});
      // if (hits) {
      //   cout << "HIT:" << x << " bounds=" << Shape(l, r) << '\n';
      // }

      // for (size_t i = 0; i < level; i++) cout << "  ";
      // cout << "caso3\n";

      return (hits?1:0)
          + (left?left->query(l, r, level+1):0)
          + (right?right->query(l, r, level+1):0);
  }

  void Node::print(int h, int level) {
      int a = x.a[level % 3];
      int b = x.b[level % 3];

      if(right) {
          //for(int i = 0; i < h; ++i) { cout << ' '; }
          //cout << "r\n";
          right->print(h+2, level+1);
      }
      for(int i = 0; i < h; ++i) { cout << ' '; }
      printf("%d:{{(%d, %d, %d), (%d , %d, %d)} (%d, %d, %d) (%d, %d, %d) %d}\n",
              level,
              x.a[0],
              x.a[1],
              x.a[2],
              x.b[0],
              x.b[1],
              x.b[2],
              low[0],
              low[1],
              low[2],
              high[0],
              high[1],
              high[2],
              count);
      if(left) {
          //for(int i = 0; i < h; ++i) { cout << ' '; }
          //cout << "l\n";
          left->print(h+4, level+1);
      }
  }


}
