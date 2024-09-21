#include <SFML/Graphics.hpp>
#include <cmath>
#include <vector>
#include "../objects/projectile.cpp"

#define MAX_FUEL 10

class Player : public sf::Sprite {
private:
    int speed = 5;
    int fuel = MAX_FUEL;
    int radius;

    sf::RenderWindow& window;
    sf::Clock fuelClock;
    sf::Clock rotationClock;
    sf::Texture texture;


    bool wasMousePressed = false;
    std::vector<Projectile> projectiles;

public:
    explicit Player(sf::RenderWindow& pwindow) : window(pwindow) {
        texture.loadFromFile(ASSETS_DIR "/player.png");
        setTexture(texture);
        setScale(3.f, 3.f);

        auto textureSize = texture.getSize();
        auto windowSize = window.getSize();

        setOrigin(textureSize.x / 2.0f, textureSize.y / 2.0f);

        setPosition(windowSize.x / 2.0f, windowSize.y / 2.0f);

        sf::FloatRect bounds = getGlobalBounds();
        radius = bounds.width / 2;
    }

    std::vector<Projectile> updatePlayer() {
        move();

        for (auto& projectile : projectiles) {
            projectile.update();
        }

        // Shoot if left mouse button is clicked
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
            if(!wasMousePressed) {
                projectiles.emplace_back(getPosition(), getRotation(), ASSETS_DIR "/player_projectile.png", 4, 16, 0.1f);
                wasMousePressed = true;
            }
        } else {
            wasMousePressed = false;
        }

        if(fuelClock.getElapsedTime().asSeconds() >= 1.0) {
            if(fuel > 0) {
                // fuel--;
            }
            fuelClock.restart();
        }
        return projectiles;
    }

    void move() {
        sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
        sf::Vector2f worldPosition = window.mapPixelToCoords(mousePosition);
        float playerRotation = getRotation() * M_PI / 180.0f - M_PI / 2.0f;

        // Calculate current movement direction
        sf::Vector2f movement(cos(playerRotation), sin(playerRotation));

        // Normalize the movement vector
        float length = sqrt(movement.x * movement.x + movement.y * movement.y);
        if (length != 0) {
            movement.x /= length;
            movement.y /= length;
        }

        // Calculate the target direction towards the cursor
        sf::Vector2f playerPosition = getPosition();
        sf::Vector2f targetDirection = worldPosition - playerPosition;
        length = sqrt(targetDirection.x * targetDirection.x + targetDirection.y * targetDirection.y);
        if (length != 0) {
            targetDirection.x /= length;
            targetDirection.y /= length;
        }

        // Apply a force to gradually change direction
        float turnRate = 0.1f; // Adjust this value to control the turning speed
        movement.x = (1 - turnRate) * movement.x + turnRate * targetDirection.x;
        movement.y = (1 - turnRate) * movement.y + turnRate * targetDirection.y;

        // Normalize the movement vector again
        length = sqrt(movement.x * movement.x + movement.y * movement.y);
        if (length != 0) {
            movement.x /= length;
            movement.y /= length;
        }

        // Scale by speed
        movement.x *= speed;
        movement.y *= speed;

        // Move the sprite
        Sprite::move(movement);

        // Smoothly rotate the sprite
        float deltaTime = rotationClock.restart().asSeconds();
        rotatate(deltaTime, worldPosition);
    }

    void rotatate(float deltaTime, sf::Vector2f worldPosition) {
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

    // Get player fuel
    int getFuel() const {
        return fuel;
    }

    // Get player position for camera
    sf::Vector2f getPlayerPosition() const {
        return getPosition();
    }

    bool checkCollision(float enemyRadius, sf::Vector2f position) {
        sf::Vector2f playerPosition = getPosition();
        float distance = sqrt(pow(playerPosition.x - position.x, 2) + pow(playerPosition.y - position.y, 2));
        return distance < enemyRadius + radius;
    }
};