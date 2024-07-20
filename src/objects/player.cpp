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
        float centerX = (windowSize.x / 2.0f) - (textureSize.x * getScale().x / 2.0f);
        float centerY = (windowSize.y / 2.0f) - (textureSize.y * getScale().y / 2.0f);

        setPosition(centerX, centerY);
    }

    void updatePlayer() {
        move();

        if(clock.getElapsedTime().asSeconds() >= 1.0) {
            if(fuel > 0) {
                fuel--;
            }
            else {
                window.close();
            }
            clock.restart();
        }

        if(isDash) checkDash();
    }

    void move() {
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

        // Adjust position to prevent moving out of screen
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