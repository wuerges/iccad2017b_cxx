#include <model.hpp>

#include <iostream>
#include <string>
#include <vector>
#include <set>
#include <utility>
#include <cassert>
#include <cmath>
#include <map>

using namespace model;

namespace {
  using std::vector;
  using std::map;
  using std::set;

  int binsize(const V1D & cs) {
    int sqr = sqrt(cs.size());
    return std::max(100, sqr);
  }

  vector<int> bins(const V1D & cs) {
    vector<int> x;
    for(int i = 0; i < cs.size(); i+= binsize(cs)) {
      x.push_back(cs[i]);
    }
    return x;
  }

  struct YGrid {
    map<int, vector<P3D> > points;

    void add_points(const set<P3D> & pts) {
      V1D ys;
      for(auto p : pts) { ys.push_back(p.y); }
      auto bs = bins(ys);
      for(int b : bs) {
        points[b] = {};
      }

      for(auto p : pts) {
        add_1point(p);
      }
    }

    void add_1point( const P3D & p) {
      auto it = points.lower_bound(p.y);
      it->second.push_back(p);
    }

    void debug() const {
      using std::cout;
      cout << "YGrid points.size() = " << points.size() << '\n';
    }
  };

  struct XGrid {
    map<int, YGrid> points;

    void add_points(const set<P3D> & pts) {
      V1D xs;
      for(auto p : pts) {
        xs.push_back(p.x);
      }
      auto bs = bins(xs);

      map<int, set<P3D>> parts;
      for(int b : bs) {
        parts[b] = {};
      }

      for(auto p : pts) {
        auto it = parts.lower_bound(p.x);
        it->second.insert(p);
      }

      for(auto [b, px] : parts) {
        points[b].add_points(px);
      }
    }

    void debug() const {
      using std::cout;
      cout << "XGrid = " << points.size() << '\n';
    }
  };

  // struct XGrid {
  //   map<int, map<int, map<int, P3D>>> parts;
  //   vector<int> x_bins, y_bins, z_bins;
  //
  //   HGrid(
  //     const V1D & xs,
  //     const V1D & ys,
  //     const V1D & zs)
  //     : x_bins(bins(xs))
  //     , y_bins(bins(ys))
  //     , z_bins(bins(zs))
  //      {
  //      }
  //
  // };
}

void model::generate_hannan(Grid & g) {
  using std::cout;
  V1D xs, ys, zs;
  for(auto p : g.grid) {
    xs.push_back(p.x);
    ys.push_back(p.y);
    zs.push_back(p.z);

  }

  // HGrid h(xs, ys, zs);

  XGrid xg;
  xg.add_points(g.grid);
  xg.debug();
  //
  // cout << "x coordinates: " << xs.size() << " binsize: " << binsize(xs) << '\n';
  // cout << "y coordinates :" << ys.size() << '\n';
  // cout << "z coordinates :" << zs.size() << '\n';

  cout << "hannan grid generated\n";

}
