#include <SFML/Graphics.hpp>

class Fuel : public sf::Sprite {
private:
    float size = 10.f;
    float speed;
public:
    Fuel(sf::Texture& texture) {
        setTexture(texture);
        speed = rand()%3 + 1;
        setPosition(rand()%1920, rand()%150*-1);
    }

    void move() {
        Sprite::move(0.f, speed);
    }
};