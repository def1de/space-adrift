#include <SFML/Graphics.hpp>
#include <cmath>

#define MAX_FUEL 10
#define DASH_COST 5

class Player : public sf::Sprite {
private:
    int speedX = 5;
    int speedY = 3;
    int fuel = MAX_FUEL;
    bool isDash = false;

    sf::RenderWindow& window;
    sf::Clock clock;
    sf::Clock dashClock;
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
        float deltaTime = clock.restart().asSeconds();
        rotatate(deltaTime);
        sf::Vector2f movement(0.f, 0.f);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
            movement.y -= speedY;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
            movement.y += speedY;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
            movement.x -= speedX;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
            movement.x += speedX;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) && fuel >= DASH_COST) {
            dash();
        }

        Sprite::move(movement);
        adjustPosition();
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

    // Adjust position to prevent moving out of screen
    void adjustPosition() {
        auto position = getPosition();
        auto globalBounds = getGlobalBounds();
        auto windowSize = window.getSize();

        // Left boundary
        if (position.x < 0) {
            position.x = 0;
        }
        // Right boundary
        else if (position.x + globalBounds.width > windowSize.x) {
            position.x = windowSize.x - globalBounds.width;
        }
        // Top boundary
        if (position.y < 0) {
            position.y = 0;
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
        speedY *= 2;
        fuel -= DASH_COST;
        isDash = true;
    }

    void checkDash() {
        if(dashClock.getElapsedTime().asSeconds() >= 5) {
            speedY /= 2;
            isDash = false;
        }
    }

    int getFuel() const {
        return fuel;
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