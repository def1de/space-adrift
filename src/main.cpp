#include <SFML/Graphics.hpp>
#include "objects/player.cpp"
#include "objects/fuel.cpp"
#include "objects/label.cpp"
#include "objects/meteor.cpp"

bool isBelowBottomBoundary(const sf::Sprite& object, const sf::RenderWindow& window);

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

    sf::Texture meteorTexture;
    meteorTexture.loadFromFile(ASSETS_DIR "/meteor.png");

    sf::Texture fuelTexture;
    fuelTexture.loadFromFile(ASSETS_DIR "/fuel.png");

    Player player(window);

    Label score(sf::Vector2f(10.f, 10.f), "Score: ", 0);
    Label stamina(sf::Vector2f(10.f, 34.f), "Fuel: ", 0);

    sf::Clock scoreClock;
    sf::Clock waveClock;
    int wave = 0;

    std::vector<Fuel> fuels;
    std::vector<Meteor> meteors;
    while (window.isOpen())
    {
        for (auto event = sf::Event{}; window.pollEvent(event);)
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }
        }

        std::vector<int> fuelsToRemove;
        std::vector<int> meteorsToRemove;

        player.updatePlayer();

        if (scoreClock.getElapsedTime().asSeconds()>=1.0) {
            score.increment();
            scoreClock.restart();
        }

        if(waveClock.getElapsedTime().asSeconds() >= 2.0) {
            waveClock.restart();
            wave++;
            for(int i = 0; i < wave/2; i++) {
                meteors.emplace_back(meteorTexture);
            }
            fuels.emplace_back(fuelTexture);
        }

        score.update_value();
        stamina.update_custom_value(std::to_string(player.getFuel()));

        window.clear();
        window.draw(background);
        uiLayer.clear(sf::Color::Transparent);

        for (int i = fuels.size()-1; i>=0; --i) {
            if (player.checkFuelCollision(fuels[i]) || isBelowBottomBoundary(fuels[i], window)) {
                fuelsToRemove.push_back(i);
            }
            fuels[i].move();
            window.draw(fuels[i]);
        }

        for(int i : fuelsToRemove) {
            fuels.erase(fuels.begin() + i);
        }

        for (int i = meteors.size()-1; i>=0; --i) {
            if (player.checkFuelCollision(meteors[i]) || isBelowBottomBoundary(meteors[i], window)) {
                meteorsToRemove.push_back(i);
            }
            meteors[i].move();
            window.draw(meteors[i]);
        }

        for(int i : meteorsToRemove) {
            meteors.erase(meteors.begin() + i);
        }

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

bool isBelowBottomBoundary(const sf::Sprite& object, const sf::RenderWindow& window) {
    float objectBottomEdge = object.getPosition().y + object.getGlobalBounds().height;
    float viewportHeight = window.getSize().y;

    return objectBottomEdge > viewportHeight;
}