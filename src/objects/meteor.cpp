#include <SFML/Graphics.hpp>

class Meteor : public sf::Sprite {
private:
    float size = 50.f;
    float speed;
public:
    Meteor(sf::Texture& texture) {
        setTexture(texture);
        setScale(5.f, 5.f);
        speed = rand()%3 + 1;
        setPosition(rand()%1920, rand()%150*-1);
    }

    void move() {
        Sprite::move(0.f, speed);
    }
};