#include <cmath>
#include "projectile.hpp"

projectile::projectile(const sf::Vector2f position, const float angle_deg, const std::string& texture_path, const int frame_width, const int frame_height, const float frame_duration) : animated_sprite(texture_path, frame_width, frame_height, frame_duration) {
    setScale(3.f, 3.f);
    setPosition(position);
    setRotation(angle_deg);

    const float angle_rad = angle_deg * M_PI / 180.0f + M_PI / 2.0f;

    direction_ = sf::Vector2f(speed_*std::cos(angle_rad), speed_*std::sin(angle_rad));
}

void projectile::update() {
    move(direction_);
    animated_sprite::update();
}