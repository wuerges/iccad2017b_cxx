# Solver for ICCAD 2017 B 

This repository contains a few solutions for the ICCAD 2017 Problem B.
The complete problem description is available inside the contest_page directory.

# Dependencies:

- cmake
- boost > 1.60

# Submodules

This project uses two submodules.
- Rapidcheck: https://github.com/emil-e/rapidcheck
- Cad Geometry: https://github.com/wuerges/cad_geometry


# Steps to compile:

```
clone <repo url>
cd <repo directory>
git submodule update --init
mkdir build
cd build
cmake ..
make
```

# Steps for testing:

```
cd <repo directory>/build
./test/TestParse <testcase path>
```

ICCAD contest official testcases are contained in /testcases/official.
Aditional debug testcases are in /testcases/unnoficial.

# TODO

Almost everything!

