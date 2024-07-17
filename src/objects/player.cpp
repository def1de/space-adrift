#include <SFML/Graphics.hpp>
#include <cmath>

class Player : public sf::Sprite {
private:
    std::string tag = "player";

    int speedX = 5;
    int speedY = 3;
    int fuel = 100;
    bool isDash = false;

    sf::RenderWindow& window;
    sf::Clock clock;
    sf::Clock dashClock;

public:
    explicit Player(sf::RenderWindow& pwindow, const sf::Texture& ptexture) : window(pwindow) {
        setTexture(ptexture);
        setScale(5.f, 5.f);

        auto textureSize = ptexture.getSize();
        auto windowSize = window.getSize();
        float centerX = (windowSize.x / 2.0f) - (textureSize.x * getScale().x / 2.0f);
        float centerY = (windowSize.y / 2.0f) - (textureSize.y * getScale().y / 2.0f);

        setPosition(centerX, centerY);
    }

    void updatePlayer() {
        move();

        if(clock.getElapsedTime().asSeconds() >= 1.0) {
            if(fuel < 100) {
                fuel++;
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
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) && fuel >= 20) {
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
        fuel -= 20;
        isDash = true;
    }

    void checkDash() {
        if(dashClock.getElapsedTime().asSeconds() >= 5) {
            speedY /= 2;
            isDash = false;
        }
    }

    int getStamina() const {
        return fuel;
    }

    std::string getTag() const {
        return tag;
    }

    bool checkCollision(const sf::CircleShape& object) const {
        return getGlobalBounds().intersects(object.getGlobalBounds());
    }

    bool checkCollision(const sf::Sprite& object) const {
        return getGlobalBounds().intersects(object.getGlobalBounds());
    }
};