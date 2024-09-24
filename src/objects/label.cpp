#include "label.hpp"

label::label(const sf::Vector2f position, std::string plabel, const unsigned int pvalue) : label_(std::move(plabel)), value_(pvalue){
    font_.loadFromFile("/usr/share/fonts/TTF/JetBrainsMono-SemiBold.ttf");
    setFont(font_);
    setCharacterSize(24); // in pixels
    setFillColor(sf::Color::White);
    setPosition(position);
    setOutlineThickness(2.f);
    setOutlineColor(sf::Color::Black);
    setString(label_);
}

void label::update_value() {
    setString(label_ + std::to_string(value_));
}

void label::update_custom_value(const std::string& custom) {
    setString(label_ + custom);
}

void label::increment() {
    value_++;
}

void label::decrement() {
    value_--;
}