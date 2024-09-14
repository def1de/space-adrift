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
    std::vector<sf::Sprite> backgrounds;
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

    sf::Music backgroundMusic;

public:
    Space() :
    window(sf::VideoMode::getDesktopMode(), "CMake SFML Project", sf::Style::Fullscreen),
    player(window),
    camera(window),
    score(sf::Vector2f(10.f, 10.f), "Score: ", 0),
    stamina(sf::Vector2f(10.f, 34.f), "Fuel: ", 0),
    fpsText(sf::Vector2f(10.f, 58.f), "FPS: ", 0)
    {
        window.setFramerateLimit(165);

        if(!backgroundTexture.loadFromFile(ASSETS_DIR "/background.png")) {
            std::cout << "Error loading background texture" << std::endl;
        }
        initializeBackground();
        // sf::Sprite background1;
        // background1.setTexture(backgroundTexture);
        // background1.setPosition(0.f, 0.f);
        // backgrounds.push_back(background1);
        //
        // sf::Sprite background2;
        // background2.setTexture(backgroundTexture);
        // background2.setPosition(0.f, backgroundTexture.getSize().y);
        // backgrounds.push_back(background2);
        //
        // sf::Sprite background3;
        // background3.setTexture(backgroundTexture);
        // background3.setPosition(backgroundTexture.getSize().x, 0.f);
        // backgrounds.push_back(background3);

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

    // In the update method, adjust the background's position to create a looping effect
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

        updateBackground();

        if (scoreClock.getElapsedTime().asSeconds() >= 1.0) {
            score.increment();
            scoreClock.restart();
        }

        score.update_value();
        if (player.getFuel() <= 0) {
            isPaused = true;
        }
        stamina.update_custom_value(std::to_string(player.getFuel()));

        for (int i : fuelsToRemove) {
            fuels.erase(fuels.begin() + i);
        }

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

    // In the draw method, draw the background multiple times to ensure it appears continuous
    void draw() {
        window.clear();

        drawBackground();

        // Draw game objects
        window.draw(player);
        for (const auto& meteor : meteors) {
            window.draw(meteor);
        }
        for (const auto& fuel : fuels) {
            window.draw(fuel);
        }

        drawUI();
        window.display();
    }

    // Draw UI elements using a fixed view
    void drawUI() {
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
    }

    static bool isBelowBottomBoundary(const sf::Sprite& object, const sf::RenderWindow& window) {
        float objectUpEdge = object.getPosition().y;
        float viewportHeight = window.getSize().y;

        return objectUpEdge > viewportHeight;
    }

    void initializeBackground() {
        sf::Vector2u imageSize = backgroundTexture.getSize();
        sf::Vector2u viewportSize = window.getSize();
        // Calculate the number of background tiles needed to cover the viewport
        int tilesX = static_cast<int>(std::ceil(viewportSize.x / (imageSize.x))) + 2;
        int tilesY = static_cast<int>(std::ceil(viewportSize.y / (imageSize.y))) + 2;

        // Clear the current background tiles
        backgrounds.clear();

        // Generate the background tiles
        for (int i = -1; i <= tilesX; ++i) {
            for (int j = -1; j <= tilesY; ++j) {
                sf::Sprite backgroundTile;
                backgroundTile.setTexture(backgroundTexture);
                backgroundTile.setPosition(i * imageSize.x, j * imageSize.y);
                backgrounds.push_back(backgroundTile);
            }
        }
    }

    void updateBackground() {
        sf::Vector2f playerPosition = player.getPosition();
        sf::Vector2u imageSize = backgroundTexture.getSize();
        sf::Vector2u viewportSize = window.getSize();

        // Calculate the number of background tiles needed to cover the viewport
        int tilesX = static_cast<int>(std::ceil(viewportSize.x / static_cast<float>(imageSize.x))) + 2;
        int tilesY = static_cast<int>(std::ceil(viewportSize.y / static_cast<float>(imageSize.y))) + 2;

        // Clear the current background tiles
        backgrounds.clear();

        // Calculate the starting position of the background tiles
        // Use fmod in a way that handles negative coordinates
        float startX = std::floor(playerPosition.x / imageSize.x) * imageSize.x - imageSize.x;
        float startY = std::floor(playerPosition.y / imageSize.y) * imageSize.y - imageSize.y;

        // Generate the background tiles
        for (int i = -1; i <= tilesX; ++i) {
            for (int j = -1; j <= tilesY; ++j) {
                sf::Sprite backgroundTile;
                backgroundTile.setTexture(backgroundTexture);
                backgroundTile.setPosition(startX + i * imageSize.x, startY + j * imageSize.y);
                backgrounds.push_back(backgroundTile);
            }
        }
    }

    void drawBackground() {
        for (const auto& background : backgrounds) {
            window.draw(background);
        }
    }
};
