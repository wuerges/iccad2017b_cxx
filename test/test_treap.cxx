#include <model.hpp>
#include <parser.hpp>
#include <iostream>
#include <algo.hpp>
#include <vector>
#include <cassert>

using namespace std;
using namespace iccad;

int test_treap(const vector<Shape> & shapes)
{
    unique_ptr<Node> root(new Node({0, 0, 0, 0, 1}));

    for (const Shape & s : shapes) {
      root->add(s);
    }

    cout << root->count << '\n';
    root->print();
    //

    for (const Shape & s : shapes) {
      unsigned count = root->query(s[0], s[1]);
      assert(count > 0);
    }
    return 0;
}


int main(int n, char**argv) {


  vector<Shape> shapes;

  shapes.push_back(Shape{PT{10, 10, 0}, PT{20, 20, 0}});
  shapes.push_back(Shape{PT{30, 30, 0}, PT{40, 40, 0}});
  shapes.push_back(Shape{PT{10, 10, 0}, PT{40, 40, 0}});
  shapes.push_back(Shape{PT{0, 0, 0}, PT{20, 20, 0}});
  shapes.push_back(Shape{PT{10, 10, 0}, PT{50, 50, 0}});

  test_treap(shapes);

  return 0;
}
