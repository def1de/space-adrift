#include <SFML/Graphics.hpp>
#include <cmath>

// Mechanics Description
// Dash: The player can dash by pressing the left shift key. This will increase the player's speed to 6 for 5 seconds, but costs 20 stamina.
// Stamina: The player's stamina will regenerate by 1 every second, up to a maximum of 100.

class Player : public sf::CircleShape {
private:
    float size = 50.f;
    int speed = 3;
    int stamina = 100;
    bool isDash = false;

    sf::RenderWindow& window;
    sf::Clock clock;
    sf::Clock dashClock;

public:
    explicit Player(sf::RenderWindow& pwindow) : window(pwindow){
        setRadius(size);
        setPosition(100.f, 100.f);
        setFillColor(sf::Color::Yellow);
    }

    void updatePlayer() {
        move();

        // Every second update
        if(clock.getElapsedTime().asSeconds()>=1.0) {
            if(stamina<100) {
                stamina++;
            }
            clock.restart();
        }

        if(isDash) checkDash();
    }

    void move() {
        sf::Vector2f movement(0.f, 0.f);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
            movement.y -= 1.f;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
            movement.y += 1.f;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
            movement.x -= 1.f;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
            movement.x += 1.f;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) && stamina>=20) {
            dash();
        }

        CircleShape::move(movement.x * speed, movement.y * speed);

        // Get the player's position and size
        sf::Vector2f position = getPosition();
        float radius = getRadius();
        sf::Vector2u windowSize = window.getSize();

        // Check and adjust the position if outside the viewport
        if (position.x + 2 * radius < 0) { // Left
            position.x = windowSize.x;
        } else if (position.x > windowSize.x) { // Right
            position.x = -2 * radius;
        }

        if (position.y + 2 * radius < 0) { // Top
            position.y = windowSize.y;
        } else if (position.y > windowSize.y) { // Bottom
            position.y = -2 * radius;
        }

        setPosition(position);
    }

    void dash() {
        if(isDash) return;
        dashClock.restart();
        speed = 6;
        stamina -= 20;
        isDash = true;
    }

    void checkDash() {
        if(dashClock.getElapsedTime().asSeconds()>=5) {
            speed = 3;
            isDash = false;
        }
    }

    int getStamina() const {
        return stamina;
    }

    bool checkCollision(const sf::CircleShape& object) const {
        // Calculate the distance between the centers of the player and food
        sf::Vector2f playerCenter = getPosition() + sf::Vector2f(getRadius(), getRadius());
        sf::Vector2f objectCenter = object.getPosition() + sf::Vector2f(object.getRadius(), object.getRadius());
        float distance = sqrt(pow(playerCenter.x - objectCenter.x, 2) + pow(playerCenter.y - objectCenter.y, 2));

        // Check if the distance is less than the sum of the radii (collision)
        return distance < (getRadius() + object.getRadius());
    }
};