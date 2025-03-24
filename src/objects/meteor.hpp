#ifndef METEOR_HPP
#define METEOR_HPP

#include <SFML/Graphics.hpp>

class meteor final : public sf::Sprite {
private:
    float speed_; // speed of the meteor
    float radius_; // radius of the meteor
    bool is_out_ = false; // if the meteor is out
public:
    explicit meteor(const sf::Texture& texture);

    float get_radius() const;

    void update();

    void out();
    bool is_out() const;
};

#endif //METEOR_HPP
