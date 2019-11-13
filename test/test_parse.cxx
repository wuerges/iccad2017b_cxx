#include <geometry.hpp>
#include <parser.hpp>
#include <iostream>
#include <vector>
#include <random>
#include <algorithm>
#include <chrono>

using namespace std;
using namespace std::chrono;
using namespace iccad;


int main(int n, char**argv) {

  Input i;
  parser::parse_file(i, argv[1]);

  return 0;
}
