
#ifndef MENU_HPP
#define MENU_HPP

#include <SFML/Graphics.hpp> // Include the SFML Graphics library
#include <SFML/Audio.hpp> // Include the SFML Audio library
#include "../objects/button.hpp" // Button class
#include "../utils/animated_sprite.hpp" // Animated sprite class for the planet

class menu {
private:
    sf::RenderWindow& window_; // Reference to the window

    sf::Sprite background_; // Background sprite

    animated_sprite planet_; // Animated sprite for the planet

    sf::Sprite player_; // Player sprite
    float player_angle_ = 0; // Player angle
    float radius_ = 0; // Radius of the player

    sf::Sprite title_; // Title sprite

    button play_button_; // Play button
    button exit_button_; // Exit button

    sf::Music background_music_; // Background music

public:
    explicit menu(sf::RenderWindow& window); // Constructor
    void run(); // Run the menu
    void update(); // Update the menu
    void draw() const; // Draw the menu
};

#endif
