// Based on http://stevehanov.ca/blog/index.php?id=130 by Steve Hanov

#pragma once

#include <algorithm>
#include <vector>
#include <queue>
#include <limits>
#include <random>
#include <cmath>

namespace vpt {

template<class InputIterator>
double sum(InputIterator begin, InputIterator end) {
    double result = 0;
    for (; begin != end; ++begin) {
        result += *begin;
    }
    return result;
}

template<typename Container>
struct EuclideanMetric {
    double operator() (const Container& v1, const Container& v2) const {
        std::vector<double> diffSquares(v1.size());
        std::transform(v1.begin(), v1.end(), v2.begin(), diffSquares.begin(), [] (double lhs, double rhs) {
            return (lhs - rhs) * (lhs - rhs);
        });
        auto sum = vpt::sum(diffSquares.begin(), diffSquares.end());
        return std::sqrt(sum);
    }
};



class Searcher;


class VpTree {
public:
    typedef std::vector<double> Vector;
    typedef std::function<double(const Vector& v1, const Vector& v2)> Metric;

public:
    template<typename InputIterator>
    explicit VpTree(InputIterator start, InputIterator end, Metric metric = EuclideanMetric<Vector>());

    template<typename Container>
    explicit VpTree(const Container& container, Metric metric = EuclideanMetric<Vector>());
    explicit VpTree(std::initializer_list<Vector> list, Metric metric = EuclideanMetric<Vector>());

    std::pair<std::vector<double>, std::vector<int>> getNearestNeighbors(const Vector& target, int neighborsCount) const;
    template<typename VectorLike>
    std::pair<std::vector<double>, std::vector<int>> getNearestNeighbors(const VectorLike& target, int neighborsCount) const;
    std::pair<std::vector<double>, std::vector<int>> getNearestNeighbors(std::initializer_list<double> target, int neighborsCount) const;

    const Metric getDistance;

private:
    struct Node {
        static const int Leaf = -1;

        Node(int item, double threshold = 0., int left = Leaf, int right = Leaf)
        : item(item), threshold(threshold), left(left), right(right)
        { }

        int item;
        double threshold;
        int left;
        int right;
    };

private:
    typedef std::pair<Vector, int> ItemType;

    std::vector<ItemType> items_;
    std::vector<Node> nodes_;

    std::mt19937 rng_;

    template<typename InputIterator>
    std::vector<ItemType> makeItems(InputIterator start, InputIterator end);

    int makeTree(int lower, int upper);
    void selectRoot(int lower, int upper);
    void partitionByDistance(int lower, int pos, int upper);
    int makeNode(int item);
    Node root() const { return nodes_[0]; }

    friend class Searcher;
};

class Searcher {
private:
    typedef typename VpTree::Vector Vector;
    typedef typename VpTree::Node Node;

public:
    explicit Searcher(const VpTree* tree, const Vector& target, int neighborsCount);

    std::pair<std::vector<double>, std::vector<int>> search();

    struct HeapItem {
        bool operator < (const HeapItem& other) const {
            return dist < other.dist;
        }

        int item;
        double dist;
    };

private:
    void searchInNode(const Node& node);

    const VpTree* tree_;
    Vector target_;
    int neighborsCount_;
    double tau_;
    std::priority_queue<HeapItem> heap_;
};



template<typename InputIterator>
VpTree::VpTree(InputIterator start, InputIterator end, Metric metric)
: getDistance(metric), items_(makeItems(start, end)), nodes_(), rng_() {
    std::random_device rd;
    rng_.seed(rd());
    nodes_.reserve(items_.size());
    makeTree(0, items_.size());
}

template<typename Container>
VpTree::VpTree(const Container& container, Metric metric)
: VpTree(container.begin(), container.end(), metric)
{ }

VpTree::VpTree(std::initializer_list<Vector> list, Metric metric)
: VpTree(list.begin(), list.end(), metric)
{ }

int VpTree::makeTree(int lower, int upper) {
    if (lower >= upper) {
        return Node::Leaf;
    } else if (lower + 1 == upper) {
        return makeNode(lower);
    } else {
        selectRoot(lower, upper);
        int median = (upper + lower) / 2;
        partitionByDistance(lower, median, upper);
        auto node = makeNode(lower);
        nodes_[node].threshold = getDistance(items_[lower].first, items_[median].first);
        nodes_[node].left = makeTree(lower + 1, median);
        nodes_[node].right = makeTree(median, upper);
        return node;
    }
}

void VpTree::selectRoot(int lower, int upper) {
    std::uniform_int_distribution<int> uni(lower, upper - 1);
    int root = uni(rng_);
    std::swap(items_[lower], items_[root]);
}

void VpTree::partitionByDistance(int lower, int pos, int upper) {
    std::nth_element(
        items_.begin() + lower + 1,
        items_.begin() + pos,
        items_.begin() + upper,
        [lower, this] (const ItemType& i1, const ItemType& i2) {
            return getDistance(items_[lower].first, i1.first) < getDistance(items_[lower].first, i2.first);
        });
}

int VpTree::makeNode(int item) {
    nodes_.push_back(Node(item));
    return nodes_.size() - 1;
}

template<typename InputIterator>
std::vector<std::pair<typename VpTree::Vector, int>> VpTree::makeItems(InputIterator begin, InputIterator end) {
    std::vector<std::pair<Vector, int>> res;
    for (int i = 0; begin != end; ++begin, ++i) {
        res.push_back(std::make_pair(Vector(begin->begin(), begin->end()), i));
    }
    return res;
}

template<typename VectorLike>
std::pair<std::vector<double>, std::vector<int>> VpTree::getNearestNeighbors(const VectorLike& target, int neighborsCount) const {
    return getNearestNeighbors(Vector(target.begin(), target.end()), neighborsCount);
}

std::pair<std::vector<double>, std::vector<int>> VpTree::getNearestNeighbors(std::initializer_list<double> target, int neighborsCount) const {
    return getNearestNeighbors(Vector(target.begin(), target.end()), neighborsCount);
}

std::pair<std::vector<double>, std::vector<int>> VpTree::getNearestNeighbors(const Vector& target, int neighborsCount) const {
    Searcher searcher(this, target, neighborsCount);
    return searcher.search();
}



Searcher::Searcher(const VpTree* tree, const Vector& target, int neighborsCount)
: tree_(tree), target_(target), neighborsCount_(neighborsCount), tau_(std::numeric_limits<double>::max()), heap_()
{ }

std::pair<std::vector<double>, std::vector<int>> Searcher::search() {
    searchInNode(tree_->root());

    std::pair<std::vector<double>, std::vector<int>> results;
    while(!heap_.empty()) {
        results.first.push_back(heap_.top().dist);
        results.second.push_back(tree_->items_[heap_.top().item].second);
        heap_.pop();
    }
    std::reverse(results.first.begin(), results.first.end());
    std::reverse(results.second.begin(), results.second.end());
    return results;
}

void Searcher::searchInNode(const Node& node) {
    double dist = tree_->getDistance(tree_->items_[node.item].first, target_);

    if (dist < tau_) {
        if (heap_.size() == neighborsCount_)
            heap_.pop();

        heap_.push(HeapItem{node.item, dist});

        if (heap_.size() == neighborsCount_)
            tau_ = heap_.top().dist;
    }

    if (dist < node.threshold) {
        if (node.left != Node::Leaf && dist - tau_ <= node.threshold)
            searchInNode(tree_->nodes_[node.left]);

        if (node.right != Node::Leaf && dist + tau_ >= node.threshold)
            searchInNode(tree_->nodes_[node.right]);
    } else {
        if (node.right != Node::Leaf && dist + tau_ >= node.threshold)
            searchInNode(tree_->nodes_[node.right]);

        if (node.left != Node::Leaf && dist - tau_ <= node.threshold)
            searchInNode(tree_->nodes_[node.left]);
    }
}

}