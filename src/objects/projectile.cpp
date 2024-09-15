#include <cmath>
#include "../utils/AnimatedSprite.cpp"

class Projectile : public AnimatedSprite {
private:
    int speed = -10;
    sf::Vector2f direction;

public:
    Projectile(sf::Vector2f position, float angle_deg, const std::string& texturePath, int frameWidth, int frameHeight, float frameDuration) : AnimatedSprite(texturePath, frameWidth, frameHeight, frameDuration) {
        setScale(3.f, 3.f);
        setPosition(position);
        setRotation(angle_deg);

        float angle_rad = angle_deg * M_PI / 180.0f + M_PI / 2.0f;

        direction = sf::Vector2f(speed*cos(angle_rad), speed*sin(angle_rad));

        std::cout << "Projectile created at " << position.x << ", " << position.y;
        std::cout << " with angle " << angle_deg << std::endl;
    }

    void update() {
        move(direction);
        AnimatedSprite::update();
    }
};