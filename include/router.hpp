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

    ostream & print2D(ostream &out, const PT & p) {
        return out << "(" << p.x << "," << p.y << ")";        
    }

    ostream & operator<<(ostream &out, const Route & r) {
        for(int i = 1; i < r.size(); ++i) {
            const PT & a = r[i-1], & b = r[i];
            int layer = std::min(a.z, b.z);
            if(a.y == b.y && a.z == b.z) {
                out << "H-line M" << layer << " ";
                print2D(out, a);
                out << " ";
                print2D(out, b);
                out << '\n';
            }
            else if(a.x == b.x && a.z == b.z) {
                out << "V-line M" << layer << " ";
                print2D(out, a);
                out << " ";
                print2D(out, b);
                out << '\n';
            }
            else if(a.x == b.x && a.y == b.y) {
                int beg = std::min(a.z, b.z);
                int end = std::max(a.z, b.z);
                for(int zi = beg; zi < end; ++zi) {
                    out << "Via V" << zi  << " ";
                    print2D(out, a);
                    out << '\n';
                }
            }
            else {
                std::cerr << "Route not rectangular\n";
                std::cerr << a << " " << b << '\n';
            }
        }
        return out;
    }

    struct AStar {

        using index = tuple<int, int, int>;

        const Treap & shapes, & obstacles;
        vector<int> xs;
        vector<int> ys;
        vector<int> zs;

        AStar(const Treap & sh, const Treap & obs, const Shape & s1, const Shape & s2)
        :shapes(sh), obstacles(obs) {
            add_shape(s1);
            add_shape(s2);

            // TODO search for obstacles             
        }

        void add_shape(const Shape & s) {
            xs.push_back(s.a.x);
            xs.push_back(s.b.x);
            ys.push_back(s.a.y);
            ys.push_back(s.b.y);
            zs.push_back(s.a.z);
            zs.push_back(s.b.z);
        }

        vector<index> neighboors(index i) {
            vector<index> result;

            auto [x, y, z] = i;
            if(x > 0)           result.push_back({x-1, y, z});
            if(x < xs.size()-1) result.push_back({x+1, y, z});

            if(y > 0)           result.push_back({x, y-1, z});
            if(y < ys.size()-1) result.push_back({x, y+1, z});

            if(z > 0)           result.push_back({x, y, z-1});
            if(z < zs.size()-1) result.push_back({x, y, z+1});
            
            return result;
        }

        const PT make_pt(index i) {
            auto [x, y, z] = i;
            return PT(xs[x], ys[y], zs[z]);
        }

        int find(int c, const vector<int> & v) const {
            for(int i = 0; i<v.size(); ++i) {
                if(v[i] == c) return i;
            }
            using std::cerr;
            cerr << "Coordinate not found\n";
            cerr << "Searching for `" << c << "' in {";
            for(int i = 0; i < v.size(); ++i) {
                cerr << v[i] << ' ';
            }
            cerr << "}\n";
            throw -100;
        }

        index find(const PT p) const {
            try {
                return {find(p.x, xs), find(p.y, ys), find(p.z, zs)};
            }
            catch (int e) {
                using std::cerr;
                cerr << "Point not found: " << p << '\n';
                throw e;
            }
        }

        vector<PT> run(const PT s, const PT t) {
            // return run(find(s), find(t));
            return bad_run(s, t);
        }

        vector<PT> bad_run(const PT s, const PT t) {
            using std::min, std::max;
            vector<PT> result;

            result.push_back(s);
            
            PT art1 = s;
            art1.x = t.x;
            if(s.x != t.x)
                result.push_back(art1);

            PT art2 = art1;
            art2.z = t.z;
            if(s.z != t.z) 
                result.push_back(art2);

            result.push_back(t);

            return result;
        }
        
        vector<PT> run(index s, index t) {
            using ii = pair<double, index>;
            const double INF = 1e20;
            
            map<index, double> dst;
            map<index, index> pred;
            set<ii> queue;
            
            dst[s] = 0;
            queue.insert({0, s});
            while(!queue.empty()) {
                auto [_, u] = *queue.begin();
                if(u == t) break;
                queue.erase(queue.begin());
                for(auto v : neighboors(u)) {
                    double w = euclid(make_pt(u), make_pt(v));
                    auto it = dst.find(v);
                    double old_w = it != dst.end() ? it->second : INF;
                    if(old_w > dst[u] + w) {
                        dst[v] = dst[u] + w;
                        pred[v] = u;
                        queue.insert({dst[v]+euclid(make_pt(v), make_pt(t)), v});
                    }
                }

            }

            vector<PT> path;
            auto x = t;
            while(true) {
                path.push_back(make_pt(x));
                auto it = pred.find(x);
                if(it != pred.end()) {
                    x = it->second;
                }
                else {
                    break;
                }
            }
            return path;
        }
    };


    struct Router {

        int spacing, viaCost;

        Router(int sp, int vc):spacing(sp), viaCost(vc) {}
        
        Route calculate_route(const Treap & treap, const Shape & s1, const Shape & s2) 
        {
            AStar st(treap, treap, s1, s2);
            auto pts = st.run(s1.a, s2.a);
            for(auto & pt : pts) {
                pt.z = z_to_layer(pt.z, viaCost);
            }
            return Route(pts);
        }


        void perform_global_routing(const vector<Shape> & shapes, 
            const vector<Shape> & obstacles, 
            ostream & out) {
            Treap  treap;
            treap.populate(shapes);

            for(auto s : shapes) std::cout << s << '\n';

            MST mst;
            auto res = mst.run(treap, shapes);

            for(auto [a, b] : res) {
                auto r = calculate_route(treap, a, b);
                out << r ;
            }
        }

    };
}