#include "tree.h"


/// Represents a tree of MCMC proposals.
///
/// @param x_t The initial value of the sub-chain.
/// @param e_t The log posterior probability of the initial value.
/// @param posterior The curried log posterior function.
/// @param n_procs Number of processors to use.
PosteriorTree::PosteriorTree(
        double x_t,
        double e_t,
        std::function<double(double)> &posterior,
        std::normal_distribution<double> &jump,
        std::mt19937 &generator,
        std::size_t n_procs) {
    // Responsible for initializing the tree, which includes:
    // Drawing jump distances and calculating proposals for each node
    // Setting node children

    // Ensure that each node has a child.
    if ((n_procs - 1) % 2 != 0) {
        throw std::invalid_argument("n_procs must be odd.");
    }


    x_density = e_t;
    target = posterior;

    std::vector<double> current_values, proposals;
    std::deque<double> epsilons;

    // Draw jump distances for calculating proposals.
    for (std::size_t i = 0; i < n_procs; ++i ) {
        epsilons.push_back( jump(generator) );
    }

    // Set up root node outside loop for simplicity.
    auto eps_init = epsilons.front();
    epsilons.pop_front();
    current_values.push_back(x_t);
    proposals.push_back(x_t + eps_init);

    // Propogate current node value to its left child, and proposal to its right
    // child.
    while (!epsilons.empty()) {
        auto eps = epsilons.front();
        epsilons.pop_front();

        std::size_t ii = current_values.size();
        bool is_right = ii % 2 == 0;

        // Do floating point division and cast to an int to avoid
        // overflow for 0 / 2.
        float parent = is_right ? (ii - 2) / 2 : (ii - 1) / 2;
        std::size_t parent_ix(parent);
        double current = is_right ? proposals[parent_ix] : current_values[parent_ix];

        current_values.push_back(current);
        proposals.push_back(current + eps);
    }

    // Set up nodes.
    for (auto cv = current_values.begin(), pr = proposals.begin();
         cv != current_values.end(); ++cv, ++pr) {
        node_ptr p = std::make_unique<Node>(*cv);
        p->proposal = *pr;
        heap.push_back(std::move(p));
    }
}

/// Get each node's current value, theta.
///
/// @return vector of thetas, in index order
std::vector<double> PosteriorTree::get_thetas(void) const {
    std::vector<double> thetas;
    for (auto &ele: heap) {
        thetas.push_back(ele->x);
    }
    return thetas;
}

/// Get each node's proposal.
///
/// @return vector of proposals, in index order
std::vector<double> PosteriorTree::get_proposals(void) const {
    std::vector<double> proposals;
    for (auto &ele: heap) {
        proposals.push_back(ele->proposal);
    }
    return proposals;
}

/// Evaluate the proposal in each node at the posterior.
///
/// The proposal value of each node in the tree is evaluated with
/// the given posterior function in parallel. Node values corresponding
/// to the posterior of the proposal are set accordingly.
///
/// @sideeffect Proposal values are added to each node in the tree.
void PosteriorTree::evaluate(void) {
    #pragma omp parallel for
    for (std::size_t ii = 0; ii < heap.size(); ++ii) {
        double proposal = heap.at(ii)->proposal;
        double density = target(proposal);
        heap.at(ii)->proposal_density = density;
    }
}

/// Walk the tree and collect draws.
///
/// @param dist Uniform distribution from which random covariates `u` are drawn.
/// @param generator PRNG.
///
/// @return Posterior draws and their corresponding densities.
///
/// @sideffect The tree's nodes are updated.
std::pair<std::vector<double>, std::vector<double>> PosteriorTree::draw(
        std::uniform_real_distribution<double> &dist,
        std::mt19937 &generator) {

    evaluate();

    std::vector<double> draws, densities;
    std::size_t ii = 0;

    // Set current draw
    double current_density = x_density;
    double current = heap.front()->x;

    while (ii < heap.size()) {
        Node &p = *heap.at(ii);

        // Get proposal values
        double proposal = p.proposal;
        double proposal_density = p.proposal_density;

        // Determine whether to accept or reject proposal
        double r = proposal_density - current_density;
        double alpha = std::min(0.0, r);
        double u = log( dist(generator) );
        bool accept = u <= r;

        if (accept) {
            // Update current values and go right
            current = proposal;
            current_density = proposal_density;
            ii = ii + 2;
        } else {
            // Propogate current values and go left
            current = current;
            current_density = current_density;
            ii = ii + 1;
        }

        // Append to subchain
        draws.push_back(current);
        densities.push_back(current_density);
    }

    std::pair<std::vector<double>, std::vector<double>> ret(draws, densities);
    return ret;
}

