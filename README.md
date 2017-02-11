# vptree
C++ Vantage Point Tree implementation

## Vantage Point Tree

[Vantage Point Tree](https://en.wikipedia.org/wiki/Vantage-point_tree) (or vp tree) is a space partitioning data structure that allows for efficient
querying of nearest neighbors in high dimensional spaces.

## Implementation

The code is based on a great tutorial: http://stevehanov.ca/blog/index.php?id=130
The only significant difference is that a search routine can be performed in parallel.

## Requirements

- C++11 compatible compiler
- cmake 3.0+ (if you want to run tests)

## Instalation

Just copy the header file (vptree.hpp).

For tests:

```
mkdir bin
cd bin
cmake ..
make
make test
```

## Usage

```c++
auto points = std::vector<std::vector<double>>{
    {0, 0, 1},
    {1, 1, 1},
    {2, 0, 0},
    {-1, -1, 0},
    {10, 0, 5}
};

vpt::VpTree t1(points); // create a tree

std::vector<double> distances;
std::vector<int> indices;
std::tie(distances, indices) = t1.getNearestNeighbors({ 0, 0, 0 }, 3); // find 3 neighbors closest to the given point

std::cout << distances[0] << "\n"; // prints 0
std::cout << indices[0] << "\n"; // prints 1
```

