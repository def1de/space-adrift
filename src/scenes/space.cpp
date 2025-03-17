#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "../objects/player.cpp"
#include "../objects/label.cpp"

class Space {

private:
    sf::RenderWindow window;
    sf::Texture backgroundTexture;
    sf::Sprite background;
    sf::RenderTexture uiLayer;
    Player player;

    Label score;

    sf::Clock scoreClock;

    Label fpsText;

    sf::Clock fpsClock;
    unsigned int frames = 0;
    float lastTime = 0;

    sf::Music backgroundMusic;

public:
    Space() :
        window(sf::VideoMode::getDesktopMode(), "CMake SFML Project", sf::Style::Fullscreen),
        player(window),
        score(sf::Vector2f(10.f, 10.f), "Score: ", 0),
        fpsText(sf::Vector2f(10.f, 58.f), "FPS: ", 0)
    {
        window.setFramerateLimit(165);

        if(!backgroundTexture.loadFromFile(ASSETS_DIR "/background.png")) {
            std::cout << "Error loading background texture" << std::endl;
        }
        background.setTexture(backgroundTexture);

        sf::Vector2u imageSize = backgroundTexture.getSize();
        sf::Vector2u viewportSize = window.getSize();
        float scaleX = static_cast<float>(viewportSize.x) / imageSize.x;
        float scaleY = static_cast<float>(viewportSize.y) / imageSize.y;
        background.setScale(scaleX, scaleY);

        uiLayer.create(window.getSize().x, window.getSize().y);

        if(!backgroundMusic.openFromFile(ASSETS_DIR "/soundtrack.ogg")) {
            std::cerr << "Failed to load background music." << std::endl;
        } else {
            backgroundMusic.setLoop(true);
            backgroundMusic.play();
        }
    }

    void run() {
        while (window.isOpen())
        {
            update();
        }
    }

    void update() {
        for (auto event = sf::Event{}; window.pollEvent(event);)
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }
        }


        player.updatePlayer();

        if (scoreClock.getElapsedTime().asSeconds()>=1.0) {
            score.increment();
            scoreClock.restart();
        }

        score.update_value();

        frames++;
        float currentTime = fpsClock.getElapsedTime().asSeconds();
        if (currentTime - lastTime >= 1.0f) {
            fpsText.setString("FPS: " + std::to_string(frames));
            frames = 0;
            lastTime += 1.0f;
        }

        draw();
    }

    void draw() {
        window.clear();

        window.draw(background);
        uiLayer.clear(sf::Color::Transparent);

        window.draw(player);

        uiLayer.draw(score);
        uiLayer.draw(fpsText);

        uiLayer.display();
        sf::Sprite uiSprite(uiLayer.getTexture());
        window.draw(uiSprite);

        window.display();
    }
};
