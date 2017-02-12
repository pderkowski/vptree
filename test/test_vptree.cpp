#define CATCH_CONFIG_MAIN

#include "vptree.hpp"
#include "catch.hpp"
#include <vector>
#include <iostream>
#include <numeric>
#include <cmath>
#include <set>
#include <algorithm>
#include <array>
#include <queue>

template<typename Matrix>
void fillPoints(Matrix& m) {
    typedef typename Matrix::value_type Column;
    std::default_random_engine rng(0);
    std::uniform_real_distribution<double> dist(0., 1.);

    for (int i = 0; i < m.size(); ++i) {
        for (int j = 0; j < m[i].size(); ++j) {
            m[i][j] = dist(rng);
        }
    }
}

class NaiveNearestNeighbors {
public:
    typedef std::vector<double> Vector;
    typedef std::vector<Vector> Matrix;

    NaiveNearestNeighbors(const Matrix& points)
    : points_(points)
    { }

    std::pair<std::vector<double>, std::vector<int>> getNearestNeighbors(const Vector& target, int neighborsCount) const {
        std::priority_queue<std::pair<double, int>, std::vector<std::pair<double, int>>, std::greater<std::pair<double, int>>> q;
        vpt::EuclideanMetric<Vector> metric;

        for (int i = 0; i < points_.size(); ++i) {
            auto dist = metric(points_[i], target);
            q.push(std::make_pair(dist, i));
        }

        std::pair<std::vector<double>, std::vector<int>> results;

        while (neighborsCount--) {
            auto top = q.top();
            q.pop();

            results.first.push_back(top.first);
            results.second.push_back(top.second);
        }

        return results;
    }

private:
    std::vector<std::vector<double>> points_;
};

TEST_CASE("Distance: exact") {
    std::vector<double> v1(10000, 0);
    std::vector<double> v2(10000, 1);
    vpt::EuclideanMetric<std::vector<double>> metric;
    REQUIRE(metric(v1, v2) == 100.);
}

TEST_CASE("Distance: approximate") {
    std::vector<double> v1(10000, 0);
    std::vector<double> v2(10000, 0.1);
    vpt::EuclideanMetric<std::vector<double>> metric;
    REQUIRE(metric(v1, v2) == Approx(10.));
}

TEST_CASE("VpTree can be constructed from a vector, std::array or initializer list") {
    auto p1 = std::vector<std::vector<double>>(1000, std::vector<double>(100));
    fillPoints(p1);

    auto p2 = std::array<std::array<double, 100>, 1000>();
    fillPoints(p2);

    vpt::VpTree t1(p1);
    vpt::VpTree t2(p1.begin(), p1.end());

    vpt::VpTree t3(p2);
    vpt::VpTree t4(p2.begin(), p2.end());

    vpt::VpTree t5({
        {1, 2, 3},
        {4, 5, 6},
        {7, 8, 9}
    });

    vpt::VpTree t6(p1);
}

TEST_CASE("VpTree::getNearestNeighbors works in simple case") {
    auto points = std::vector<std::vector<double>>{
        {0, 0, 1},
        {1, 1, 1},
        {2, 0, 0},
        {-1, -1, 0},
        {10, 0, 5}
    };

    vpt::VpTree t1(points);

    std::vector<double> distances;
    std::vector<int> indices;
    std::tie(distances, indices) = t1.getNearestNeighbors({ 0, 0, 0 }, 3);
    REQUIRE((distances.size() == 3 && indices.size() == 3) == true);

    REQUIRE(indices[0] == 0);
    REQUIRE(distances[0] == 1);

    REQUIRE(indices[1] == 3);
    REQUIRE(distances[1] == std::sqrt(2));

    REQUIRE(indices[2] == 1);
    REQUIRE(distances[2] == std::sqrt(3));
}

TEST_CASE("VpTree::getNearestNeighbors returns the same results as naive method for a larger case") {
    auto points = std::vector<std::vector<double>>(1000, std::vector<double>(100));
    fillPoints(points);

    auto tree = vpt::VpTree(points);
    auto naive = NaiveNearestNeighbors(points);

    std::vector<double> naiveDistances, treeDistances;
    std::vector<int> naiveIndices, treeIndices;

    for (const auto& p : points) {
        const auto neighborsCount = 10;
        std::tie(treeDistances, treeIndices) = tree.getNearestNeighbors(p, neighborsCount);
        std::tie(naiveDistances, naiveIndices) = naive.getNearestNeighbors(p, neighborsCount);

        REQUIRE((treeDistances == naiveDistances && treeIndices == naiveIndices) == true);
    }
}

TEST_CASE("VpTree throws an exception when data has inconsistent dimensions") {
    auto points = std::vector<std::vector<double>>{
        {1, 2},
        {3, 4, 5}
    };

    REQUIRE_THROWS_AS([&] () {
        vpt::VpTree t(points);
        t.getNearestNeighbors({0, 0}, 1);
    }(), vpt::DimensionMismatch);
}

TEST_CASE("VpTree throws an exception when query has inconsistent dimensions in respect to data") {
    auto points = std::vector<std::vector<double>>{
        {1, 2, 3},
        {3, 4, 5}
    };

    REQUIRE_THROWS_AS([&] () {
        vpt::VpTree t(points);
        t.getNearestNeighbors({0, 0}, 1);
    }(), vpt::DimensionMismatch);
}