#ifndef LABEL_HPP
#define LABEL_HPP

#include <SFML/Graphics.hpp> // Graphics library for the Text

class label final: public sf::Text {
private:
    sf::Font font_; // Font for the text
    std::string label_; // Label for the text
    unsigned int value_ = 0; // Value for the text

public:
    // Constructor
    explicit label(sf::Vector2f position, std::string plabel, unsigned int pvalue);

    void update_value(); // Update the value of the text
    void update_custom_value(const std::string& custom); // Update the value of the text with a custom string
    void increment(); // Increment the value of the text
    void decrement(); // Decrement the value of the text
};

#endif //LABEL_HPP
