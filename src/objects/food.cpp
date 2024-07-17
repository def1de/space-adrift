#include <SFML/Graphics.hpp>

class Food : public sf::CircleShape {
private:
    float size = 10.f;
public:
    Food() {
        setRadius(size);
        setPosition(500.f, 500.f);
        setFillColor(sf::Color::Blue);
    }

    void randomizePosition() {
        setPosition(rand()%1920, rand()%1080);
    }
};