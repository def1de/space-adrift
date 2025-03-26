#ifndef CAMERA_HPP
#define CAMERA_HPP

// Include the SFML graphics library to work with the view
#include <SFML/Graphics.hpp>

class camera {
private:
    sf::View view_; // The view of the camera
    sf::RenderWindow& window_; // Reference to the game window

public:
    // Constructor
    explicit camera(sf::RenderWindow& pwindow);

    // Update the camera based on the player's position
    void update(const sf::Vector2f& player_position);

    sf::View get_view() const; // Get the camera view
};

#endif
