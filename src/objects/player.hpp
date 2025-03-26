#ifndef PLAYER_HPP
#define PLAYER_HPP

#ifndef M_PI
#define M_PI 3.14159265358979323846 // Define PI if it is not defined
#endif

#define PLAYER_IDLE_TEXTURE_PATH ASSETS_DIR "/player.png" // Define the path to the player idle texture
#define PLAYER_DEATH_TEXTURE_PATH ASSETS_DIR "/player_death.png" // Define the path to the player death texture

#include <SFML/Graphics.hpp> // Include SFML graphics library
#include <SFML/Audio.hpp> // Include SFML audio library
#include "../utils/projectile_manager.hpp" // Include projectile manager header
#include "../utils/animated_sprite.hpp" // Include animated sprite header

class player final: public animated_sprite {
private:
    int speed_ = 4; // Speed of the player
    int radius_; // Radius of the player
    bool is_dead_ = false; // Flag to check if the player is dead

    sf::RenderWindow& window_; // Reference to the game window
    sf::Clock rotation_clock_; // Clock to handle rotation
    sf::Clock shoot_clock_; // Clock to handle shooting
    projectile_manager& projectile_manager_; // Reference to the projectile manager
    bool was_mouse_pressed_ = false; // Flag to check if the mouse was pressed

    sf::Clock sound_clock_; // Clock to handle sound
    int sound_duration_; // Duration of the sound

    std::shared_ptr<sf::Sound> sound_; // Sound pointer

public:
    // Constructor
    // explicit keyword is used to prevent implicit conversions by the compiler
    explicit player(sf::RenderWindow& pwindow, projectile_manager& pprojectile_manager);

    void update() override; // Update the player, overrides the virtual function from the base class

    void move(); // Move the player

    void rotate(float delta_time, sf::Vector2f world_position); // Rotate the player

    void smooth_rotate(float target_angle, float delta_time); // Smoothly apply rotation

    // Get player position for camera
    sf::Vector2f get_player_position() const;

    bool check_collision(float enemy_radius, sf::Vector2f position) const; // Collision detection

    float get_radius() const; // Get the radius of the player

    void draw() const; // Draw the player

    void die(); // Kill the player

    bool status() const; // Check if the player is dead and animation is finished
};

#endif
