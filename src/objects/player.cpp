#include <SFML/Graphics.hpp>
#include <cmath>
#include "../utils/AnimatedSprite.cpp"

#define MAX_FUEL 10
#define DASH_COST 5

class Player : public sf::Sprite {
private:
    int speed = 5;
    int fuel = MAX_FUEL;
    bool isDash = false;

    sf::RenderWindow& window;
    sf::Clock clock;
    sf::Clock dashClock;
    sf::Texture texture;

public:
    explicit Player(sf::RenderWindow& pwindow) : window(pwindow) {
        texture.loadFromFile(ASSETS_DIR "/player.png");
        setTexture(texture);
        setScale(5.f, 5.f);

        auto textureSize = texture.getSize();
        auto windowSize = window.getSize();

        setOrigin(textureSize.x / 2.0f, textureSize.y / 2.0f);

        setPosition(windowSize.x / 2.0f, windowSize.y / 2.0f);
    }

    void updatePlayer() {
        move();

        if(clock.getElapsedTime().asSeconds() >= 1.0) {
            if(fuel > 0) {
                // fuel--;
            }
            else {
                window.close();
            }
            clock.restart();
        }

        if(isDash) checkDash();
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
        float deltaTime = clock.restart().asSeconds();
        rotatate(deltaTime, worldPosition);
        // adjustPosition();
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

    // Adjust position to prevent moving out of screen
    void adjustPosition() {
        auto position = getPosition();
        auto globalBounds = getGlobalBounds();
        auto windowSize = window.getSize();

        globalBounds.width = globalBounds.width/2;
        globalBounds.height = globalBounds.height/2;

        // Left boundary
        if (position.x < globalBounds.width) {
            position.x = globalBounds.width;
        }
        // Right boundary
        else if (position.x + globalBounds.width > windowSize.x) {
            position.x = windowSize.x - globalBounds.width;
        }
        // Top boundary
        if (position.y < globalBounds.height) {
            position.y = globalBounds.height;
        }
        // Bottom boundary
        else if (position.y + globalBounds.height > windowSize.y) {
            position.y = windowSize.y - globalBounds.height;
        }

        setPosition(position);
    }

    void dash() {
        if(isDash) return;
        dashClock.restart();
        speed *= 2;
        fuel -= DASH_COST;
        isDash = true;
    }

    void checkDash() {
        if(dashClock.getElapsedTime().asSeconds() >= 5) {
            speed /= 2;
            isDash = false;
        }
    }

    int getFuel() const {
        return fuel;
    }

    sf::Vector2f getPlayerPosition() const {
        return getPosition();
    }

    bool checkCollision(const sf::CircleShape& object) const {
        // Calculate the distance between the centers of the player and food
        sf::Vector2f playerCenter = getPosition() + sf::Vector2f(getGlobalBounds().width/2, getGlobalBounds().height/2);
        float radius = (getGlobalBounds().width+getGlobalBounds().height) / 4;
        sf::Vector2f objectCenter = object.getPosition() + sf::Vector2f(object.getRadius(), object.getRadius());
        float distance = sqrt(pow(playerCenter.x - objectCenter.x, 2) + pow(playerCenter.y - objectCenter.y, 2));

        // Check if the distance is less than the sum of the radii (collision)
        return distance < (radius + object.getRadius());
    }

    bool checkRoundCollision(const sf::Sprite& object) const {
        // Calculate the distance between the centers of the player and food
        sf::Vector2f playerCenter = getPosition() + sf::Vector2f(getGlobalBounds().width/2, getGlobalBounds().height/2);
        float playerRadius = (getGlobalBounds().width+getGlobalBounds().height) / 4;
        sf::Vector2f objectCenter = object.getPosition() + sf::Vector2f(object.getGlobalBounds().width/2, object.getGlobalBounds().height/2);
        float objectRadius = (object.getGlobalBounds().width+object.getGlobalBounds().height) / 4;
        float distance = sqrt(pow(playerCenter.x - objectCenter.x, 2) + pow(playerCenter.y - objectCenter.y, 2));

        // Check if the distance is less than the sum of the radii (collision)
        return distance < (playerRadius + objectRadius);
    }

    bool checkCollision(const sf::Sprite& object) const {
        return getGlobalBounds().intersects(object.getGlobalBounds());
    }

    bool checkFuelCollision(const sf::Sprite& fuel) {
        if(checkCollision(fuel) && this->fuel<MAX_FUEL){
            this->fuel += 5;
            if (this->fuel > MAX_FUEL) this->fuel = MAX_FUEL;
            return true;
        }
        return false;
    }

    bool checkMeteorCollision(const sf::Sprite& meteor) const {
        if(checkRoundCollision(meteor)){
            return true;
        }
        return false;
    }
};