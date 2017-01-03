#!/usr/bin/env Rscript

# Plot trace of chain, saved under `trace.png`.

draws <- rep(NA, 10000L)
i <- 1L

fh <- file("stdin")
open(fh, blocking = TRUE)
while(length(x <- readLines(fh, n = 1L)) > 0L) {
    if (i > length(draws)) {
        draws <- c(draws, rep(NA, 10000L))
    }

    draws[i] <- x
    i <- i + 1L
}
close(fh)


draws <- as.numeric(draws[!is.na(draws)])

n <- length(draws)
post_burnin <- draws[as.integer(n/2):n]
estimate <- round(mean(post_burnin), 4L)

png("trace.png")
plot(seq_along(draws), draws,
     type = "l",
     col = "blue",
     xlab = "Iteration",
     ylab = substitute(mu),
     main = paste0("Posterior estimate: ", estimate))
invisible(dev.off())

