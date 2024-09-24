#ifndef RANDOM_HPP
#define RANDOM_HPP

#include <random>

inline int random(const int a, const int b) {
    // Random using C++11 random library
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(a, b);
    const int random = dis(gen);
    return random;
}

#endif //RANDOM_HPP
