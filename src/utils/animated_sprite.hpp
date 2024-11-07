#ifndef ANIMATEDSPRITE_HPP
#define ANIMATEDSPRITE_HPP

#include <SFML/Graphics.hpp>

class animated_sprite : public sf::Sprite {
private:
    struct animation {
        std::string name;
        sf::Texture texture;
        std::vector<sf::IntRect> frames;
        float frame_duration;

        bool loop;
    };

    std::vector<animation> animation_list_;
    animation current_animation_;
    sf::Clock animation_clock_;
    size_t current_frame_;

public:
    animated_sprite(const std::string& texture_path, int frame_width, int frame_height, float frame_duration);

    void add_animation(const std::string& name, const std::string &texture_path, int frame_width, int frame_height, float frame_duration, bool loop);

    void set_animation(const std::string& name);

    virtual void update(); // virtual keyword allows polymorphism

    bool is_animation_done() const;
};

#endif
