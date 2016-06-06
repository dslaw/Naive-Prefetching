#include "include/tree.h"

#include <iostream>

int main () {
    const double nu = 5.0;
    const std::size_t n = 7;
    double theta = 10.0;
    double foo = 0.0;

    std::function<double(double)> posterior = [&](double t) { return t; };
    std::normal_distribution<double> jump_dist(0.0, nu);
    std::uniform_real_distribution<double> runif(0.0, 1.0);
    std::mt19937 gen;

    auto tree = PosteriorTree(theta, foo,
            posterior,
            jump_dist,
            gen,
            n);

    std::vector<double> thetas = tree.get_thetas();
    std::vector<double> proposals = tree.get_proposals();

    if (thetas.size() != proposals.size()) {
        std::cout << "Egregious error" << std::endl;
        return 1;
    }

    std::cout << "Nodes prior to evaluation" << std::endl;
    for (std::size_t ii = 0; ii < thetas.size(); ++ii) {
        auto theta = thetas[ii];
        auto p = proposals[ii];
        std::cout << theta << "\t" << p << std::endl;
    }

    // Print out path
    auto nodes = tree.draw(runif, gen);
    auto draws = nodes.first;
    auto densities = nodes.second;

    std::cout << "Drawing nodes" << std::endl;
    for (auto ii = 0; ii < draws.size(); ++ii) {
        std::cout << draws[ii] << "\t" << densities[ii] << std::endl;
    }

}

