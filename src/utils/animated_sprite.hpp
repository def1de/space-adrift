#ifndef ANIMATEDSPRITE_HPP
#define ANIMATEDSPRITE_HPP

#include <memory> // Include the memory library for shared pointers
#include <SFML/Graphics.hpp> // Include the SFML graphics library for the sprite and texture

class animated_sprite : public sf::Sprite {
private:
    struct animation { // Struct to store animation data
        std::string name; // Name of the animation
        std::shared_ptr<sf::Texture> texture; // Texture of the animation
        std::vector<sf::IntRect> frames; // Frames of the animation
        float frame_duration; // Duration of each frame

        bool loop; // Flag to indicate if the animation should loop
    };

    std::vector<animation> animation_list_; // List of all animations
    animation current_animation_; // Current animation
    sf::Clock animation_clock_; // Clock to keep track of the animation time
    size_t current_frame_; // Current frame of the animation

public:
    // Constructor
    animated_sprite(const std::string& texture_path, int frame_width, int frame_height, float frame_duration);

    // Add an animation to the animation pool
    void add_animation(const std::string& name, const std::string &texture_path, int frame_width, int frame_height, float frame_duration, bool loop);

    // Set the current animation from the animation pool
    void set_animation(const std::string& name);

    // Update the animation
    virtual void update(); // virtual keyword allows polymorphism

    // Check if the animation is done
    bool is_animation_done() const;
};

#endif
