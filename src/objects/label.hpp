#ifndef LABEL_HPP
#define LABEL_HPP

#include <SFML/Graphics.hpp>

class label final: public sf::Text {
private:
    sf::Font font_;
    std::string label_;
    unsigned int value_ = 0;

public:
    explicit label(sf::Vector2f position, std::string plabel, unsigned int pvalue);

    void update_value();
    void update_custom_value(const std::string& custom);
    void increment();
    void decrement();
};

#endif //LABEL_HPP
