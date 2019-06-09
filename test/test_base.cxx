#include <base.hpp>
#include <rapidcheck.h>


#include <iostream>
// #include <cassert>

using namespace std;
using namespace iccad;

int main(int argc, char ** argv) {

    PT a(10, 10, 0);
    PT b(20, 20, 0);
    PT c(30, 30, 1);
    PT d(15, 15, 1);
    PT e(0, 0, 1);

    
    cout << "Distance " << distance(c, Shape(a, b)) << '\n';
    // assert(distance(c, Shape(a, b)) == 21);
    
    cout << "Distance " << distance(e, Shape(a, b)) << '\n';
    // assert(distance(e, Shape(a, b)) == 1);

    cout << "Distance " << distance(e, Shape(a, b)) << '\n';
    // RC_ASSERT(distance(e, Shape(a, b)) == 21);

    


    return 0;
}
