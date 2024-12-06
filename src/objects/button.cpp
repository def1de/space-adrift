#include "button.hpp"

#include "../utils/texture_manager.hpp"

button::button(sf::RenderWindow& window, const float scale, const sf::Vector2f& position, const std::string& idle_path, const std::string& hover_path, const std::string& active_path) :
window_(window)
{
    textures_.idle = texture_manager::get_texture(idle_path);
    textures_.hover = texture_manager::get_texture(hover_path);
    textures_.active = texture_manager::get_texture(active_path);

    sprite_.setTexture(textures_.idle);
    sprite_.setPosition(position);
    sf::FloatRect bounds = sprite_.getLocalBounds();
    sprite_.setScale(scale, scale);
    sprite_.setOrigin(bounds.width / 2.0f, bounds.height / 2.0f);

    sound_buffer_.loadFromFile(ASSETS_DIR "/button_click.ogg");
    sound_.setBuffer(sound_buffer_);
    sound_duration_ = sound_buffer_.getDuration().asMilliseconds();
    sound_.setVolume(100.0f);
}

void button::set_callback(std::function<void()> callback)
{
    callback_ = std::move(callback);
}

void button::handle_event(const sf::Event& event)
{
    // Click event
    if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
        sf::Vector2f mouse = window_.mapPixelToCoords(sf::Mouse::getPosition(window_));
        if (sprite_.getGlobalBounds().contains(mouse)) {
            sprite_.setTexture(textures_.active);
        }
    }

    // Release event
    if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left) {
        sf::Vector2f mouse = window_.mapPixelToCoords(sf::Mouse::getPosition(window_));
        if (sprite_.getGlobalBounds().contains(mouse)) {
            sprite_.setTexture(textures_.hover);
            sound_.play();
            do_callback_ = true;
            sound_clock_.restart();
        } else {
            sprite_.setTexture(textures_.idle);
        }
    }

    // Hover event
    if(event.type == sf::Event::MouseMoved) {
        sf::Vector2f mouse = window_.mapPixelToCoords(sf::Mouse::getPosition(window_));
        if (sprite_.getGlobalBounds().contains(mouse)) {
            sprite_.setTexture(textures_.hover);
        } else {
            sprite_.setTexture(textures_.idle);
        }
    }
}

void button::update() {
    if (do_callback_ && sound_clock_.getElapsedTime().asMilliseconds() >= sound_duration_) {
        do_callback_ = false;
        callback_();
    }
}

void button::draw() const {
    window_.draw(sprite_);
}