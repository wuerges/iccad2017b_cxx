#include <model.hpp>

#include <iostream>
#include <string>
#include <vector>
#include <set>
#include <utility>
#include <cassert>
#include <cmath>
#include <map>

using namespace iccad;

namespace {
  using std::vector;
  using std::map;
  using std::set;

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

// void iccad::generate_hannan(Grid & g) {
//   using std::cout;
//   V1D xs, ys, zs;
//   for(auto p : g.grid) {
//     xs.push_back(p.x);
//     ys.push_back(p.y);
//     zs.push_back(p.z);
//   }

//   // HGrid h(xs, ys, zs);

//   //
//   // cout << "x coordinates: " << xs.size() << " binsize: " << binsize(xs) << '\n';
//   // cout << "y coordinates :" << ys.size() << '\n';
//   // cout << "z coordinates :" << zs.size() << '\n';

//   cout << "hannan grid generated\n";

// }
