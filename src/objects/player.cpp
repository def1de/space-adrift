#include <SFML/Graphics.hpp>
#include <cmath>

class Player : public sf::Sprite {
private:

    sf::RenderWindow& window;
    sf::Clock clock;
    sf::Texture texture;

public:
    explicit Player(sf::RenderWindow& pwindow) : window(pwindow) {
        texture.loadFromFile(ASSETS_DIR "/ship.png");
        setTexture(texture);
        setScale(5.f, 5.f);

        auto textureSize = texture.getSize();
        auto windowSize = window.getSize();

        setOrigin(textureSize.x / 2.0f, textureSize.y / 2.0f);

        setPosition(windowSize.x / 2.0f, windowSize.y / 2.0f);
    }

    void updatePlayer() {
        move();
    }

    void move() {
        float deltaTime = clock.restart().asSeconds();
        rotatate(deltaTime);
    }

    void rotatate(float deltaTime) {
        sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
        sf::Vector2f worldPosition = window.mapPixelToCoords(mousePosition);
        sf::Vector2f playerPosition = getPosition();
        float angle_rad = atan2(worldPosition.y - playerPosition.y, worldPosition.x - playerPosition.x) + M_PI / 2;
        float angle_deg = angle_rad * 180 / M_PI;

        smoothRotate(angle_deg, deltaTime);
    }

    void smoothRotate(float targetAngle, float deltaTime) {
        float currentAngle = getRotation();
        float angleDifference = targetAngle - currentAngle;

        // Normalize the angle difference to the range [-180, 180]
        while (angleDifference > 180) angleDifference -= 360;
        while (angleDifference < -180) angleDifference += 360;

        // Define the rotation speed (degrees per second)
        float rotationSpeed = 120.0f;

        // Calculate the amount to rotate this frame
        float rotationAmount = rotationSpeed * deltaTime;

        // Clamp the rotation amount to the angle difference
        if (std::abs(angleDifference) < rotationAmount) {
            rotationAmount = angleDifference;
        } else {
            rotationAmount *= (angleDifference > 0) ? 1 : -1;
        }

        // Apply the rotation
        setRotation(currentAngle + rotationAmount);
    }
};