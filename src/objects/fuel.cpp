#include <SFML/Graphics.hpp>

class Fuel : public sf::CircleShape {
private:
    float size = 10.f;
    float speed;
public:
    Fuel() {
        speed = rand()%3 + 1;
        setRadius(size);
        setPosition(rand()%1920, rand()%150*-1);
        setFillColor(sf::Color::Green);
    }

    void move() {
        CircleShape::move(0.f, speed);
    }
};