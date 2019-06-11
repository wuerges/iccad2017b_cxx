#include <model.hpp>
#include <parser.hpp>
#include <router.hpp>
#include <iostream>
#include <algo.hpp>
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

    Input i;
    parser::parse_file(i, argv[1]);
    // ofstream fout(argv[2]);
    const vector<Shape> shapes = get_routed_shapes(i);
    const vector<Shape> obstacles = get_obstacles(i);

    // cout << shapes.size() << '\n';
    // for(auto obs: obstacles) cout << "OBS: " << obs << '\n';

    stringstream ss;

    cout << argv[1] << ", 10, O, " << account_route(i, 10, ss, shapes, obstacles) << '\n';
    cout << argv[1] << ", 100, O, "  << account_route(i, 100, ss, shapes, obstacles) << '\n';
    cout << argv[1] << ", 1000, O, "  << account_route(i, 1000, ss, shapes, obstacles) << '\n';
    cout << argv[1] << ", 10, E, "  << account_route(i, 10, ss, shapes, vector<Shape>()) << '\n';
    cout << argv[1] << ", 100, E, "  << account_route(i, 100, ss, shapes, vector<Shape>()) << '\n';
    cout << argv[1] << ", 1000, E, "  << account_route(i, 1000, ss, shapes, vector<Shape>()) << '\n';
    
    return 0;
}
