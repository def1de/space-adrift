#ifndef ANIMATEDSPRITE_HPP
#define ANIMATEDSPRITE_HPP

#include <SFML/Graphics.hpp>


class animated_sprite : public sf::Sprite {
private:
    struct animation {
        std::string name; // name of the animation
        sf::Texture texture; // texture of the animation
        std::vector<sf::IntRect> frames; // frames of the animation
        float frame_duration; // duration of each frame

        bool loop; // whether the animation should loop
    };

    std::vector<animation> animation_list_;
    animation current_animation_;
    sf::Clock animation_clock_;
    size_t current_frame_;

public:
    // constructor
    animated_sprite(const std::string& texture_path, int frame_width, int frame_height, float frame_duration);

    void add_animation(const std::string& name, const std::string &texture_path,
        int frame_width, int frame_height, float frame_duration, bool loop);

    void set_animation(const std::string& name);

    virtual void update(); // virtual keyword allows polymorphism

    bool is_animation_done() const;
};

#endif
