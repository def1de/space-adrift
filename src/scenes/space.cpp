#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "../objects/player.cpp"
#include "../objects/fuel.cpp"
#include "../objects/label.cpp"
#include "../utils/quadtree.cpp"
#include "../objects/meteor.cpp"
#include "../utils/Camera.cpp"

class Space {

private:
    sf::RenderWindow window;
    sf::Texture backgroundTexture;
    sf::Sprite background;
    sf::RenderTexture uiLayer;
    sf::Texture meteorTexture;
    sf::Texture fuelTexture;
    Player player;
    Camera camera;

    Label score;
    Label stamina;

    sf::Clock scoreClock;
    sf::Clock waveClock;
    int wave = 0;

    Label fpsText;

    sf::Clock fpsClock;
    unsigned int frames = 0;
    float lastTime = 0;

    std::vector<Fuel> fuels;
    std::vector<Meteor> meteors;

    bool isPaused = false;

    Quadtree quadtree;

    sf::Music backgroundMusic;

public:
    Space() :
    window(sf::VideoMode::getDesktopMode(), "CMake SFML Project", sf::Style::Fullscreen),
    player(window),
    camera(window),
    score(sf::Vector2f(10.f, 10.f), "Score: ", 0),
    stamina(sf::Vector2f(10.f, 34.f), "Fuel: ", 0),
    fpsText(sf::Vector2f(10.f, 58.f), "FPS: ", 0),
    quadtree(0, sf::FloatRect(0, 0, window.getSize().x, window.getSize().y))
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
        background.setScale(scaleX*4, scaleY*4);

        uiLayer.create(window.getSize().x, window.getSize().y);

        if (!meteorTexture.loadFromFile(ASSETS_DIR "/meteor.png")) {
            std::cerr << "Failed to load meteor texture." << std::endl;
        } else {
            std::cout << "Meteor texture loaded successfully." << std::endl;
        }

        meteors.emplace_back(meteorTexture);

        if (!fuelTexture.loadFromFile(ASSETS_DIR "/fuel.png")) {
            std::cerr << "Failed to load fuel texture." << std::endl;
        } else {
            std::cout << "Fuel texture loaded successfully." << std::endl;
        }

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
    for (auto event = sf::Event{}; window.pollEvent(event);) {
        if (event.type == sf::Event::Closed) {
            window.close();
        }
    }

    if (isPaused) {
        return;
    }

    std::vector<int> fuelsToRemove;
    std::vector<int> meteorsToRemove;

    player.updatePlayer();
    camera.update(player.getPlayerPosition());

    if (scoreClock.getElapsedTime().asSeconds() >= 1.0) {
        score.increment();
        scoreClock.restart();
    }

    if (waveClock.getElapsedTime().asSeconds() >= 2.0) {
        waveClock.restart();
        wave++;
        for (int i = 0; i < wave / 2; i++) {
            // meteors.emplace_back(meteorTexture); // no quadtree
            // quadtree.insert(meteors.back());
        }
        // fuels.emplace_back(fuelTexture); // no quadtree
    }

    score.update_value();
    if (player.getFuel() <= 0) {
        isPaused = true;
    }
    stamina.update_custom_value(std::to_string(player.getFuel()));

    for (int i = fuels.size() - 1; i >= 0; --i) {
        if (player.checkFuelCollision(fuels[i]) || isBelowBottomBoundary(fuels[i], window)) {
            fuelsToRemove.push_back(i);
        }
        fuels[i].move();
    }

    for (int i : fuelsToRemove) {
        fuels.erase(fuels.begin() + i);
    }

    // std::vector<sf::Sprite> nearbyMeteors;
    // quadtree.retrieve(nearbyMeteors, player);
    // for (int i = meteors.size() - 1; i >= 0; --i) {
    //     isPaused = player.checkMeteorCollision(meteors[i]);
    //     if (isBelowBottomBoundary(meteors[i], window)) {
    //         meteorsToRemove.push_back(i);
    //     }
    //     // meteors[i].move();
    // }

    for (int i : meteorsToRemove) {
        meteors.erase(meteors.begin() + i);
    }

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

        // Move background based on the camera view
        sf::Vector2f viewCenter = camera.getView().getCenter();
        sf::Vector2f viewSize = camera.getView().getSize();
        background.setPosition(viewCenter.x - viewSize.x / 2, viewCenter.y - viewSize.y / 2);

        window.draw(background);

        // Draw game objects
        window.draw(player);
        for (const auto& meteor : meteors) {
            window.draw(meteor);
        }
        for (const auto& fuel : fuels) {
            window.draw(fuel);
        }

        // Draw UI elements using a fixed view
        sf::View originalView = window.getView();
        window.setView(window.getDefaultView());

        uiLayer.clear(sf::Color::Transparent);
        uiLayer.draw(score);
        uiLayer.draw(stamina);
        uiLayer.draw(fpsText);
        uiLayer.display();

        sf::Sprite uiSprite(uiLayer.getTexture());
        window.draw(uiSprite);

        window.setView(originalView);
        window.display();
    }

    static bool isBelowBottomBoundary(const sf::Sprite& object, const sf::RenderWindow& window) {
        float objectUpEdge = object.getPosition().y;
        float viewportHeight = window.getSize().y;

        return objectUpEdge > viewportHeight;
    }
};
