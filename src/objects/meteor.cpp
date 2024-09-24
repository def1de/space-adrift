#include "meteor.hpp"
#include "../utils/random.hpp"

meteor::meteor(const sf::Texture& texture) {
    setTexture(texture);
    setScale(3.f, 3.f);
    speed_ = random(1, 3);

    const sf::FloatRect bounds = getLocalBounds();
    radius_ = bounds.width / 2;
    setOrigin(bounds.width / 2, bounds.height / 2);
}

float meteor::get_radius() const {
    return radius_;
}

void meteor::update() {
    setRotation(getRotation()+.05f*speed_);
}