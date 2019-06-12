#include <mst.hpp>
#include <astar.hpp>

#include <set>


namespace iccad {
using std::vector, std::pair, std::map;

MST::MST(int n) : num_neighboors(n) {}

Shape MST::Find(const Shape & x) {
    if(parent.find(x) == parent.end()) {
        parent[x] = x;
    }

    if(parent[x] == x) {
        return x;
    }
    return parent[x] = Find(parent[x]);
}

void MST::Union(const Shape & x, const Shape & y) {
    Shape xr = Find(x);
    Shape yr = Find(y);

    if(rank[xr] > rank[yr]) {
        parent[yr] = xr;
    }
    else if (rank[xr] < rank[yr]){
        parent[xr] = yr;
    }
    else {
        rank[xr]++;
        parent[yr] = xr;
    }
}


vector<pair<Shape, Shape>> MST::run(const Treap & treap, const Treap & obstacles,
const vector<Shape> & shapes, const V1D & boundary) {
    vector<pair<Shape, Shape>> result;
    using std::set, std::tuple;

    set<tuple<int, Shape, Shape>> edges;

    for(const Shape & u : shapes) {
        vector<Shape> vs = treap.collect(u.a, u.b);
        for(auto v : vs) {
        Union(u,v);
        }
    }

    for(const Shape & u : shapes) {
        vector<Shape> vs = treap.neighboors(u, num_neighboors);

        for(Shape & v : vs) {
            if(distance(u, v) > 0) {
                edges.insert({distance(u, v), u, v});
            }
        }
    }
    // sort_by_distance(edges);

    while(!edges.empty()) {
        auto [w, u, v] = *edges.begin();
        edges.erase(edges.begin());

        if(Find(u) != Find(v)) {
            auto a = min(u.a, v.a);
            auto b = min(u.b, v.b);        
            int new_d = AStar(treap, obstacles, u, v, boundary).run().length();    

            if(obstacles.query(a, b) > 0) {


            }

            Union(u, v);
            // if(distance(u, v) > 0)
            result.push_back({u, v});
        }
    }

    //
    
                
    return result;
}

vector<pair<Shape, Shape>> MST::bad_run(const Treap & treap, 
const vector<Shape> & shapes) {

    vector<pair<Shape, Shape>> result;

    for(const Shape & u : shapes) {
        vector<Shape> vs = treap.collect(u.a, u.b);
        for(auto v : vs) {
        result.push_back({u,v});

        std::cout << "Distance: " << distance(u, v) << " -> " << u << v << "\n";
        }


    }
    return result;
}    

}
