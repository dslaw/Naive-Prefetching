#include "../include/dist.h"


/// Log normal pdf
///
/// @param x value at which the pdf should be evaluated
/// @param mu mean
/// @param sigma standard deviation
///
/// @return log density of the normal distribution at x
double pdf_normal(double x, double mu, double sigma) {
    double numerator = 1.0 * pow((x - mu), 2);
    double denominator = 2.0 * pow(sigma, 2);
    double m = log(1.0 / sqrt(2 * M_PI));
    double s = (-1.0) * log(sigma);

    double e = numerator / denominator;
    return( m + s - e );
}

