#include <SFML/Graphics.hpp>

class Meteor : public sf::CircleShape {
private:
    float size = 50.f;
    float speed;
public:
    Meteor() {
        speed = rand()%3 + 1;
        setRadius(size);
        setPosition(rand()%1920, rand()%150*-1);
        setFillColor(sf::Color::Black);
    }

    void move() {
        CircleShape::move(0.f, speed);
    }
};