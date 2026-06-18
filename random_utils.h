#ifndef RANDOM_UTILS_H
#define RANDOM_UTILS_H

#include <cstdlib>
#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

inline double randomUniform() {
    return (double)rand() / (double)RAND_MAX;
}

inline double randomUniformNonZero() {
    return (double)(rand() + 1) / ((double)RAND_MAX + 1);
}

inline double randomNormal(double media, double desvio) {
    double u1 = randomUniformNonZero();
    double u2 = randomUniform();
    double z = sqrt(-2.0 * log(u1)) * cos(2.0 * M_PI * u2);
    return media + desvio * z;
}

inline double randomExponential(double media) {
    double u = randomUniform() + 1e-7;
    return -media * log(u);
}

#endif
