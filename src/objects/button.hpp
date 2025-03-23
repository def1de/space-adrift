#ifndef BUTTON_HPP
#define BUTTON_HPP

#include <SFML/Graphics.hpp> // SFML graphics library
#include <SFML/Audio.hpp> // SFML audio library
#include <functional> // library to do callback functions

class button {
private:
    // struct to hold the button textures for different states
    struct textures {
        sf::Texture idle;
        sf::Texture hover;
        sf::Texture active;
    };

    sf::RenderWindow& window_; // pointer to the window

    // function to call when the button is clicked
    std::function<void()> callback_;

    textures textures_; // button textures
    sf::Sprite sprite_; // button sprite

    // sound buffer for the button click sound
    sf::SoundBuffer sound_buffer_;
    sf::Sound sound_; // sound for the button click
    bool do_callback_ = false; // flag to call the callback function
    sf::Clock sound_clock_; // clock to measure the sound duration
    float sound_duration_; // duration of the sound

public:
    button(sf::RenderWindow& window, float scale, const sf::Vector2f& position,
        const std::string& idle_path, const std::string& hover_path, const std::string& active_path);
    void set_callback(std::function<void()> callback);
    void handle_event(const sf::Event& event);
    void update();
    void draw() const;
};

#endif
