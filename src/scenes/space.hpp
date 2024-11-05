#ifndef SPACE_HPP
#define SPACE_HPP

// C++ standard libraries
#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

// Project headers
#include "../objects/player.hpp"
#include "../objects/label.hpp"
#include "../utils/quadtree.hpp"
#include "../objects/meteor.hpp"
#include "../utils/Camera.hpp"
#include "../utils/projectile_manager.hpp"

// For chunk management
#include <unordered_set>
#include <unordered_map>

#define CHUNK_SIZE 1024 // Size of each chunk in pixels

class space {
private:
    struct chunk {
        sf::Vector2f position;  // Absolute position of the chunk
        std::vector<meteor> meteors;  // Meteors in this chunk
    };

    struct chunk_coordinates {
        int x;
        int y;

        bool operator==(const chunk_coordinates& other) const {
            return x == other.x && y == other.y;
        }
    };

    // Custom hash function for ChunkCoordinates
    struct chunk_coordinates_hash {
        std::size_t operator()(const chunk_coordinates& key) const {
            return std::hash<int>()(key.x) ^ (std::hash<int>()(key.y) << 1);
        }
    };

    // Container for chunks

    sf::RenderWindow& window_;

    // Very technical stuff
    quad quadtree_;
    std::unordered_map<chunk_coordinates, chunk, chunk_coordinates_hash> chunks_;

    // Background
    sf::Texture background_texture_;
    sf::Sprite background_;
    std::vector<sf::Sprite> backgrounds_;
    sf::Music background_music_;

    // Player
    player player_;
    camera camera_;

    // Meteors
    sf::Texture meteor_texture_;
    std::vector<meteor> meteors_;

    //Projectiles
    projectile_manager projectile_manager_;

    // UI
    sf::RenderTexture ui_layer_;
    label score_;
    label fps_text_;
    label position_;
    label chunk_;

    sf::Clock score_clock_;

    // FPS counter
    sf::Clock fps_clock_;
    unsigned int frames_ = 0;
    float last_time_ = 0;

    bool is_paused_ = false;

    std::vector<projectile> projectiles_;

public:
    explicit space(sf::RenderWindow& pwindow);

    // Main game loop
    void run();
    // Updates
    bool update();
    void update_chunks();
    void update_quadtree();
    // Helper Functions
    void fps();
    // Drawing functions
    void draw();
    void draw_chunks();
    void draw_ui();
};

#endif //SPACE_HPP
