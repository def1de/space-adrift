#include <SFML/Graphics.hpp>
#include "objects/player.cpp"
#include "objects/fuel.cpp"
#include "objects/label.cpp"
#include "objects/meteor.cpp"

int main()
{
    auto window = sf::RenderWindow{ sf::VideoMode::getDesktopMode(), "CMake SFML Project", sf::Style::Fullscreen};
    window.setFramerateLimit(144);

    sf::Texture backgroundTexture;
    if(!backgroundTexture.loadFromFile(ASSETS_DIR "/background.png")) {
        return -1;
    }
    sf::Sprite background(backgroundTexture);
    // Get the original image size
    sf::Vector2u imageSize = backgroundTexture.getSize();

    // Get the viewport size
    sf::Vector2u viewportSize = window.getSize();

    // Calculate the scale factors
    float scaleX = static_cast<float>(viewportSize.x) / imageSize.x;
    float scaleY = static_cast<float>(viewportSize.y) / imageSize.y;

    // Set the sprite's scale to fit the viewport
    background.setScale(scaleX, scaleY);

    sf::RenderTexture uiLayer;
    if(!uiLayer.create(window.getSize().x, window.getSize().y)) {
        return -1;
    };

    sf::Texture playerTexture;
    if(!playerTexture.loadFromFile(ASSETS_DIR "/ship.png")) {
        return -1;
    }
    Player player(window, playerTexture);
    Fuel fuel;
    Meteor meteor;

    Label score(sf::Vector2f(10.f, 10.f), "Score: ", 0);
    Label stamina(sf::Vector2f(10.f, 34.f), "Stamina: ", 0);

    sf::Clock clock;
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
        fuel.move();
        meteor.move();

        if (clock.getElapsedTime().asSeconds()>=1.0) {
            score.increment();
            clock.restart();
        }

        score.update_value();
        stamina.update_custom_value(std::to_string(player.getStamina()));

        window.clear();
        window.draw(background);
        uiLayer.clear(sf::Color::Transparent);

        window.draw(fuel);
        window.draw(meteor);
        window.draw(player);

        uiLayer.draw(score);
        uiLayer.draw(stamina);

        uiLayer.display();
        sf::Sprite uiSprite(uiLayer.getTexture());
        window.draw(uiSprite);
        window.display();
    }
    return 1;
}
