#include <SFML/Graphics.hpp>
#include <cmath>
#include <ctime>

void move_player(sf::CircleShape& player, const sf::RenderWindow& window) {
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

    player.move(movement.x * 3, movement.y * 3);

    // Get the player's position and size
    sf::Vector2f position = player.getPosition();
    float radius = player.getRadius();
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

    player.setPosition(position);
}

bool checkCollision(const sf::CircleShape& player, const sf::CircleShape& food) {
    // Calculate the distance between the centers of the player and food
    sf::Vector2f playerCenter = player.getPosition() + sf::Vector2f(player.getRadius(), player.getRadius());
    sf::Vector2f foodCenter = food.getPosition() + sf::Vector2f(food.getRadius(), food.getRadius());
    float distance = sqrt(pow(playerCenter.x - foodCenter.x, 2) + pow(playerCenter.y - foodCenter.y, 2));

    // Check if the distance is less than the sum of the radii (collision)
    return distance < (player.getRadius() + food.getRadius());
}

int main()
{
    auto window = sf::RenderWindow{ { 1920u, 1080u }, "CMake SFML Project" };
    window.setFramerateLimit(144);

    sf::CircleShape player(50.f);
    player.setPosition(100.f, 100.f);
    player.setFillColor(sf::Color::Yellow);

    sf::CircleShape food(10.f);
    food.setPosition(500.f, 500.f);
    food.setFillColor(sf::Color::Blue);

    unsigned int score = 0;

    sf::Font font;
    if (!font.loadFromFile("/usr/share/fonts/TTF/JetBrainsMono-SemiBold.ttf")) {
        return 1;
    }

    // Create a text object
    sf::Text scoreText;
    scoreText.setFont(font);
    scoreText.setCharacterSize(24); // in pixels
    scoreText.setFillColor(sf::Color::White);
    scoreText.setPosition(10.f, 10.f);
    scoreText.setOutlineThickness(2.f);
    scoreText.setOutlineColor(sf::Color::Black);

    while (window.isOpen())
    {
        for (auto event = sf::Event{}; window.pollEvent(event);)
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }
        }

        move_player(player, window);

        if(checkCollision(player, food)) {
            food.setPosition(rand()%1920, rand()%1080);
            player.setRadius(player.getRadius()+5);
            score++;
        }

        scoreText.setString("Score: " + std::to_string(score));

        window.clear();
        window.draw(food);
        window.draw(player);
        window.draw(scoreText);
        window.display();
    }
}
