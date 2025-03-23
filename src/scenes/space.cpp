#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "../objects/player.cpp"
#include "../objects/fuel.cpp"
#include "../objects/label.cpp"
#include "../utils/quadtree.cpp"
#include "../objects/meteor.cpp"
#include "../utils/Camera.cpp"
// #include "../utils/AnimatedSprite.cpp"
#include <unordered_set>
#include <unordered_map>

class Space {

private:
    struct Chunk {
        sf::Vector2f position;  // Absolute position of the chunk
        std::vector<Meteor> meteors;  // Meteors in this chunk
    };

    struct ChunkCoordinates {
        int x;
        int y;

        bool operator==(const ChunkCoordinates& other) const {
            return x == other.x && y == other.y;
        }
    };

    // Custom hash function for ChunkCoordinates
    struct ChunkCoordinatesHash {
        std::size_t operator()(const ChunkCoordinates& key) const {
            return std::hash<int>()(key.x) ^ (std::hash<int>()(key.y) << 1);
        }
    };

    // Container for chunks
    std::unordered_map<ChunkCoordinates, Chunk, ChunkCoordinatesHash> chunks;

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
    std::vector<Projectile> projectiles;

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

        uiLayer.create(window.getSize().x, window.getSize().y);

        if (!meteorTexture.loadFromFile(ASSETS_DIR "/meteor.png")) {
            std::cerr << "Failed to load meteor texture." << std::endl;
        } else {
            std::cout << "Meteor texture loaded successfully." << std::endl;
        }

        // the mighty meteor doesn't need to be here anymore but he still is because he is the mighty meteor 3:<
        // meteors.emplace_back(meteorTexture);

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

    void updateChunks() {
        sf::Vector2f playerPosition = player.getPosition();
        sf::Vector2u imageSize = backgroundTexture.getSize();

        // Calculate the number of background tiles needed to cover the viewport
        int tilesX = static_cast<int>(std::ceil(window.getSize().x / static_cast<float>(imageSize.x))) + 2;
        int tilesY = static_cast<int>(std::ceil(window.getSize().y / static_cast<float>(imageSize.y))) + 2;

        // Calculate the starting position of the chunks based on the universe center
        int startX = std::floor(playerPosition.x / imageSize.x);
        int startY = std::floor(playerPosition.y / imageSize.y);

        // Generate new chunks
        for (int i = -tilesX; i <= tilesX; ++i) {
            for (int j = -tilesY; j <= tilesY; ++j) {
                int coordX = startX + i;
                int coordY = startY + j;

                ChunkCoordinates coords = {coordX, coordY};

                // Check if the chunk already exists
                if (chunks.find(coords) == chunks.end()) {
                    Chunk chunk;
                    chunk.position = sf::Vector2f((startX + i) * 1024, (startY + j) * 1024);

                    // Randomly spawn meteors
                    int numMeteors = rand() % 3;
                    for (int m = 0; m < numMeteors; ++m) {
                        Meteor meteor(meteorTexture);
                        float meteorX = chunk.position.x + rand() % static_cast<int>(imageSize.x - meteor.getGlobalBounds().width*2)+meteor.getGlobalBounds().width*2;
                        float meteorY = chunk.position.y + rand() % static_cast<int>(imageSize.y - meteor.getGlobalBounds().height*2)+meteor.getGlobalBounds().height*2;
                        meteor.setPosition(meteorX, meteorY);
                        chunk.meteors.push_back(meteor);
                    }

                    chunks[coords] = chunk;
                    std::cout << "\n============";
                    std::cout << "\nChecking chunk at (" << i << ", " << j << ")";
                    std::cout << "\nPlayers Coordinates: (" << playerPosition.x << ", " << playerPosition.y << ")";
                    std::cout << "\nStart X: " << startX << " Start Y: " << startY;
                    std::cout << "\nAdded chunk at (" << coords.x << ", " << coords.y << ")";
                    std::cout << "\nChunk coordinates: (" << chunk.position.x << ", " << chunk.position.y << ")";
                    std::cout << "\n============" << std::endl;
                } else {
                    // iterate through the meteors in the chunk
                    Chunk& chunk = chunks[coords];
                    for (auto& meteor : chunk.meteors) {
                        meteor.update();
                    }
                }
            }
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

        projectiles = player.updatePlayer();
        camera.update(player.getPlayerPosition());
        updateChunks();

        if (scoreClock.getElapsedTime().asSeconds() >= 1.0) {
            score.increment();
            scoreClock.restart();
            score.update_value();
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

        drawChunks();

        // Draw game objects
        window.draw(player);

        for (const auto& projectile : projectiles) { // iterate through all projectiles
            window.draw(projectile); // draw the projectile
        }

        drawUI();
        window.display();
    }

    void drawChunks() {
        // Draw all background tiles first
        for (const auto& pair : chunks) {
            const Chunk& chunk = pair.second;

            sf::Sprite backgroundTile;
            backgroundTile.setTexture(backgroundTexture);
            backgroundTile.setPosition(chunk.position);
            window.draw(backgroundTile);
        }

        // Draw all meteors after background tiles
        for (const auto& pair : chunks) {
            const Chunk& chunk = pair.second;

            for (const auto& meteor : chunk.meteors) {
                window.draw(meteor);
            }
        }
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
};
