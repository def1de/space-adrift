#ifndef PLAYER_HPP
#define PLAYER_HPP

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#include <SFML/Graphics.hpp>
#include "../utils/projectile_manager.hpp"

class player final: public sf::Sprite {
private:
    int speed_ = 5;
    int radius_;

    sf::RenderWindow& window_;
    sf::Clock rotation_clock_;
    sf::Texture texture_;
    projectile_manager& projectile_manager_;
    bool was_mouse_pressed_ = false;

public:
    explicit player(sf::RenderWindow& pwindow, projectile_manager& pprojectile_manager);

    void update();

    void move();

    void rotate(float delta_time, sf::Vector2f world_position);

    void smooth_rotate(float target_angle, float delta_time);

    // Get player position for camera
    sf::Vector2f get_player_position() const;

    bool check_collision(float enemy_radius, sf::Vector2f position) const;

    float get_radius() const;

    void draw() const;
};

#endif
