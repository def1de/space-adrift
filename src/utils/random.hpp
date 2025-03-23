#ifndef RANDOM_HPP
#define RANDOM_HPP

#include <random> // include the random library

inline int random(const int a, const int b) {
    std::random_device rd; // obtain a random number from hardware
    std::mt19937 gen(rd()); // seed the generator using the mt19937 algorithm
    std::uniform_int_distribution<> dis(a, b); // define the range
    const int random = dis(gen); // generate the random number
    return random;
}

#endif //RANDOM_HPP
