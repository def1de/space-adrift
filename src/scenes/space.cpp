#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "../objects/player.cpp"
#include "../objects/fuel.cpp"
#include "../objects/label.cpp"
#include "../utils/quadtree.hpp"
#include "../objects/meteor.cpp"
#include "../utils/Camera.cpp"
// #include "../utils/AnimatedSprite.cpp"
#include <unordered_set>
#include <unordered_map>

#define DEBUG_CHUNKS 0
#define CHUNK_SIZE 1024

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
    Label fpsText;
    Label position;
    Label chunk;

    sf::Clock scoreClock;
    sf::Clock waveClock;
    int wave = 0;

    sf::Clock fpsClock;
    unsigned int frames = 0;
    float lastTime = 0;

    std::vector<Fuel> fuels;
    std::vector<Meteor> meteors;

    bool isPaused = false;

    sf::Music backgroundMusic;
    std::vector<Projectile> projectiles;

    Quad quadtree;

    sf::RectangleShape dummy;
public:
    Space() :
    window(sf::VideoMode::getDesktopMode(), "CMake SFML Project", sf::Style::Fullscreen),
    player(window),
    camera(window),
    score(sf::Vector2f(10.f, 10.f), "Score: ", 0),
    stamina(sf::Vector2f(10.f, 34.f), "Fuel: ", 0),
    fpsText(sf::Vector2f(10.f, 58.f), "FPS: ", 0),
    position(sf::Vector2f(10.f, 82.f), "Position: ", 0),
    chunk(sf::Vector2f(10.f, 106.f), "Chunk: ", 0),
    quadtree(sf::FloatRect(0, 0, 5000, 5000))
    {
        window.setFramerateLimit(165);

        if(!backgroundTexture.loadFromFile(ASSETS_DIR "/background.png")) {
            std::cout << "Error loading background texture" << std::endl;
        }

        dummy.setSize(sf::Vector2f(100.f, 100.f));
        dummy.setFillColor(sf::Color::White);
        dummy.setPosition(0, 0);

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
                    chunk.position = sf::Vector2f((startX + i) * CHUNK_SIZE, (startY + j) * CHUNK_SIZE);

                    // Randomly spawn meteors
                    int numMeteors = rand() % 3;
                    for (int m = 0; m < numMeteors; ++m) {
                        Meteor meteor(meteorTexture);
                        float meteorX = chunk.position.x + rand() % static_cast<int>(CHUNK_SIZE - meteor.getGlobalBounds().width*2)+meteor.getGlobalBounds().width*2;
                        float meteorY = chunk.position.y + rand() % static_cast<int>(CHUNK_SIZE - meteor.getGlobalBounds().height*2)+meteor.getGlobalBounds().height*2;
                        meteor.setPosition(meteorX, meteorY);
                        chunk.meteors.push_back(meteor);
                    }

                    chunks[coords] = chunk;
#if DEBUG_CHUNKS
                    std::cout << "\n============";
                    std::cout << "\nChecking chunk at (" << i << ", " << j << ")";
                    std::cout << "\nPlayers Coordinates: (" << playerPosition.x << ", " << playerPosition.y << ")";
                    std::cout << "\nStart X: " << startX << " Start Y: " << startY;
                    std::cout << "\nAdded chunk at (" << coords.x << ", " << coords.y << ")";
                    std::cout << "\nChunk coordinates: (" << chunk.position.x << ", " << chunk.position.y << ")";
                    std::cout << "\n============" << std::endl;
#endif
                } else {
                    // iterate through the meteors in the chunk
                    Chunk& chunk = chunks[coords];
                    for (auto& meteor : chunk.meteors) {
                        meteor.update();
                    }
                }

                quadtree.clear();  // Clear the quadtree before each update
                for (auto& pair : chunks) {
                    Chunk& chunk = pair.second;
                    for (auto& meteor : chunk.meteors) {
                        Node* node = new Node(meteor.getPosition(), &meteor, meteor.getRadius());
                        quadtree.insert(node);  // Insert the meteor into the quadtree
                    }
                }
            }
        }

        // quadtree.clear();  // Clear the quadtree before each update
        // for (auto& pair : chunks) {
        //     Chunk& chunk = pair.second;
        //     for (auto& meteor : chunk.meteors) {
        //         Node* node = new Node(meteor.getPosition(), &meteor);
        //         quadtree.insert(node);  // Insert the meteor into the quadtree
        //     }
        // }
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
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
                isPaused = false;
            }
            return;
        }

        std::vector<Meteor*> nearbyMeteors;
        sf::FloatRect playerRange = player.getGlobalBounds();  // Use player's bounding box
        quadtree.retrieve(nearbyMeteors, playerRange);

        // Check for collisions
        for (Meteor* meteor : nearbyMeteors) {
            if (player.checkCollision(meteor->getRadius(), meteor->getPosition())) {
                isPaused = true;
                //Draw a dummy rectangle of the size of collided object at its position to indicate collision
                dummy.setSize(sf::Vector2f(meteor->getLocalBounds().width, meteor->getLocalBounds().height));
                dummy.setPosition(meteor->getPosition());
            }
        }

        projectiles = player.updatePlayer();
        camera.update(player.getPlayerPosition());

        updateChunks();

        sf::Vector2f playerPosition = player.getPosition();
        std::string playerPos = "(" + std::to_string(playerPosition.x) + ", " + std::to_string(playerPosition.y) + ")";
        position.update_custom_value(playerPos);
        std::string chunkPos = "(" + std::to_string(playerPosition.x / CHUNK_SIZE) + ", " + std::to_string(playerPosition.y / CHUNK_SIZE) + ")";
        chunk.update_custom_value(chunkPos);


        if (scoreClock.getElapsedTime().asSeconds() >= 1.0) {
            score.increment();
            scoreClock.restart();
        }

        score.update_value();
        if (player.getFuel() <= 0) {
            isPaused = true;
        }
        stamina.update_custom_value(std::to_string(player.getFuel()));


        fps();
        draw();
    }

    void draw() {
        window.clear();

        drawChunks();

        // Draw game objects
        window.draw(player);
        for (const auto& meteor : meteors) {
            window.draw(meteor);
        }
        for (const auto& fuel : fuels) {
            window.draw(fuel);
        }
        for (const auto& projectile : projectiles) {
            window.draw(projectile);
        }
        window.draw(dummy);

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
        uiLayer.draw(position);
        uiLayer.draw(chunk);
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

    void fps() {
        frames++;
        float currentTime = fpsClock.getElapsedTime().asSeconds();
        if (currentTime - lastTime >= 1.0f) {
            fpsText.setString("FPS: " + std::to_string(frames));
            frames = 0;
            lastTime += 1.0f;
        }
    }
};
