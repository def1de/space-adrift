#ifndef PLAYER_HPP
#define PLAYER_HPP

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#include <SFML/Graphics.hpp>
#include "../utils/projectile_manager.hpp"

class player final: public sf::Sprite {
private:
    int speed_ = 5; // speed of the player
    int radius_; // radius of the player

    sf::RenderWindow& window_; // reference to the window
    sf::Clock rotation_clock_; // clock to measure the time between rotations
    sf::Texture texture_; // texture of the player
    projectile_manager& projectile_manager_; // reference to the projectile manager
    bool was_mouse_pressed_ = false; // if the mouse was pressed

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
