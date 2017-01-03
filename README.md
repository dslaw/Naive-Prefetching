# Naive-Prefetching

Simple implementation of parallel MCMC via prefetching using a univariate
Normal-Normal model.

Provided that a version of `gcc` that supports OpenMP is installed, the sampler
can be compiled via `make sampler`.

To run the sampler and plot a trace of the resulting chain: `./bin/sampler |
Rscript R/plot.R` (`R` must be installed to generate the plot). The plot will
be available as `trace.png`.

### References

[Brockwell, A., 2005, Parallel Markov Chain Monte Carlo Simulation by
Pre-Fetching](http://repository.cmu.edu/cgi/viewcontent.cgi?article=1149&context=statistics)

