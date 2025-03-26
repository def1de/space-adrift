#include "button.hpp" // Include the button header file

#include "../utils/texture_manager.hpp" // Include the texture manager
#include "../utils/sound_manager.hpp" // Include the sound manager

button::button(sf::RenderWindow& window, const float scale,
    const sf::Vector2f& position, const std::string& idle_path,
    const std::string& hover_path, const std::string& active_path) :
window_(window)
{
    // Load state textures
    textures_.idle = texture_manager::get_texture(idle_path);
    textures_.hover = texture_manager::get_texture(hover_path);
    textures_.active = texture_manager::get_texture(active_path);

    sprite_.setTexture(*textures_.idle); // Set the texture to the idle texture
    sprite_.setPosition(position); // Set the position of the button
    sf::FloatRect bounds = sprite_.getLocalBounds(); // Get the local bounds of the sprite
    sprite_.setScale(scale, scale); // Set the scale of the sprite
    sprite_.setOrigin(bounds.width / 2.0f, bounds.height / 2.0f); // Set the origin of the sprite

    // Sound buffer is only used to get the duration of the sound
    sf::SoundBuffer sound_buffer; // Sound buffer for the button
    sound_buffer.loadFromFile(ASSETS_DIR "/button_click.ogg"); // Load the sound buffer
    sound_duration_ = sound_buffer.getDuration().asMilliseconds(); // Get the duration of the sound buffer

    sound_ = sound_manager::get_sound(ASSETS_DIR "/button_click.ogg"); // Obtain the sound pointer
}

void button::set_callback(std::function<void()> callback)
{
    callback_ = std::move(callback); // Set the callback function to the parameter
}

void button::handle_event(const sf::Event& event)
{
    // Click event
    if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
        sf::Vector2f mouse = window_.mapPixelToCoords(sf::Mouse::getPosition(window_)); // Get the mouse position
        if (sprite_.getGlobalBounds().contains(mouse)) { // Check if the mouse is inside the button
            sprite_.setTexture(*textures_.active); // Set the texture to the active texture
        }
    }

    // Release event
    if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left) {
        sf::Vector2f mouse = window_.mapPixelToCoords(sf::Mouse::getPosition(window_)); // Get the mouse position
        if (sprite_.getGlobalBounds().contains(mouse)) { // Check if the mouse is inside the button
            sprite_.setTexture(*textures_.hover); // Set the texture to the hover texture
            sound_->play(); // Play the sound
            do_callback_ = true; // Set the boolean to call the callback
            sound_clock_.restart(); // Restart the clock
        } else {
            sprite_.setTexture(*textures_.idle); // Set the texture to the idle texture
        }
    }

    // Hover event
    if(event.type == sf::Event::MouseMoved) {
        sf::Vector2f mouse = window_.mapPixelToCoords(sf::Mouse::getPosition(window_)); // Get the mouse position
        if (sprite_.getGlobalBounds().contains(mouse)) { // Check if the mouse is inside the button
            sprite_.setTexture(*textures_.hover); // Set the texture to the hover texture
        } else {
            sprite_.setTexture(*textures_.idle); // Set the texture to the idle texture
        }
    }
}

void button::update() {
    if (do_callback_ && sound_clock_.getElapsedTime().asMilliseconds() >= sound_duration_) {
        do_callback_ = false; // Reset the boolean
        callback_(); // Call the callback function
    }
}

void button::draw() const {
    window_.draw(sprite_); // Draw the button
}