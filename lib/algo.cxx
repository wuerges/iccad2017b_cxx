#include <base.hpp>
#include <algo.hpp>
#include <vector>

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


  int Node::query(const PT center, int radius, int level) {
      return 0;
  }
        

  int Node::query(const PT l, const PT r, int level) {

      if(l <= low && r >= high) {
          return count;
      }
      if(l > high || r < low) {
          return 0;
      }
      if(l[level % 3] > high[level % 3] || r[level % 3] < low[level % 3]) {
          return 0;
      }

      bool hits = collides(x, Shape{l, r});

      return (hits?1:0)
          + (left?left->query(l, r, level+1):0)
          + (right?right->query(l, r, level+1):0);
  }

  int Node::collect(std::vector<Shape> & results, const PT l, const PT r, int level) {

      if(l > high || r < low) {
          return 0;
      }
      if(l[level % 3] > high[level % 3] || r[level % 3] < low[level % 3]) {
          return 0;
      }

      bool hits = collides(x, Shape{l, r});
      if(hits) results.push_back(x);

      return (hits?1:0)
          + (left?left->collect(results, l, r, level+1):0)
          + (right?right->collect(results, l, r, level+1):0);
  }

    int Node::collect(std::vector<Shape> & results, const PT center, int radius, int level) {
        return 0;
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
