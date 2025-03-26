#ifndef PROJECTILE_MANAGER_HPP
#define PROJECTILE_MANAGER_HPP

#define TEXTURE_PATH ASSETS_DIR "/player_projectile.png" // Path to the projectile texture
#define FRAME_WIDTH 4 // Width of the projectile frame
#define FRAME_HEIGHT 16 // Height of the projectile frame
#define FRAME_DURATION 0.1f // Duration of each frame

#include <SFML/Graphics.hpp> // Include the SFML graphics library
#include <SFML/Audio.hpp> // Include the SFML audio library
#include <vector> // Include the vector library
#include "../objects/projectile.hpp" // Include the projectile class
#include "../utils/quadtree.hpp" // Include the quadtree class

class projectile_manager {
private:
    std::vector<projectile> projectiles_ = {}; // Vector of all currently existing projectiles
    sf::RenderWindow& window_; // Reference to the game window
    quad& quadtree_; // Reference to the quadtree

    std::shared_ptr<sf::Sound> sound_; // Sound of the projectile

public:
    // Constructor
    explicit projectile_manager(sf::RenderWindow& pwindow, quad& pquadtree);

    void append(sf::Vector2f position, float angle_deg); // Append a new projectile to the vector
    void update(); // Update the projectiles
    void draw(); // Draw the projectiles
};

#endif
