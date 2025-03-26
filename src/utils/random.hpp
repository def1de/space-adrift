#ifndef RANDOM_HPP
#define RANDOM_HPP

#include <random> // Include the random library

inline int random(const int a, const int b) {
    // Random using C++11 random library
    std::random_device rd; // obtain a random number from hardware
    std::mt19937 gen(rd()); // seed the generator
    std::uniform_int_distribution<> dis(a, b); // define the range
    const int random = dis(gen); // generate numbers
    return random; // return the random number
}

#endif //RANDOM_HPP
