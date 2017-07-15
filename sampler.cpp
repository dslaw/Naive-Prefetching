#include "dist.h"
#include "tree.h"

#include <fstream>
#include <iomanip>
#include <iostream>
#include <numeric>
#include <sstream>
#include <string>
#include <vector>


std::vector<double> read_data(std::string filename);

int main() {
    const double nu = 0.05;
    const std::size_t N = 3;

    // Read in simulated data.
    std::string path = "R/samples.txt";
    auto xs = read_data(path);
    const double sigma = 3.0; // known parameter

    std::normal_distribution<double> jump_dist(0.0, nu);
    std::uniform_real_distribution<double> runif(0.0, 1.0);
    std::mt19937 gen(13);

    // Curry the posterior.
    std::function<double(double)> posterior = [&](double mu) {
        // Hyperpriors
        const double lambda = 7.0;
        const double phi = 1.0;

        // Likelihood
        std::vector<double> log_ll(xs.size());
        for (auto &x: xs) {
            log_ll.push_back( pdf_normal(x, mu, sigma) );
        }

        auto sum_ll = std::accumulate(log_ll.begin(), log_ll.end(), 0.0);

        // Prior
        auto prior_density = pdf_normal(mu, lambda, phi);

        return prior_density + sum_ll;
    };

    // Initialize the chain.
    const std::size_t n_iter = 10000;
    const double theta_0 = 2.1;

    std::size_t h = 0;
    double current(theta_0),
           density(posterior(theta_0));

    for (std::size_t t = 0; t < n_iter; t+=h) {
        PosteriorTree tree(
            current,
            density,
            posterior,
            jump_dist,
            gen,
            N
        );

        auto nodes = tree.draw(runif, gen);
        auto draws = nodes.first;
        auto densities = nodes.second;

        // Advance chain and write new draws to stdout.
        current = draws.back();
        density = densities.back();

        for (auto &draw: draws) {
            std::cout << std::setprecision(10) << draw << std::endl;
        }

        h = draws.size();
    }
}


std::vector<double> read_data(std::string filename) {
    std::string line;
    std::vector<double> xs;

    std::ifstream fh(filename);

    while (std::getline(fh, line)) {
        std::istringstream input_stream(line);
        double x;
        if (!(input_stream >> x)) {
            break;
        }

        xs.push_back(x);
    }

    return xs;
}

