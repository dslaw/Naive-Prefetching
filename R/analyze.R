#!/usr/bin/env Rscript


x <- read.table('../draws')[, 1L]
t <- seq_along(x)

n <- length(x)
post_burnin <- x[(n/2):n]
estimate <- mean(post_burnin)
estimate <- round(estimate, 4L)

plot(t, x, type = 'l', col = 'blue',
     ylab = substitute(mu),
     main = paste0('Posterior estimate: ', estimate))


