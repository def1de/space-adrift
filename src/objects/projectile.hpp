#ifndef PROJECTILE_HPP
#define PROJECTILE_HPP

#include "../utils/AnimatedSprite.hpp"

class projectile final : public animated_sprite {
private:
    int speed_ = -10; // speed of the projectile
    sf::Vector2f direction_; // direction of the projectile
    // timer to check if the projectile should be removed
    sf::Clock despawn_timer_;
    // if the projectile should be removed
    bool is_out_ = false;


public:
    projectile(sf::Vector2f position, float angle_deg, const std::string& texture_path, int frame_width, int frame_height, float frame_duration);

    void update() override;
    bool is_out() const;
    void out();
    bool check_collision(float radius, sf::Vector2f position) const;
};

#endif //PROJECTILE_HPP
