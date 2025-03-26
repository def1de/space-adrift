#ifndef BUTTON_HPP
#define BUTTON_HPP

#include <SFML/Graphics.hpp> // Graphics library for the Sprite
#include <SFML/Audio.hpp> // Audio library for the Sound
#include <functional> // Function library for the callback function
#include <memory> // Memory library for the shared pointers

class button {
private:
    struct textures { // Textures for the button
        std::shared_ptr<sf::Texture> idle; // Idle texture
        std::shared_ptr<sf::Texture> hover; // Hover texture
        std::shared_ptr<sf::Texture> active; // Active texture
    };

    sf::RenderWindow& window_; // Reference to the window

    std::function<void()> callback_; // Callback function

    textures textures_; // Textures for the button
    sf::Sprite sprite_; // Sprite for the button

    // Button sounds
    std::shared_ptr<sf::Sound> sound_; // Sound for the button
    bool do_callback_ = false; // Boolean to check if the callback should be called
    sf::Clock sound_clock_; // Clock for the sound
    float sound_duration_; // Duration for the sound

public:
    // Constructor
    button(sf::RenderWindow& window, float scale, const sf::Vector2f& position, const std::string& idle_path, const std::string& hover_path, const std::string& active_path);
    void set_callback(std::function<void()> callback); // Set the callback function
    void handle_event(const sf::Event& event); // Handle the event
    void update(); // Update the button
    void draw() const; // Draw the button
};

#endif
