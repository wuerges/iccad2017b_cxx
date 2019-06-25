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
    ofstream fout(argv[2]);

    if (argc != 3) {
        cout << "usage: " << argv[0] << " <testcase> <output_file_for_solution>\n";
        return -1;
    }
    const vector<Shape> shapes = get_routed_shapes(i);
    const vector<Shape> obstacles = get_obstacles(i);

    int r1 = account_route(i, 20, fout, shapes, obstacles);
    cout << argv[1] << ": " << r1 << '\n';    
    
    return 0;
}
