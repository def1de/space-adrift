#ifndef METEOR_HPP
#define METEOR_HPP

#include <SFML/Audio.hpp> // Include SFML audio library
#include "../utils/animated_sprite.hpp" // Include animated sprite header

class meteor final : public animated_sprite {
private:
    float speed_; // Speed of the meteor
    float radius_; // Radius of the meteor
    bool is_out_ = false; // Flag to check if the meteor should be removed

    std::shared_ptr<sf::Sound> sound_; // Sound pointer

public:
    // Constructor
    explicit meteor();

    float get_radius() const; // Get the radius of the meteor

    void update() override; // Update the meteor, overrides the virtual function from the base class

    void out(); // Mark the meteor to be removed
    bool is_out() const; // Check if the meteor should be removed
};

#endif //METEOR_HPP
