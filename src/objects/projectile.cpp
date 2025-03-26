#include <cmath> // Include cmath for mathematical functions like cos, sin, and pow
#include "projectile.hpp" // Include the projectile header file

projectile::projectile(const sf::Vector2f position, const float angle_deg, const std::string& texture_path, const int frame_width, const int frame_height, const float frame_duration) : animated_sprite(texture_path, frame_width, frame_height, frame_duration) {
    setScale(3.f, 3.f); // Scale the projectile
    setPosition(position); // Set the position of the projectile to the player's position
    setRotation(angle_deg); // Set the rotation of the projectile to the player's rotation

    const float angle_rad = angle_deg * M_PI / 180.0f + M_PI / 2.0f; // Convert the angle to radians

    direction_ = sf::Vector2f(speed_*std::cos(angle_rad), speed_*std::sin(angle_rad));
}

void projectile::update() {
    if(despawn_timer_.getElapsedTime().asSeconds() > 2) {
        // Mark the projectile for removal after 2 seconds
        is_out_ = true;
    }

    move(direction_); // Move the projectile
    animated_sprite::update(); // Update the animation
}

bool projectile::is_out() const {
    return is_out_; // Return the flag that indicates if the projectile should be removed
}

void projectile::out() {
    is_out_ = true; // Mark the projectile to be removed
}

bool projectile::check_collision(const float radius, const sf::Vector2f position) const {
    const sf::Vector2f projectile_position = getPosition(); // Get the position of the projectile
    // Calculate the distance between the projectile and the position
    const float distance = std::sqrt(std::pow(projectile_position.x - position.x, 2) + std::pow(projectile_position.y - position.y, 2));

    return distance < radius; // Return true if the distance is less than the radius of the object
}