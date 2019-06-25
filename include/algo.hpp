#pragma once

#include <base.hpp>
#include <iostream>
#include <memory>
#include <vector>

namespace iccad {

const bool sphere_collides(const PT center, int radius32, const int64_t y, const int z, const int64_t x1, const int64_t x2);
const bool sphere_collides(const PT center, int radius32, const PT low,
                           const PT high);
const bool sphere_contains(const PT center, int radius, const PT a, const PT b);

// const bool diamond_collides(const Shape & center, int radius32, const PT low,
//                            const PT high);
// const bool diamond_contains(const Shape & center, int radius32, const PT low,
//                            const PT high);


struct Node {
  Node(const Shape _x) : x(_x), low(_x.a), high(_x.b), count(1) {}

  const Shape x;
  PT low, high;
  int count;

  std::unique_ptr<Node> left, right;

  void add(const Shape &shape, int level = 0);
  bool hits(const PT l, const PT r, int level = 0);
  int query(const PT l, const PT r, int level = 0);
  int query_sphere(const PT center, int radius, int level = 0);
  int query_diamond(const Shape & center, int radius, int level = 0);
  // bool sphere_collides(const PT center, int radius) const ;
  // bool sphere_contains(const PT center, int radius) const ;
  int collect(std::vector<Shape> &results, const PT l, const PT r,
              int level = 0);
  int collect_sphere(std::vector<Shape> &results, const PT center, int radius,
                     int level = 0);
  int collect_diamond(std::vector<Shape> &results, const Shape & center, int radius,
                     int level = 0);
  void print(int h = 0, int level = 0);
};

/**
 * A facade for the Treap, with easier to use functions.
 */
struct Treap {
  std::unique_ptr<Node> root;

  void populate(const std::vector<Shape> &shapes) {
    for (const Shape &s : shapes) {
      add(s);
    }
  }

  void add(const Shape &shape) {
    if (root) {
      root->add(shape);
    } else {
      root = std::make_unique<Node>(shape);
    }
  }

  int query(const PT l, const PT r) const {
    if (root) {
      return root->query(min(l, r), max(l, r));
    }
    return 0;
  }

  int query(const PT center, int radius) const {
    if (root) {
      return root->query_sphere(center, radius);
    }
    return 0;
  }

  int query_diamond(const Shape & center, int radius) const {
    if (root) {
      return root->query_diamond(center, radius);
    }
    return 0;
  }

  int hits(const PT l, const PT r) const {
    if (root) {
      return root->hits(min(l, r), max(l, r));
    }
    return 0;
  }


  std::vector<Shape> collect(const PT l, const PT r) const {

    std::vector<Shape> results;
    if (root) {
      root->collect(results, l, r);
    }
    return results;
  }

  std::vector<Shape> collect(const PT center, int radius) const {

    std::vector<Shape> results;
    if (root) {
      root->collect_sphere(results, center, radius);
    }
    return results;
  }

  std::vector<Shape> collect_diamond(const Shape & center, int radius) const {

    std::vector<Shape> results;
    if (root) {
      root->collect_diamond(results, center, radius);
    }
    return results;
  }

  std::vector<Shape> neighboors(const Shape &u, size_t number) const {
    int w = 1;
    PT l = u.a;
    PT r = u.b;
    int q = query(PT(l.x - w, l.y - w, l.z - w), PT(r.x + w, r.y + w, r.z + w));
    // std::cout << " w =" << w << "\n";
    while (q <= number && w < 1e8) {
      w = w * 2;
      q = query(PT(l.x - w, l.y - w, l.z - w), PT(r.x + w, r.y + w, r.z + w));
      // std::cout << "Query=" << q << " w=" << w
      //   << " pts=" << PT(l.x-w, l.y-w, l.z-w)
      //   << " <-> "<<  PT(r.x+w, r.y+w, r.z+w) <<  '\n';
    }
    return collect(PT(l.x - w, l.y - w, l.z - w),
                   PT(l.x + w, l.y + w, l.z + w));
  }

  std::vector<Shape> neighboors_sphere(const Shape &u, size_t number) const {
    int w = 1;
    PT center =
        PT{(u.a.x + u.b.x) / 2, (u.a.y + u.b.y) / 2, (u.a.z + u.b.z) / 2};

    int t = abs(u.a.x - u.b.x) + abs(u.a.y-u.b.y) + abs(u.a.z-u.b.z);
    int q = query(center, t+w);
    // std::cout << " w =" << w << "\n";
    while (q <= number && w < 1e8) {
      w = w * 2;
      q = query(center, t+w);
      // std::cout << "Query=" << q << " w=" << w << '\n';
      // std::cout << "Query=" << q << " w=" << w
      //   << " pts=" << PT(l.x-w, l.y-w, l.z-w)
      //   << " <-> "<<  PT(r.x+w, r.y+w, r.z+w) <<  '\n';
    }
    return collect(center, t+w);
  }

  std::vector<Shape> neighboors_diamond(const Shape &u, size_t number) const {
    int w = 1;
    int q = query_diamond(u, w);
    // std::cout << " w =" << w << "\n";
    // return collect_diamond(center, t+8000);

    while (q <= number && w < 1e8) {
      w = w * 2;
      q = query_diamond(u, w);
      // std::cout << "Query=" << q << " w=" << w << '\n';
      // std::cout << "Query=" << q << " w=" << w
      //   << " pts=" << PT(l.x-w, l.y-w, l.z-w)
      //   << " <-> "<<  PT(r.x+w, r.y+w, r.z+w) <<  '\n';
    }
    return collect_diamond(u, w);
  }
};
} // namespace iccad
