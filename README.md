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
- cmake (if you want to run tests)

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

vpt::VpTree<> t1(points); // create a tree

std::vector<vpt::Result<>> results = t1.getNearestNeighbors({ 0, 0, 0 }, 3); // find 3 neighbors closest to the given point

// index is the position in the original (points) vector
std::cout << results[0].index << "\n"; // prints 0

// dist is the distance from the searched point
std::cout << results[0].dist << "\n"; // prints 1

// item holds the pointer to the point itself
*(results[0].item) == points[0] // true
*results[0] == points[0] // the same as above
```

By default VpTree uses `std::vector<double>` as a representation of multidimensional points, although other
vector-like containers can be specified (for example `std::array`). Another default is the euclidean metric, this can be
changed by giving the `vpt::VpTree` a second template argument. For example:

```c++
std::vector<std::array<double, 100>> points;
// fill points

vpt:VpTree<std::array<double, 100>, SomeOtherMetric> vpt(points);
```

