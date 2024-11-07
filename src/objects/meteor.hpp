#ifndef METEOR_HPP
#define METEOR_HPP

#include <SFML/Audio.hpp>
#include "../utils/animated_sprite.hpp"

class meteor final : public animated_sprite {
private:
    float speed_;
    float radius_;
    bool is_out_ = false;

    sf::SoundBuffer buffer_;
    sf::Sound sound_;
public:
    explicit meteor();

    float get_radius() const;

    void update() override;

    void out();
    bool is_out() const;
};

#endif //METEOR_HPP
