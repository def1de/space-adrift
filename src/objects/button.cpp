#include "button.hpp"

button::button(sf::RenderWindow& window, const float scale, const sf::Vector2f& position,
    const std::string& idle_path, const std::string& hover_path, const std::string& active_path) :
window_(window)
{
    // Load textures for each state
    textures_.idle.loadFromFile(idle_path);
    textures_.hover.loadFromFile(hover_path);
    textures_.active.loadFromFile(active_path);

    // set the texture and position
    sprite_.setTexture(textures_.idle);
    sprite_.setPosition(position);

    sf::FloatRect bounds = sprite_.getLocalBounds(); // get the bounds of the sprite
    sprite_.setScale(scale, scale); // scale teh sprite
    sprite_.setOrigin(bounds.width / 2.0f, bounds.height / 2.0f); // set the origin of the sprite to the center

    sound_buffer_.loadFromFile(ASSETS_DIR "/button_click.ogg"); // load the sound buffer
    sound_.setBuffer(sound_buffer_); // set the sound buffer to the sound
    sound_duration_ = sound_buffer_.getDuration().asMilliseconds(); // get the duration of the sound
}

void button::set_callback(std::function<void()> callback)
{
    callback_ = std::move(callback); // move the callback function to the callback_
}

void button::handle_event(const sf::Event& event)
{
    // Click event
    if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
        // get the mouse position
        sf::Vector2f mouse = window_.mapPixelToCoords(sf::Mouse::getPosition(window_));
        // check if the mouse is inside the button
        if (sprite_.getGlobalBounds().contains(mouse)) {
            sprite_.setTexture(textures_.active); // set the texture to active
        }
    }

    // Release event
    if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left) {
        // get the mouse position
        sf::Vector2f mouse = window_.mapPixelToCoords(sf::Mouse::getPosition(window_));
        // check if the mouse is inside the button
        if (sprite_.getGlobalBounds().contains(mouse)) {
            sprite_.setTexture(textures_.hover); // set the texture to hover
            sound_.play(); // play the sound
            do_callback_ = true; // set the flag to call the callback function
            sound_clock_.restart(); // restart the sound clock
        } else {
            // set the texture to idle if the mouse is not inside the button
            sprite_.setTexture(textures_.idle);
        }
    }

    // Hover event
    if(event.type == sf::Event::MouseMoved) {
        // get the mouse position
        sf::Vector2f mouse = window_.mapPixelToCoords(sf::Mouse::getPosition(window_));
        // check if the mouse is inside the button
        if (sprite_.getGlobalBounds().contains(mouse)) {
            sprite_.setTexture(textures_.hover); // set the texture to hover
        } else {
            // set the texture to idle if the mouse is not inside the button
            sprite_.setTexture(textures_.idle);
        }
    }
}

void button::update() {
    // Check if the sound has finished playing and callback is waiting
    if (do_callback_ && sound_clock_.getElapsedTime().asMilliseconds() >= sound_duration_) {
        do_callback_ = false; // reset the flag
        callback_(); // call the callback function
    }
}

void button::draw() const {
    window_.draw(sprite_);
}