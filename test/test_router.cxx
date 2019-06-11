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

using namespace std;
using namespace std::chrono;
using namespace iccad;

int main(int argc, char ** argv) {

    Input i;
    parser::parse_file(i, argv[1]);
    ofstream fout(argv[2]);
    vector<Shape> shapes = get_routed_shapes(i);
    vector<Shape> obstacles = get_obstacles(i);

    cout << shapes.size() << '\n';

    Router router(i.spacing, i.viaCost, i.boundary[0], i.boundary[1]);
    router.perform_global_routing(shapes, obstacles, fout);

    return 0;
}
