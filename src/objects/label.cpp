#include <utility>

#include "SFML/Graphics.hpp"

class Label : public sf::Text {
private:
    sf::Font font;
    std::string label;
    unsigned int value = 0;

public:
    explicit Label(sf::Vector2f position, std::string plabel, unsigned int pvalue) : label(std::move(plabel)), value(pvalue){
        font.loadFromFile("/usr/share/fonts/TTF/JetBrainsMono-SemiBold.ttf");
        setFont(font);
        setCharacterSize(24); // in pixels
        setFillColor(sf::Color::White);
        setPosition(position);
        setOutlineThickness(2.f);
        setOutlineColor(sf::Color::Black);
        setString(label);
    }

    void update_value() {
        setString(label + std::to_string(value));
    }

    void update_custom_value(const std::string& custom) {
        setString(label + custom);
    }

    void increment() {
        value++;
    }

    void decrement() {
        value--;
    }
};
