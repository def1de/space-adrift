#include <SFML/Graphics.hpp>

class Fuel : public sf::CircleShape {
private:
    std::string tag = "fuel";
    float size = 10.f;
public:
    Fuel() {
        setRadius(size);
        setPosition(rand()%1920, -10.f);
        setFillColor(sf::Color::Green);
    }

    void move() {
        CircleShape::move(0.f, 1.f);
    }
};