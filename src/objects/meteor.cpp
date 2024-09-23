#include <SFML/Graphics.hpp>
#include <iostream>

class Meteor : public sf::Sprite {
private:
    float speed;
    float radius;
public:
    explicit Meteor(sf::Texture& texture) {
        setTexture(texture);
        setScale(3.f, 3.f);
        speed = rand()%3 + 1;

        sf::FloatRect bounds = getLocalBounds();
        radius = bounds.width / 2;
        setOrigin(bounds.width / 2, bounds.height / 2);
    }

    float getRadius() const {
        return radius;
    }

    void update() {
        // setRotation(getRotation()+.05f*speed);
        return;
    }
};