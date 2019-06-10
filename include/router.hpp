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

        Treap & shapes, & obstacles;
        vector<int> xs;
        vector<int> ys;
        vector<int> zs;

        AStar(Treap & sh, Treap & obs, const Shape & s1, const Shape & s2)
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

        void remove_duplicates(vector<int> & v) {
            sort(v.begin(), v.end());
            v.erase(std::unique(v.begin(), v.end()), v.end());
        }

        vector<PT> run(const Shape & s, const Shape & t) {
            remove_duplicates(xs);
            remove_duplicates(ys);
            remove_duplicates(zs);
            return run1(s, t);
            // return bad_run(s, t);
        }

        vector<PT> bad_run(const PT s, const Shape & ts) {
            using std::min, std::max;
            vector<PT> result;

            PT t = ts.a;

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
        
        vector<PT> run1(const Shape & shape_s, const Shape & shape_t) {
            using ii = pair<int64_t, index>;
            const int64_t INF = 1e9;
            index s = find(shape_s.a);
            index t = find(shape_t.a);
            
            map<index, int64_t> dst;
            map<index, index> pred;
            set<ii> queue;
            
            dst[s] = 0;
            queue.insert({0, s});
            index x = t;
            while(!queue.empty()) {
                auto [_, u] = *queue.begin();
                if(u == t) break;
                if(distance(make_pt(u), shape_t) == 0) {
                // if(collides(Shape(make_pt(u), make_pt(u)), shape_t)) {
                    x = u;
                    break;
                }
                queue.erase(queue.begin());
                // std::cout << "N of " << make_pt(u) << '\n';
                // for(auto v : neighboors(u)) {
                //     std::cout << ":-> " << make_pt(v) << '\n';
                // }
                // std::cout << '\n';
                
                for(auto v : neighboors(u)) {

                    int w = manhatan(make_pt(u), make_pt(v));
                    // if(collides(Shape(make_pt(u), make_pt(u)), shape_s)) {
                    if(distance(make_pt(u), shape_s) == 0) {
                        w = 0;
                    }
                    // if(distance(make_pt(u), shape_s) == 0) w = 0;

                    auto it = dst.find(v);
                    int64_t old_w = it != dst.end() ? it->second : INF;

                    
                    if(old_w > dst[u] + w) {
                        dst[v] = dst[u] + w;
                        pred[v] = u;
                        queue.erase({old_w, v});
                        queue.insert({dst[v], v}); // Dijkstra

                        // A* heuristic
                        // int a_star = distance(make_pt(v), shape_t);
                        // queue.insert({dst[v]+a_star, v}); 
                    }
                }

            }

            vector<PT> path;

            while(true) {
                PT pt_x = make_pt(x);
                path.push_back(pt_x);
                
               
                if(collides(Shape(pt_x, pt_x), shape_s)) break;
                if(distance(make_pt(x), shape_s) == 0) break;

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
        Treap  treap, obstacles;

        Router(int sp, int vc):spacing(sp), viaCost(vc) {}
        
        Route calculate_route(const Shape & s1, const Shape & s2) 
        {
            AStar st(treap, obstacles, s1, s2);
            auto pts = st.run(s1, s2);
            for(auto & pt : pts) {
                pt.z = z_to_layer(pt.z, viaCost);
            }
            return Route(pts);
        }


        void perform_global_routing(const vector<Shape> & shapes, 
            const vector<Shape> & obs, 
            ostream & out) {
            treap.populate(shapes);
            obstacles.populate(obs);

            // for(auto s : shapes) std::cout << s << '\n';

            MST mst;
            auto res = mst.run(treap, shapes);

            for(auto [a, b] : res) {
                auto r = calculate_route(a, b);
                out << r ;
            }
        }

    };
}