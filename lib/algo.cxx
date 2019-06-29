#include <algo.hpp>
#include <base.hpp>

#include <iostream>
#include <vector>
#include <cmath>

namespace iccad {

using std::unique_ptr, std::make_unique, std::cout;

void Node::add(const Shape &shape, int level) {
  int a = x.a[level % 3];
  int b = x.b[level % 3];
  int l = shape.a[level % 3];
  int r = shape.b[level % 3];
  if (r < a) {
    if (left) {
      left->add(shape, level + 1);
    } else {
      left = make_unique<Node>(shape);
    }
  } else {
    if (right) {
      right->add(shape, level + 1);
    } else {
      right = make_unique<Node>(shape);
    }
  }
  if (left) {
    low = min(low, left->low);
    high = max(high, left->high);
  }
  if (right) {
    low = min(low, right->low);
    high = max(high, right->high);
  }
  count = 1 + (left ? left->count : 0) + (right ? right->count : 0);
}

const bool sphere_collides(const PT center, int radius32, const int64_t y, const int z, const int64_t x1, const int64_t x2) {
  // (x - h)^2 + (y - k)^2 + (z - l)^2 == r^2
  
  int64_t h = center.x;
  int64_t k = center.y;
  int64_t l = center.z;
  int64_t r = radius32;

  int64_t a = 1;  
  int64_t b = - 2 * h;

  int64_t c = h*h + (y-k)*(y-k) + (z-l)*(z-l) - r*r;

  // has solution if b^2 - 4ac >= 0

  //std::printf("y=%d z=%d x1=%d x2=%d\n", y, z, x1, x2);
  if (b*b - 4*a*c < 0) {
    //std::cout << "infeasible\n";
    return false;
  }

  double delta = b*b - 4*a*c;
  double root1 = (-b+sqrt(delta))/(2*a);
  double root2 = (-b-sqrt(delta))/(2*a);
  // std::cout << "roots: " << root1 << ", " << root2 << '\n';
  return (x1 >= root1 && root1 >= x2) || (x1 >= root2 && root2 >= x2);
}



const bool sphere_collides(const PT center, int radius32, const PT low,
                           const PT high) {
  
  const PT transposed{center.y, center.x, center.z};

  return sphere_collides(center, radius32, low.y, low.z, high.x, low.x)
    ||  sphere_collides(center, radius32, high.y, low.z, high.x, low.x)
    || sphere_collides(transposed, radius32, low.x, low.z, high.y, low.y)
    || sphere_collides(transposed, radius32, high.x, low.z, high.y, low.y)
    || sphere_contains(center, radius32, low, high);

  // int64_t radius = radius32;
  // int64_t s_radius = radius * radius;

  // for (int x : {high.x, low.x}) {
  //   for (int y : {high.y, low.y}) {
  //     for (int z : {high.z, low.z}) {
  //       int64_t dx = (x - center.x);
  //       int64_t dy = (y - center.y);
  //       int64_t dz = (z - center.z);
  //       int64_t p_radius = dx * dx + dy * dy + dz * dz;
  //       if (p_radius <= s_radius) {
  //         return true;
  //       }
  //     }
  //   }
  // }
  // return false;
}

const bool diamond_collides(const Shape & center, int radius32, const PT low, const PT high) {
  return distance(center, Shape{low, high}) <= radius32;
}
const bool diamond_contains(const Shape & center, int radius32, const PT low, const PT high) {
  PT a{low.x, high.y, low.z};
  PT b{high.x, low.y, low.z};
  return distance(low, center) <= radius32 
    && distance(high, center) <= radius32
    && distance(a, center) <= radius32
    && distance(b, center) <= radius32;
}


const bool sphere_contains(const PT center, int radius32, const PT low,
                           const PT high) {
  int64_t radius = radius32;
  int64_t s_radius = radius * radius;
  for (int x : {high.x, low.x}) {
    for (int y : {high.y, low.y}) {
      for (int z : {high.z, low.z}) {
        int64_t dx = (x - center.x);
        int64_t dy = (y - center.y);
        int64_t dz = (z - center.z);
        int64_t p_radius = dx * dx + dy * dy + dz * dz;
        if (p_radius > s_radius) {
          return false;
        }
      }
    }
  }
  return true;
}

int Node::query_sphere(const PT center, int radius, int level) {
  // std::cout << "query_sphere("<<center<<","<<radius<<","<<level<<")\n";

  // Passes for 100.000 tests
  if (  ((center[level % 3] - radius) > high[level % 3])
     || ((center[level % 3] + radius) < low[level % 3])   ) {
    return 0;
  }

  // Passes for 100.000 tests
  if (sphere_contains(center, radius, low, high)) {
    // std::cout << "sphere " << center << " " << radius << '\n';
    // std::cout << "case contains " << low << " " << high << '\n';
    // std::cout << "count = " << count << '\n';
    return count;
  }
  // if (!sphere_collides(center, radius, low, high)) {
  //   return 0;
  // }
  bool hits = sphere_collides(center, radius, x.a, x.b);

  return (hits ? 1 : 0) +
         (left ? left->query_sphere(center, radius, level + 1) : 0) +
         (right ? right->query_sphere(center, radius, level + 1) : 0);
}

int Node::query_diamond(const Shape & center, int radius, int level) {
  // std::cout << "query_sphere("<<center<<","<<radius<<","<<level<<")\n";

  // Passes for 100.000 tests
  if (  ((center.a[level % 3] - radius) > high[level % 3])
     || ((center.b[level % 3] + radius) < low[level % 3])   ) {
    return 0;
  }

  // Passes for 100.000 tests
  if (diamond_contains(center, radius, low, high)) {
    // std::cout << "sphere " << center << " " << radius << '\n';
    // std::cout << "case contains " << low << " " << high << '\n';
    // std::cout << "count = " << count << '\n';
    return count;
  }
  // if (!sphere_collides(center, radius, low, high)) {
  //   return 0;
  // }
  bool hits = diamond_collides(center, radius, x.a, x.b);

  return (hits ? 1 : 0) +
         (left ? left->query_diamond(center, radius, level + 1) : 0) +
         (right ? right->query_diamond(center, radius, level + 1) : 0);
}



int Node::query(const PT l, const PT r, int level) {

  if (l <= low && r >= high) {
    return count;
  }
  if (l > high || r < low) {
    return 0;
  }
  if (l[level % 3] > high[level % 3] || r[level % 3] < low[level % 3]) {
    return 0;
  }

  bool hits = collides(x, Shape{l, r});

  return (hits ? 1 : 0) + (left ? left->query(l, r, level + 1) : 0) +
         (right ? right->query(l, r, level + 1) : 0);
}

bool Node::hits(const PT l, const PT r, int level) {
  // std::cout << "hits: "<< level<<"\n";

  if (l <= low && r >= high) {
    // std::cout << "case1\n";
    return count > 0;
  }
  if (l > high || r < low) {
    // std::cout << "case2\n";
    return false;
  }
  if (l[level % 3] > high[level % 3] || r[level % 3] < low[level % 3]) {
    // std::cout << "case3\n";
    return false;
  }

  bool hits = collides(x, Shape{l, r});
  // std::cout << "collides?"<<hits<<"\n";

  return hits || 
    (left ? left->hits(l, r, level + 1) : false) ||
    (right ? right->hits(l, r, level + 1) : false);
}

int Node::collect(std::vector<Shape> &results, const PT l, const PT r,
                  int level) {

  if (l > high || r < low) {
    return 0;
  }
  if (l[level % 3] > high[level % 3] || r[level % 3] < low[level % 3]) {
    return 0;
  }

  bool hits = collides(x, Shape{l, r});
  if (hits)
    results.push_back(x);

  return (hits ? 1 : 0) + (left ? left->collect(results, l, r, level + 1) : 0) +
         (right ? right->collect(results, l, r, level + 1) : 0);
}

int Node::collect_sphere(std::vector<Shape> &results, const PT center,
                         int radius, int level) {
  if (  ((center[level % 3] - radius) > high[level % 3])
     || ((center[level % 3] + radius) < low[level % 3])   ) {
    return 0;
  }



  // std::cout << "collect_sphere\n";
  bool hits = sphere_collides(center, radius, x.a, x.b);
  // std::cout << "center: " << center << " radius = " << radius << " (" << x << ")\n";
  if (hits) {
    // std::cout << "HIT! " << x <<'\n';
    results.push_back(x);
  }


  // if (!sphere_collides(center, radius, low, high)) {
  //   return 0;
  // }

  return (hits ? 1 : 0) +
         (left ? left->collect_sphere(results, center, radius, level + 1) : 0) +
         (right ? right->collect_sphere(results, center, radius, level + 1)
                : 0);
}

int Node::collect_diamond(std::vector<Shape> &results, const Shape & center,
                         int radius, int level) {
  if (  ((center.a[level % 3] - radius) > high[level % 3])
     || ((center.b[level % 3] + radius) < low[level % 3])   ) {
    return 0;
  }



  // std::cout << "collect_sphere\n";
  bool hits = diamond_collides(center, radius, x.a, x.b);
  // std::cout << "center: " << center << " radius = " << radius << " (" << x << ")\n";
  if (hits) {
    // std::cout << "HIT! " << x <<'\n';
    results.push_back(x);
  }


  // if (!sphere_collides(center, radius, low, high)) {
  //   return 0;
  // }

  return (hits ? 1 : 0) +
         (left ? left->collect_diamond(results, center, radius, level + 1) : 0) +
         (right ? right->collect_diamond(results, center, radius, level + 1)
                : 0);
}

int Node::collect_diamond_2(std::vector<Shape> &results, const Shape & center,
                         int radius1, int radius2, int level) {
  if (  ((center.a[level % 3] - radius2) > high[level % 3])
     || ((center.b[level % 3] + radius2) < low[level % 3])   
     ) {
    return 0;
  }

  // if (diamond_collides(center, radius2, low, high)) return 0;
  // if (distance(center, Shape{low, high}) < radius1 ) return 0;

  // if ( ! diamond_collides(center, radius2+1, low, high)) return 0;

  if( diamond_contains(center,radius1-1, low, high) ) return 0;
  // if (radius1 > 0 && diamond_collides(center, radius1, x.a, x.b)) {
  //   return 0;
  // }

  // std::cout << "collect_sphere\n";
  bool hits1 = radius1 > 0 && diamond_collides(center, radius1, x.a, x.b);
  bool hits = diamond_collides(center, radius2, x.a, x.b);
  // std::cout << "center: " << center << " radius = " << radius << " (" << x << ")\n";
  if (!hits1 && hits) {
    // std::cout << "HIT! " << x <<'\n';
    results.push_back(x);
  }


  // if (!sphere_collides(center, radius, low, high)) {
  //   return 0;
  // }

  return (hits ? 1 : 0) +
         (left ? left->collect_diamond_2(results, center, radius1, radius2, level + 1) : 0) +
         (right ? right->collect_diamond_2(results, center, radius1, radius2, level + 1)
                : 0);
}

void Node::print(int h, int level) {
  int a = x.a[level % 3];
  int b = x.b[level % 3];

  if (right) {
    // for(int i = 0; i < h; ++i) { cout << ' '; }
    // cout << "r\n";
    right->print(h + 2, level + 1);
  }
  for (int i = 0; i < h; ++i) {
    cout << ' ';
  }
  printf("%d:{{(%d, %d, %d), (%d , %d, %d)} (%d, %d, %d) (%d, %d, %d) %d}\n",
         level, x.a[0], x.a[1], x.a[2], x.b[0], x.b[1], x.b[2], low[0], low[1],
         low[2], high[0], high[1], high[2], count);
  if (left) {
    // for(int i = 0; i < h; ++i) { cout << ' '; }
    // cout << "l\n";
    left->print(h + 4, level + 1);
  }
}

} // namespace iccad
