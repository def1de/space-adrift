#ifndef PROJECTILE_MANAGER_HPP
#define PROJECTILE_MANAGER_HPP

#define TEXTURE_PATH ASSETS_DIR "/player_projectile.png" // Path to the projectile texture
#define FRAME_WIDTH 4 // Width of the individual projectile frame
#define FRAME_HEIGHT 16 // Height of the individual projectile frame
#define FRAME_DURATION 0.1f // Duration of the individual projectile frame

#include <SFML/Graphics.hpp> // For drawing sprites on the window
#include <vector> // For storing projectiles
#include "../objects/projectile.hpp" // For projectile class
#include "../utils/quadtree.hpp" // For quadtree functionality

class projectile_manager {
private:
    std::vector<projectile> projectiles_ = {}; // Vector of projectiles on the scene
    sf::RenderWindow& window_; // Reference to the game window
    quad& quadtree_; // Reference to the quadtree

public:
    explicit projectile_manager(sf::RenderWindow& pwindow, quad& pquadtree);

    void append(sf::Vector2f position, float angle_deg); // Append a new projectile to the scene
    void update(); // Update projectiles
    void draw(); // Draw projectiles on the window
};

#endif
