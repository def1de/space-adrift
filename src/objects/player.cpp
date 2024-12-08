#include "player.hpp"

#include "../utils/texture_manager.hpp"
#include <cmath>

#include "../utils/sound_manager.hpp"

player::player(sf::RenderWindow& pwindow, projectile_manager& pprojectile_manager) : animated_sprite(PLAYER_IDLE_TEXTURE_PATH, 64, 64, 0.1f),
                                                                                     window_(pwindow),
                                                                                     projectile_manager_(pprojectile_manager)
{
    sf::Texture texture = *texture_manager::get_texture(PLAYER_IDLE_TEXTURE_PATH);
    setScale(3.f, 3.f);

    const auto texture_size = texture.getSize();
    const auto window_size = window_.getSize();

    setOrigin(texture_size.x / 2.0f, texture_size.y / 2.0f);

    setPosition(window_size.x / 2.0f, window_size.y / 2.0f);

    const sf::FloatRect bounds = getLocalBounds();
    radius_ = bounds.width / 2;

    sf::SoundBuffer sound_buffer;
    sound_buffer.loadFromFile(ASSETS_DIR "/player_death.ogg");
    sound_duration_ = sound_buffer.getDuration().asMilliseconds();
    sound_ = sound_manager::get_sound(ASSETS_DIR "/player_death.ogg");

    add_animation("death", PLAYER_DEATH_TEXTURE_PATH, 64, 64, 0.1f, false);
}

void player::update() {
    if(!is_dead_) {
        move();
        projectile_manager_.update();
        // Shoot if left mouse button is clicked
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
            if(!was_mouse_pressed_) {
                projectile_manager_.append(getPosition(), getRotation());
                was_mouse_pressed_ = true;
            }
        } else {
            was_mouse_pressed_ = false;
        }
    }
    animated_sprite::update();
}

void player::move() {
    const sf::Vector2i mouse_position = sf::Mouse::getPosition(window_);
    const sf::Vector2f world_position = window_.mapPixelToCoords(mouse_position);
    const float player_rotation = getRotation() * M_PI / 180.0f - M_PI / 2.0f;

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
    length = sqrt(target_direction.x * target_direction.x + target_direction.y * target_direction.y);
    if (length != 0) {
        target_direction.x /= length;
        target_direction.y /= length;
    }

    // Apply a force to gradually change direction
    constexpr float turn_rate = 0.1f; // Adjust this value to control the turning speed
    movement.x = (1 - turn_rate) * movement.x + turn_rate * target_direction.x;
    movement.y = (1 - turn_rate) * movement.y + turn_rate * target_direction.y;

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
    Sprite::move(movement);

    // Smoothly rotate the sprite
    const float delta_time = rotation_clock_.restart().asSeconds();
    rotate(delta_time, world_position);
}


void player::rotate(const float delta_time, const sf::Vector2f world_position) {
    const sf::Vector2f player_position = getPosition();
    const float angle_rad = atan2(world_position.y - player_position.y, world_position.x - player_position.x) + M_PI / 2;
    const float angle_deg = angle_rad * 180 / M_PI;

    smooth_rotate(angle_deg, delta_time);
}

void player::smooth_rotate(const float target_angle, const float delta_time) {
    const float current_angle = getRotation();
    float angle_difference = target_angle - current_angle;

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
    const sf::Vector2f player_position = getPosition();
    const float distance = sqrt(pow(player_position.x - position.x, 2) + pow(player_position.y - position.y, 2));
    return distance < enemy_radius + radius_;
}

float player::get_radius() const {
    return radius_;
}

void player::draw() const {
    window_.draw(*this);
    projectile_manager_.draw();
}

void player::die() {
    if(!is_dead_) {
        set_animation("death");
        sound_->play();
        sound_clock_.restart();
        is_dead_ = true;
    }
}

bool player::status() const {
    return !(is_dead_ && sound_clock_.getElapsedTime().asMilliseconds() >= sound_duration_ && animated_sprite::is_animation_done());
}