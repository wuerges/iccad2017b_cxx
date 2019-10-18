#include <geometry.hpp>
#include <parser.hpp>
#include <router.hpp>
#include <iostream>
#include <treap.hpp>
#include <vector>
#include <random>
#include <algorithm>
#include <chrono>
#include <mst.hpp>
#include <fstream>
#include <sstream>

using namespace std;
using namespace std::chrono;
using namespace iccad;

int account_route(const Input & i, int n, ostream & out, const vector<Shape> & shapes, const vector<Shape> & obstacles) {
    Router router(i.spacing, i.viaCost, i.boundary, n);
    int result = router.perform_global_routing(shapes, obstacles, out);
    return result;
}


int main(int argc, char ** argv) {


    for(int a = 1; a < argc; ++a) {
        Input i;
        parser::parse_file(i, argv[a]);
        // ofstream fout(argv[2]);
        const vector<Shape> shapes = get_routed_shapes(i);
        const vector<Shape> obstacles = get_obstacles(i);

        // cout << shapes.size() << '\n';
        // for(auto obs: obstacles) cout << "OBS: " << obs << '\n';


        // for(int n = 10; n <= 200; n = n * 2) {

        int n = 10;
        auto t1 = high_resolution_clock::now();
        stringstream ss;
        int r1 = account_route(i, n, ss, shapes, obstacles);
        auto t2 = high_resolution_clock::now();
        auto dur1 = chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count();
        cout << argv[a] << ", " << r1 <<", " << dur1 << endl;

    }

    
    return 0;
}
