# Simulate draws from a normal distribution

set.seed(13)

mu <- 10
sigma <- 3 # known
n <- 5000L

ys <- rnorm(n, mu, sigma)

write.table(ys, "data.txt", row.names = FALSE, col.names = FALSE)

