# vptree
C++ Vantage Point Tree implementation with Python bindings

## Vantage Point Tree

[Vantage Point Tree](https://en.wikipedia.org/wiki/Vantage-point_tree) (or vp tree) is a space partitioning data structure that allows for efficient
querying of nearest neighbors in high dimensional spaces.

## Implementation

The code is based on a great tutorial: http://stevehanov.ca/blog/index.php?id=130
I modified it so that the search routine can be run in parallel.
The C++ code is just a single header file. Optional Python bindings can be built.

## Requirements

- C++11 compatible compiler

Additionally, for building Python bindings and running tests:
- cmake 2.8.12+
- python (python-dev or python3-dev package on Debian/Ubuntu)
- pip

## Installation

### C++
Just copy the header file (vptree.hpp). Compile with C++11 or higher, preferably with -fopenmp flag for speedup.

### Python
```
git clone --recursive https://github.com/piotder/vptree.git <your_local_path>
pip install <your_local_path>
```

### Tests
```
mkdir bin
cd bin
cmake ..
make
make test
```

## Usage

### C++
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

auto batch = t1.getNearestNeighborsBatch({{0, 0, 0}, {1, 1, 1}, {0.5, 0.5, 0.5}}, 3); // split the work between threads
batch.first[0] == distances // true
batch.second[0] == indices // true

```
### Python
```Python
from vptree import VpTree

points = [
    [1, 2, 3],
    [4, 5, 6],
    [1.5, 2.5, 3.5]
] # can also be a numpy array

tree = VpTree(points)

distances, indices = tree.getNearestNeighbors([0, 0, 0.5], 2)
batchDistances, batchIndices = tree.getNearestNeighborsBatch([[0, 0, 0.5], [1, 1, 1], [2, 2, 2]], 2) # split the work between threads
distances == batchDistances[0] # True
indices == batchIndices[0] # True
```