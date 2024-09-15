#include <SFML/Graphics.hpp>
#include <iostream>

class Meteor : public sf::Sprite {
private:
    float size = 50.f;
    float speed;
public:
    explicit Meteor(sf::Texture& texture) {
        setTexture(texture);
        setScale(3.f, 3.f);
        setRotationCenter();
        speed = rand()%3 + 1;
    }

    void setRotationCenter() {
        auto bounds = getLocalBounds();
        setOrigin(bounds.width / 2, bounds.height / 2);
    }

    void update() {
        setRotation(getRotation()+.05f*speed);
    }
};