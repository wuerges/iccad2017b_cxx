#include <astar.hpp>
#include <config.hpp>

#include <algorithm>
#include <map>
#include <set>

namespace iccad {

    using std::sort, std::pair, std::map, std::set;

    void Route::add_point(PT p) {
        route.push_back(p);
    }

    int Route::length() const {
        int result = 0;

        for(int i = 1; i < route.size();++i) {
            result += manhatan(route[i], route[i-1]);
        }

        return result;
    }

    void Route::simplify() {
        for(int i = 2; i < route.size(); ++i) {
            PT & a = route[i-2], & b = route[i-1], & c = route[i];
            
            if(a[0] == b[0] && b[0] == c[0] && a[2] == b[2] && b[2] == c[2]) {
                b.coords[1] = a[1];
            }
            if(a[1] == b[1] && b[1] == c[1] && a[2] == b[2] && b[2] == c[2]) {
                b.coords[0] = a[0];
            }
            if(a[0] == b[0] && b[0] == c[0] && a[1] == b[1] && b[1] == c[1]) {
                b.coords[2] = a[2];
            }
        }
        route.erase(unique(route.begin(), route.end()), route.end());
    }

    ostream & print2D(ostream &out, const PT & p) {
        return out << "(" << p[0] << "," << p[1] << ")";        
    }

    bool operator<(const Route & r1, const Route & r2) {
        return r1.route < r2.route;
    }
        

    ostream & operator<<(ostream &out, const Route & r_) {
        const auto & r = r_.route;
        for(int i = 1; i < r.size(); ++i) {
            const PT & a = r[i-1], & b = r[i];
            int layer = std::min(a[2], b[2]);
            if(a[1] == b[1] && a[2] == b[2]) {
                out << "H-line M" << layer << " ";
                print2D(out, a);
                out << " ";
                print2D(out, b);
                out << '\n';
            }
            else if(a[0] == b[0] && a[2] == b[2]) {
                out << "V-line M" << layer << " ";
                print2D(out, a);
                out << " ";
                print2D(out, b);
                out << '\n';
            }
            else if(a[0] == b[0] && a[1] == b[1]) {
                int beg = std::min(a[2], b[2]);
                int end = std::max(a[2], b[2]);
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


    AStar::AStar(const Treap & sh, const Treap & obs, const Shape & s1, const Shape & s2, V1D b)
    :shapes(sh), obstacles(obs), boundary(b) {
        add_shape(s1);
        add_shape(s2);

        auto pa = min(min(s1.a, s2.a), min(s1.b, s2.b));
        auto pb = max(max(s1.a, s2.a), max(s1.b, s2.b));
        // if(!CONFIG_FAST_MST) {
            pa.coords[1] -= ROUTING_WINDOW;
            pa.coords[0] -= ROUTING_WINDOW;
            pa.coords[2] -= ROUTING_WINDOW;
            pb.coords[0] += ROUTING_WINDOW;
            pb.coords[1] += ROUTING_WINDOW;
            pb.coords[2] += ROUTING_WINDOW;
        // }

        // for (auto sx : obstacles.collect(pa, pb)) {
        //     add_shape(sx.expand(1));
        // };
        obstacles.visit(Shape(pa, pb), [this](const Shape * s) {
            add_shape(s->expand(1));
            return true;
        });

        remove_duplicates(xs);
        fix_boundaries(xs, boundary[0], boundary[2]);
        remove_duplicates(ys);
        fix_boundaries(ys, boundary[1], boundary[3]);
        remove_duplicates(zs);

    }

    AStar::AStar(const Treap & sh, const Treap & obs, const vector<Shape> & sh_vector, const vector<Shape> & obs_vector, V1D b) 
    :shapes(sh), obstacles(obs), boundary(b) {
        for(auto &s : sh_vector) {
            add_shape(s);
        }
        for(auto &o : obs_vector) {
            add_shape(o.expand(1));
        }
        remove_duplicates(xs);
        fix_boundaries(xs, boundary[0], boundary[2]);
        remove_duplicates(ys);
        fix_boundaries(ys, boundary[1], boundary[3]);
        remove_duplicates(zs);
    }


    void AStar::add_shape(const Shape & s) {
        xs.push_back(s.a[0]);
        xs.push_back(s.b[0]);
        ys.push_back(s.a[1]);
        ys.push_back(s.b[1]);
        zs.push_back(s.a[2]);
        zs.push_back(s.b[2]);
    }

    vector<AStar::index> AStar::neighboors(AStar::index i) const {
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

    const PT AStar::make_pt(index i) const {
        auto [x, y, z] = i;
        return PT(xs[x], ys[y], zs[z]);
    }

    int AStar::find(int c, const vector<int> & v) const {
        auto it = lower_bound(v.begin(), v.end(), c);
        if(it != v.end()) return std::distance(v.begin(), it);
        // for(int i = 0; i<v.size(); ++i) {

        //     if(v[i] == c) return i;
        // }
        using std::cerr;
        cerr << "Coordinate not found\n";
        cerr << "Searching for `" << c << "' in {";
        for(int i = 0; i < v.size(); ++i) {
            cerr << v[i] << ' ';
        }
        cerr << "}\n";
        throw -100;
    }

    AStar::index AStar::find(const PT p) const {
        try {
            return {find(p[0], xs), find(p[1], ys), find(p[2], zs)};
        }
        catch (int e) {
            using std::cerr;
            cerr << "Point not found: " << p << '\n';
            throw e;
        }
    }

    void AStar::remove_duplicates(vector<int> & v) {
        sort(v.begin(), v.end());
        v.erase(std::unique(v.begin(), v.end()), v.end());
    }
    void AStar::fix_boundaries(vector<int> & v, int min_bound, int max_bound) {
        v.erase(std::remove_if(v.begin(), v.end(), 
            [min_bound, max_bound](int c) { return c < min_bound || c > max_bound; }
        ),v.end());
    }

    Route AStar::run(const Shape & shape_s, const Shape & shape_t) const {
        using ii = pair<int64_t, index>;
        const int64_t INF = 1e9;
        index s = find(shape_s.a);
        index t = find(shape_t.a);
        
        map<index, int64_t> dst;
        map<index, index> pred;
        set<ii> queue;
        
        // auto xb = std::lower_bound(xs.begin(), xs.end(), shape_s.a[0]);
        // auto xe = std::upper_bound(xb, xs.end(), shape_s.b[0]);
        // auto yb = std::lower_bound(ys.begin(), ys.end(), shape_s.a[1]);
        // auto ye = std::upper_bound(yb, ys.end(), shape_s.b[1]);

        // vector<pair<int64_t, PT>> cands;
        // for(auto ix = xb; ix != xe; ++ix) {
        //     for(auto iy = yb; iy != ye; ++iy)  {
        //         PT cand{*ix, *iy, shape_s.a[2]};
        //         cands.emplace_back(distance(cand, shape_t), cand);
        //     }
        // }
        // sort(cands.begin(), cands.end());
        // auto [w0,p0] = cands.front();
        
        // dst[find(p0)] = 0;
        // queue.insert({0, find(p0)});
        
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

                // Avoid a point if it is inside an obstacle
                auto u_pt = make_pt(u);
                auto v_pt = make_pt(v);

                // std::cout << "Query = " << Shape(u_pt, v_pt) << " = " << obstacles.query(v_pt, u_pt)  <<'\n';
                // for(auto ob : obstacles.collect(v_pt, v_pt)) {
                //     std::cout << "Obstacle: " << ob << '\n';
                // }
                // if(obstacles.query(v_pt, u_pt) > 0) {
                if(obstacles.hits(v_pt, u_pt)) {
                    // std::cout << "Point " << v_pt << " is in an obstacle\n";
                    // for(auto ob : obstacles.collect(v_pt, v_pt)) {
                    //     std::cout << "Obstacle: " << ob << '\n';
                    // }
                    continue;
                }

                int w = manhatan(make_pt(u), make_pt(v));
                // if(collides(Shape(make_pt(u), make_pt(u)), shape_s)) {
                if(distance(make_pt(v), shape_s) == 0) {
                    w = 0;
                }
                // if(distance(make_pt(u), shape_s) == 0) w = 0;

                auto it = dst.find(v);
                int64_t old_w = it != dst.end() ? it->second : INF;

                
                if(old_w > dst[u] + w) {
                    dst[v] = dst[u] + w;
                    pred[v] = u;
                    queue.erase({old_w, v});

                    if(CONFIG_ASTAR) {
                        // A* heuristic
                        int a_star = distance(make_pt(v), shape_t);
                        queue.insert({dst[v]+a_star, v}); 

                    }
                    else {
                        // Dijkstra
                        queue.insert({dst[v], v}); 
                    }

                }
            }

        }

        Route path;

        while(true) {
            PT pt_x = make_pt(x);
            path.add_point(pt_x);
            
            
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

}
