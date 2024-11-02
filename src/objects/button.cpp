#include "button.hpp"

button::button(const float scale, const sf::Vector2f& position, const std::string& idle_path, const std::string& hover_path, const std::string& active_path)
{
    textures_.idle.loadFromFile(idle_path);
    textures_.hover.loadFromFile(hover_path);
    textures_.active.loadFromFile(active_path);

    sprite_.setTexture(textures_.idle);
    sprite_.setPosition(position);
    sf::FloatRect bounds = sprite_.getLocalBounds();
    sprite_.setScale(scale, scale);
    sprite_.setOrigin(bounds.width / 2.0f, bounds.height / 2.0f);
}

void button::set_callback(std::function<void()> callback)
{
    callback_ = std::move(callback);
}

void button::handle_event(const sf::Event& event, const sf::RenderWindow& window)
{
    // Click event
    if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
        sf::Vector2f mouse = window.mapPixelToCoords(sf::Mouse::getPosition(window));
        if (sprite_.getGlobalBounds().contains(mouse)) {
            sprite_.setTexture(textures_.active);
            callback_();
        }
    }

    // Release event
    if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left) {
        sf::Vector2f mouse = window.mapPixelToCoords(sf::Mouse::getPosition(window));
        if (sprite_.getGlobalBounds().contains(mouse)) {
            sprite_.setTexture(textures_.hover);
        } else {
            sprite_.setTexture(textures_.idle);
        }
    }

    // Hover event
    if(event.type == sf::Event::MouseMoved) {
        sf::Vector2f mouse = window.mapPixelToCoords(sf::Mouse::getPosition(window));
        if (sprite_.getGlobalBounds().contains(mouse)) {
            sprite_.setTexture(textures_.hover);
        } else {
            sprite_.setTexture(textures_.idle);
        }
    }
}

void button::draw(sf::RenderWindow& window) const
{
    window.draw(sprite_);
    // window.draw(text_);
}