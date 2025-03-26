#ifndef PROJECTILE_HPP
#define PROJECTILE_HPP

#include "../utils/animated_sprite.hpp" // Include animated sprite header

class projectile final : public animated_sprite {
private:
    int speed_ = -10; // Negative speed to move up
    sf::Vector2f direction_; // Direction of the projectile
    sf::Clock despawn_timer_; // Timer to despawn the projectile
    bool is_out_ = false; // Flag to check if the projectile is out of the screen

public:
    // Constructor
    projectile(sf::Vector2f position, float angle_deg, const std::string& texture_path, int frame_width, int frame_height, float frame_duration);

    // Update the projectile
    void update() override; // Override means that this function is a virtual function from the base class
    bool is_out() const; // Check if the projectile is out of the game
    void out(); // Set the projectile out of the game
    bool check_collision(float radius, sf::Vector2f position) const; // Collision detection
};

#endif //PROJECTILE_HPP
