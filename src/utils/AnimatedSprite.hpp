#ifndef ANIMATEDSPRITE_HPP
#define ANIMATEDSPRITE_HPP

#include <SFML/Graphics.hpp>

class animated_sprite : public sf::Sprite {
private:
    std::vector<sf::IntRect> frames_;
    size_t current_frame_;
    sf::Clock animation_clock_;
    float frame_duration_; // Duration of each frame in seconds
    sf::Texture texture_; // Store texture as a member variable

public:
    animated_sprite(const std::string& texture_path, int frame_width, int frame_height, float frame_duration);

    virtual void update(); // virtual keyword allows polymorphism
};

#endif
