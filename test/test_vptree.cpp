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

    std::vector<vpt::Result<>> getNearestNeighbors(const Vector& target, int neighborsCount) const {
        std::priority_queue<std::pair<double, int>, std::vector<std::pair<double, int>>, std::greater<std::pair<double, int>>> q;
        vpt::EuclideanMetric<Vector> metric;

        for (int i = 0; i < points_.size(); ++i) {
            auto dist = metric(points_[i], target);
            q.push(std::make_pair(dist, i));
        }

        std::vector<vpt::Result<>> results;

        while (neighborsCount--) {
            auto top = q.top();
            q.pop();

            results.push_back(vpt::Result<>{&(points_[top.second]), top.second, top.first});
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

    vpt::VpTree<std::vector<double>> t1(p1);
    vpt::VpTree<std::vector<double>> t2(p1.begin(), p1.end());

    vpt::VpTree<std::array<double, 100>> t3(p2);
    vpt::VpTree<std::array<double, 100>> t4(p2.begin(), p2.end());

    vpt::VpTree<std::vector<double>> t5({
        {1, 2, 3},
        {4, 5, 6},
        {7, 8, 9}
    });

    vpt::VpTree<> t6(p1);
}

TEST_CASE("VpTree::getNearestNeighbors works in simple case") {
    auto points = std::vector<std::vector<double>>{
        {0, 0, 1},
        {1, 1, 1},
        {2, 0, 0},
        {-1, -1, 0},
        {10, 0, 5}
    };

    vpt::VpTree<std::vector<double>> t1(points);

    auto results = t1.getNearestNeighbors({ 0, 0, 0 }, 3);
    REQUIRE(results.size() == 3);

    REQUIRE(results[0].index == 0);
    REQUIRE(*results[0] == points[0]);
    REQUIRE(results[0].dist == 1);

    REQUIRE(results[1].index == 3);
    REQUIRE(*results[1] == points[3]);
    REQUIRE(results[1].dist == std::sqrt(2));

    REQUIRE(results[2].index == 1);
    REQUIRE(*results[2] == points[1]);
    REQUIRE(results[2].dist == std::sqrt(3));
}

TEST_CASE("VpTree::getNearestNeighbors returns the same results as naive method for a larger case") {
    auto points = std::vector<std::vector<double>>(1000, std::vector<double>(100));
    fillPoints(points);

    auto tree = vpt::VpTree<std::vector<double>>(points);
    auto naive = NaiveNearestNeighbors(points);

    for (const auto& p : points) {
        const auto neighborsCount = 10;
        auto treeResults = tree.getNearestNeighbors(p, neighborsCount);
        auto naiveResults = naive.getNearestNeighbors(p, neighborsCount);

        for (int i = 0; i < neighborsCount; ++i) {
            REQUIRE(treeResults[i] == naiveResults[i]);
        }
    }
}
