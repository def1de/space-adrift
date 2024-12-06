#ifndef PLAYER_HPP
#define PLAYER_HPP

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#define PLAYER_IDLE_TEXTURE_PATH ASSETS_DIR "/player.png"
#define PLAYER_DEATH_TEXTURE_PATH ASSETS_DIR "/player_death.png"

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "../utils/projectile_manager.hpp"
#include "../utils/animated_sprite.hpp"

class player final: public animated_sprite {
private:
    int speed_ = 4;
    int radius_;
    bool is_dead_ = false;

    sf::RenderWindow& window_;
    sf::Clock rotation_clock_;
    projectile_manager& projectile_manager_;
    bool was_mouse_pressed_ = false;

    sf::Clock sound_clock_;
    int sound_duration_;

    sf::SoundBuffer buffer_;
    sf::Sound sound_;

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

    void die();

    bool status() const;
};

#endif
