#ifndef TREE_H__
#define TREE_H__

#include <cmath>
#include <deque>
#include <functional>
#include <memory>
#include <random>
#include <stdexcept>
#include <utility>
#include <vector>


class PosteriorTree {
public:
    PosteriorTree(
            double x_t,
            double e_t,
            std::function<double(double)> &posterior,
            std::normal_distribution<double> &jump,
            std::mt19937 &generator,
            std::size_t n_procs
    );

    std::vector<double> get_thetas(void) const;
    std::vector<double> get_proposals(void) const;
    std::pair<std::vector<double>, std::vector<double>> draw(
            std::uniform_real_distribution<double> &dist,
            std::mt19937 &generator
    );

private:
    struct Node {
        double x, proposal, proposal_density;
        Node(double x) : x(x) {};
    };

    typedef std::unique_ptr<Node> node_ptr;

    double x_density;
    std::vector<node_ptr> heap;
    std::function<double(double)> target;

    void evaluate(void);
};

#endif

