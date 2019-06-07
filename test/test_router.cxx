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

using namespace std;
using namespace std::chrono;
using namespace iccad;

int main(int argc, char ** argv) {

    Input i;
    parser::parse_file(i, argv[1]);
    vector<Shape> shapes = get_routed_shapes(i);

    cout << shapes.size() << '\n';

    Router router;
    router.perform_global_routing(shapes);

    return 0;
}
