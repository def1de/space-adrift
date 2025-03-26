#include "label.hpp" // Include the label header

label::label(const sf::Vector2f position, std::string plabel, const unsigned int pvalue) : label_(std::move(plabel)), value_(pvalue){
    font_.loadFromFile(ASSETS_DIR "/font.ttf"); // Load the font
    setFont(font_); // Set the font
    setCharacterSize(24); // Set the character size in pixels
    setFillColor(sf::Color::White); // Set the color of the text
    setPosition(position); // Set the position of the text
    setOutlineThickness(2.f); // Set the thickness of the outline
    setOutlineColor(sf::Color::Black); // Set the color of the outline
    setString(label_); // Set the string of the text
}

void label::update_value() {
    setString(label_ + std::to_string(value_)); // Set the string of the text with the value
}

void label::update_custom_value(const std::string& custom) {
    setString(label_ + custom); // Set the string of the text with the custom string
}

void label::increment() {
    value_++; // Increment the value
}

void label::decrement() {
    value_--; // Decrement the value
}