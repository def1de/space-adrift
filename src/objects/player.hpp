#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <SFML/Graphics.hpp>
#include <vector>
#include "projectile.hpp"

class player final: public sf::Sprite {
private:
    int speed_ = 5;
    int radius_;

    sf::RenderWindow& window_;
    sf::Clock rotation_clock_;
    sf::Texture texture_;
    std::vector<projectile> projectiles_;
    bool was_mouse_pressed_ = false;

public:
    explicit player(sf::RenderWindow& pwindow);

    std::vector<projectile> update_player();

    void move();

    void move_projectiles();

    void rotate(float delta_time, sf::Vector2f world_position);

    void smooth_rotate(float target_angle, float delta_time);

    // Get player position for camera
    sf::Vector2f get_player_position() const;

    bool check_collision(float enemy_radius, sf::Vector2f position) const;

    float get_radius() const;
};

#endif //PLAYER_HPP
