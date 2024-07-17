#include <SFML/Graphics.hpp>

class Meteor : public sf::CircleShape {
private:
    std::string tag = "meteor";
    float size = 50.f;
public:
    Meteor() {
        setRadius(size);
        setPosition(rand()%1920, -10.f);
        setFillColor(sf::Color::Black);
    }

    void move() {
        CircleShape::move(0.f, 1.5f);
    }
};