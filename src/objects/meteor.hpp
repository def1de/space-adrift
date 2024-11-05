#ifndef METEOR_HPP
#define METEOR_HPP

#include <SFML/Graphics.hpp>

class meteor final : public sf::Sprite {
private:
    float speed_;
    float radius_;
    bool is_out_ = false;
public:
    explicit meteor(const sf::Texture& texture);

    float get_radius() const;

    void update();

    void out();
    bool is_out() const;
};

#endif //METEOR_HPP
