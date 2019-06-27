#include <kappamst.hpp>

#include <set>
#include <unordered_set>
#include <algorithm>
#include <astar.hpp>

namespace iccad {

    using std::set, std::tuple, std::unordered_set;

KMST::KMST(const Treap & treap, const Treap & obstacles,
      const vector<Shape> & shapes, const V1D & boundary) 
      : treap(treap), obstacles(obstacles), shapes(shapes), boundary(boundary)
{}

tuple<bool, int, Shape> KMST::fetch_next(const Shape & u) {
    
    if(adj_i[u] >= adj[u].size()) {
        const int INF = 1e8;

        if(radius[u] < 1e8) {
            radius[u] = std::min(INF, 2 * radius[u] + 1);
        }
        adj_i[u] = 0;
        adj[u].clear();

        auto rs = treap.collect_diamond(u, radius[u]);
        std::cout << "radius " << radius[u] << '\n';
        std::cout << "fetched " << rs.size() << '\n';

        for(auto & v : rs) {
            adj[u].emplace_back(distance(u, v), v);
        }
        sort(adj[u].begin(), adj[u].end());


    }

    std::cout << "not fetched " << adj_i[u] << " " << adj[u].size() << '\n';
        

    auto [w, v] = adj[u][adj_i[u]++];
    return {true, w, u};
}    

vector<pair<Shape, Shape>> KMST::run()  {
    vector<pair<Shape, Shape>> results;    

    set<tuple<int64_t, Shape, Shape, bool>> queue;

    // for(int i = 0; i < adj.size(); ++i) 
    //     sort(adj[i].begin(), adj[i].end());

    for(auto & k : shapes) {
        adj[k] = {};        
        queue.insert({0, k, k, true});
        adj_i[k] = 0;
        radius[k] = 1000;            
    }

    int edges = 0;
    int64_t total = 0;



    while(!queue.empty() && (edges+1) < shapes.size()) {
        std::cout << "\n\n\nqueue not empty: "<< shapes.size()<<" "<< edges <<" " << queue.empty()<<" \n";
        
        //auto [w, u, v] = *queue.begin();
        auto [w, u, v, calc] = *queue.begin();
        queue.erase(queue.begin());

        std::cout << "processing " << u << " " << v << '\n';

        if(muf.Find(u) != muf.Find(v)) {
            //printf("edge %d -> %d : %d\n", u, v, w);


            auto a = min(min(u.a, v.a), min(u.b, v.b));
            auto b = max(max(u.a, v.a), max(u.b, v.b));

            if (!calc && obstacles.query(a, b) > 0) {
                int new_d = AStar(treap, obstacles, boundary).run1(u, v).length();
                queue.insert({new_d, u, v, true});
            }
            else {
                total += w;
                ++edges;
                std::cout << "adding a result " << u << " " << v << '\n';
                muf.Union(u, v);
                results.emplace_back(u,v);
            }
        }

        std::cout << "fetching next" << '\n';
        auto [t, w2, v2] = fetch_next(u);
        queue.insert({w2, u, v2, false});

    }
 
    return results;

}


}