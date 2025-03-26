#include "player.hpp"

#include "../utils/texture_manager.hpp" // Include texture manager header
#include <cmath> // Include cmath for mathematical functions like cos, sin, and pow

#include "../utils/sound_manager.hpp" // Include sound manager header

player::player(sf::RenderWindow& pwindow, projectile_manager& pprojectile_manager)
: animated_sprite(PLAYER_IDLE_TEXTURE_PATH, 64, 64, 0.1f),
// Initialize the player with the idle texture, frame width of 64, frame height of 64, and frame duration of 0.1 seconds
window_(pwindow),
projectile_manager_(pprojectile_manager)
{
    // get the player texture
    sf::Texture texture = *texture_manager::get_texture(PLAYER_IDLE_TEXTURE_PATH);

    setScale(3.f, 3.f); // Scale the player

    const auto texture_size = texture.getSize(); // Get the size of the texture
    const auto window_size = window_.getSize(); // Get the size of the window

    setOrigin(texture_size.x / 2.0f, texture_size.y / 2.0f); // Set the origin of the player to the center

    setPosition(window_size.x / 2.0f, window_size.y / 2.0f); // Set the position of the player to the center of the window

    const sf::FloatRect bounds = getLocalBounds(); // Get the local bounds of the player
    // Local bounds are the dimensions of the player without scaling or transformations

    radius_ = bounds.width / 2; // Set the radius of the player to half of the width

    // This sound buffer is used only to determine the duration of the sound
    sf::SoundBuffer sound_buffer; // Create a sound buffer
    sound_buffer.loadFromFile(ASSETS_DIR "/player_death.ogg"); // Load the sound from the file
    sound_duration_ = sound_buffer.getDuration().asMilliseconds(); // Get the duration of the sound in milliseconds

    // Load the sound
    sound_ = sound_manager::get_sound(ASSETS_DIR "/player_death.ogg");

    // Add the death animation
    add_animation("death", PLAYER_DEATH_TEXTURE_PATH, 64, 64, 0.1f, false);
}

void player::update() {
    if(!is_dead_) { // Only update if the player is alive
        move(); // Move the player
        projectile_manager_.update(); // Update the projectiles
        // Shoot if left mouse button is clicked
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
            if(!was_mouse_pressed_ && shoot_clock_.getElapsedTime().asSeconds() > 1.0f) {
                projectile_manager_.append(getPosition(), getRotation()); // Create a new projectile
                was_mouse_pressed_ = true; // Set the flag to true
                shoot_clock_.restart(); // Restart the shoot clock
            }
        } else {
            was_mouse_pressed_ = false; // Reset the flag
        }
    }
    animated_sprite::update(); // Update the animation despite the player being dead or alive
}

void player::move() {
    const sf::Vector2i mouse_position = sf::Mouse::getPosition(window_); // Get the mouse position
    const sf::Vector2f world_position = window_.mapPixelToCoords(mouse_position); // Convert the mouse position to world coordinates
    const float player_rotation = getRotation() * M_PI / 180.0f - M_PI / 2.0f; // Convert the player rotation to radians

    // Calculate current movement direction
    sf::Vector2f movement(std::cos(player_rotation), std::sin(player_rotation));

    // Normalize the movement vector
    float length = sqrt(movement.x * movement.x + movement.y * movement.y);
    if (length != 0) {
        movement.x /= length;
        movement.y /= length;
    }

    // Calculate the target direction towards the cursor
    const sf::Vector2f player_position = getPosition();
    sf::Vector2f target_direction = world_position - player_position;
    // Normalize the target direction
    length = sqrt(target_direction.x * target_direction.x + target_direction.y * target_direction.y);
    if (length != 0) {
        target_direction.x /= length;
        target_direction.y /= length;
    }

    // Apply a force to gradually change direction
    constexpr float turn_rate = 0.1f; // Adjust this value to control the turning speed
    movement.x = (1 - turn_rate) * movement.x + turn_rate * target_direction.x; // Interpolate between the current and target x-direction
    movement.y = (1 - turn_rate) * movement.y + turn_rate * target_direction.y; // Interpolate between the current and target y-direction

    // Normalize the movement vector again
    length = sqrt(movement.x * movement.x + movement.y * movement.y);
    if (length != 0) {
        movement.x /= length;
        movement.y /= length;
    }

    // Scale by speed
    movement.x *= speed_;
    movement.y *= speed_;

    // Move the sprite
    Sprite::move(movement); // Apply the movement to the player

    // Smoothly rotate the sprite
    const float delta_time = rotation_clock_.restart().asSeconds(); // Get the time since the last frame
    rotate(delta_time, world_position); // Rotate the player
}


void player::rotate(const float delta_time, const sf::Vector2f world_position) {
    const sf::Vector2f player_position = getPosition(); // Get the player position
    // Calculate the angle between the player and the cursor
    const float angle_rad = atan2(world_position.y - player_position.y, world_position.x - player_position.x) + M_PI / 2;
    const float angle_deg = angle_rad * 180 / M_PI; // Convert the angle to degrees

    smooth_rotate(angle_deg, delta_time); // Smoothly rotate the player
}

void player::smooth_rotate(const float target_angle, const float delta_time) {
    const float current_angle = getRotation(); // Get the current rotation angle
    float angle_difference = target_angle - current_angle; // Calculate the angle difference between the target and current angle

    // Normalize the angle difference to the range [-180, 180]
    while (angle_difference > 180) angle_difference -= 360;
    while (angle_difference < -180) angle_difference += 360;

    // Define the rotation speed (degrees per second)
    constexpr float rotation_speed = 120.0f;

    // Calculate the amount to rotate this frame
    float rotation_amount = rotation_speed * delta_time;

    // Clamp the rotation amount to the angle difference
    if (std::abs(angle_difference) < rotation_amount) {
        rotation_amount = angle_difference;
    } else {
        rotation_amount *= (angle_difference > 0) ? 1 : -1;
    }

    // Apply the rotation
    setRotation(current_angle + rotation_amount);
}

// Get player position for camera
sf::Vector2f player::get_player_position() const {
    return getPosition();
}

bool player::check_collision(const float enemy_radius, const sf::Vector2f position) const {
    const sf::Vector2f player_position = getPosition(); // Get the player position
    // Calculate the distance between the player and the position
    const float distance = sqrt(pow(player_position.x - position.x, 2) + pow(player_position.y - position.y, 2));
    return distance < enemy_radius + radius_; // Return true if the distance is less than the sum of the radii
}

float player::get_radius() const {
    return radius_; // Return the radius of the player
}

void player::draw() const {
    window_.draw(*this); // Draw the player
    projectile_manager_.draw(); // Draw the projectiles
}

void player::die() {
    if(!is_dead_) { // Only die if the player is currently alive
        set_animation("death"); // Set the death animation
        sound_->play(); // Play the death sound
        sound_clock_.restart(); // Restart the sound clock
        is_dead_ = true; // Set the flag to true
    }
}

bool player::status() const {
    // Return true if the player is dead and the animation with the sound is finished
    return !(is_dead_ && sound_clock_.getElapsedTime().asMilliseconds() >= sound_duration_ && animated_sprite::is_animation_done());
}