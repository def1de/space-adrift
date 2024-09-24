#ifndef PROJECTILE_HPP
#define PROJECTILE_HPP

#include "../utils/AnimatedSprite.hpp"

class projectile final : public animated_sprite {
private:
    int speed_ = -10;
    sf::Vector2f direction_;

public:
    projectile(sf::Vector2f position, float angle_deg, const std::string& texture_path, int frame_width, int frame_height, float frame_duration);

    void update() override;
};

#endif //PROJECTILE_HPP
