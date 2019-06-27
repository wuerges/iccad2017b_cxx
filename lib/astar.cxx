#include <astar.hpp>

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
            
            if(a.x == b.x && b.x == c.x && a.z == b.z && b.z == c.z) {
                b.y = a.y;
            }
            if(a.y == b.y && b.y == c.y && a.z == b.z && b.z == c.z) {
                b.x = a.x;
            }
            if(a.x == b.x && b.x == c.x && a.y == b.y && b.y == c.y) {
                b.z = a.z;
            }
        }
        route.erase(unique(route.begin(), route.end()), route.end());
    }

    ostream & print2D(ostream &out, const PT & p) {
        return out << "(" << p.x << "," << p.y << ")";        
    }

    ostream & operator<<(ostream &out, const Route & r_) {
        const auto & r = r_.route;
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


    AStar::AStar(const Treap & sh, const Treap & obs, const Shape & s1, const Shape & s2, V1D b)
    :shapes(sh), obstacles(obs), boundary(b), source(s1), target(s2) {
        add_shape(s1);
        add_shape(s2);

        auto pa = min(min(s1.a, s2.a), min(s1.b, s2.b));
        auto pb = max(max(s1.a, s2.a), max(s1.b, s2.b));

        for (auto sx : obstacles.collect(pa, pb)) {
            add_shape(sx.expand(1));
        };

    }

    void AStar::add_shape(const Shape & s) {
        xs.push_back(s.a.x);
        xs.push_back(s.b.x);
        ys.push_back(s.a.y);
        ys.push_back(s.b.y);
        zs.push_back(s.a.z);
        zs.push_back(s.b.z);
    }

    vector<AStar::index> AStar::neighboors(AStar::index i) {
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

    const PT AStar::make_pt(index i) {
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
            return {find(p.x, xs), find(p.y, ys), find(p.z, zs)};
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

    Route AStar::run() {

        remove_duplicates(xs);
        fix_boundaries(xs, boundary[0], boundary[2]);
        remove_duplicates(ys);
        fix_boundaries(ys, boundary[1], boundary[3]);
        remove_duplicates(zs);
        return run1(source, target);
    }

    // vector<PT> AStar::bad_run(const PT s, const Shape & ts) {
    //     using std::min, std::max;
    //     vector<PT> result;

    //     PT t = ts.a;

    //     result.push_back(s);
        
    //     PT art1 = s;
    //     art1.x = t.x;
    //     if(s.x != t.x)
    //         result.push_back(art1);

    //     PT art2 = art1;
    //     art2.z = t.z;
    //     if(s.z != t.z) 
    //         result.push_back(art2);

    //     result.push_back(t);

    //     return result;
    // }
    
    Route AStar::run1(const Shape & shape_s, const Shape & shape_t) {
        using ii = pair<int64_t, index>;
        const int64_t INF = 1e9;
        index s = find(shape_s.a);
        index t = find(shape_t.a);
        
        map<index, int64_t> dst;
        map<index, index> pred;
        set<ii> queue;
        
        dst[s] = 0;
        // auto xb = std::lower_bound(xs.begin(), xs.end(), shape_s.a.x);
        // auto xe = std::upper_bound(xb, xs.end(), shape_s.b.x);
        // auto yb = std::lower_bound(ys.begin(), ys.end(), shape_s.a.y);
        // auto ye = std::upper_bound(yb, ys.end(), shape_s.b.y);
        // for(auto ix = xb; ix != xe; ++ix) {
        //     for(auto iy = yb; iy != ye; ++iy)  {
        //         PT cand{*ix, *iy, shape_s.a.z};
        //         if(collides(cand, shape_s)) {
        //             dst[find(cand)] = 0;
        //             queue.insert({0, find(cand)});
        //         }
        //     }
        // }
        queue.insert({0, s});
        // queue.insert({0, find(shape_s.b)});
        // queue.insert({0, find(PT{shape_s.a.x, shape_s.b.y, shape_s.a.z})});
        // queue.insert({0, find(PT{shape_s.b.x, shape_s.a.y, shape_s.a.z})});
        
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
                    // queue.insert({dst[v], v}); // Dijkstra

                    // A* heuristic
                    int a_star = distance(make_pt(v), shape_t);
                    queue.insert({dst[v]+a_star, v}); 
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
