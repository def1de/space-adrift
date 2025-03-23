#include <cmath>
#include "../utils/AnimatedSprite.cpp"

class Projectile : public AnimatedSprite {
private:
    int speed = -10; // Negative speed to move the projectile up
    sf::Vector2f direction; // Direction of the projectile

public:
    Projectile(sf::Vector2f position, float angle_deg, const std::string& texturePath, int frameWidth, int frameHeight, float frameDuration)
    : AnimatedSprite(texturePath, frameWidth, frameHeight, frameDuration) {
        setScale(3.f, 3.f); // Scale the projectile
        setPosition(position); // Set the position of the projectile
        setRotation(angle_deg); // Set the rotation of the projectile

        float angle_rad = angle_deg * M_PI / 180.0f + M_PI / 2.0f; // Convert the angle to radians

        direction = sf::Vector2f(speed*cos(angle_rad), speed*sin(angle_rad)); // Calculate the direction of the projectile
    }

    void update() {
        move(direction); // Move the projectile
        AnimatedSprite::update(); // Update the animation
    }
};

