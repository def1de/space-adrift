#include <SFML/Graphics.hpp>
#include "objects/player.cpp"
#include "objects/food.cpp"
#include "objects/label.cpp"

int main()
{
    auto window = sf::RenderWindow{ sf::VideoMode::getDesktopMode(), "CMake SFML Project", sf::Style::Fullscreen};
    window.setFramerateLimit(144);

    Player player(window);
    Food food;
    Label score(sf::Vector2f(10.f, 10.f), "Score: ", 0);
    Label stamina(sf::Vector2f(10.f, 34.f), "Stamina: ", 0);

    while (window.isOpen())
    {
        for (auto event = sf::Event{}; window.pollEvent(event);)
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }
        }

        player.updatePlayer();

        if(player.checkCollision(food)) {
            while(player.checkCollision(food)) // Prevent food from spawning inside the player
                food.randomizePosition(); // Randomize the food's position
            score.increment();
        }

        score.update_value();
        stamina.update_custom_value(std::to_string(player.getStamina()));

        window.clear();
        window.draw(food);
        window.draw(player);
        window.draw(score);
        window.draw(stamina);
        window.display();
    }
}
