#ifndef SPACE_HPP
#define SPACE_HPP

// C++ standard libraries
#include <iostream> // Handles input and output
#include <SFML/Graphics.hpp> // Handles graphics
#include <SFML/Audio.hpp> // Handles audio

// Project headers
#include "../objects/player.hpp" // Player object
#include "../objects/label.hpp" // Label object
#include "../utils/quadtree.hpp" // Quadtree for collision detection
#include "../objects/meteor.hpp" // Meteor object
#include "../utils/Camera.hpp" // Camera object
#include "../utils/projectile_manager.hpp" // Projectile manager object

// For chunk management
#include <unordered_map> // Hash map

#define CHUNK_SIZE 1024 // Size of each chunk in pixels

class space {
private:
    struct chunk {
        sf::Vector2f position;  // Absolute position of the chunk
        std::vector<meteor> meteors;  // Meteors in this chunk
    };

    struct chunk_coordinates {
        int x; // x coordinate of the chunk
        int y; // y coordinate of the chunk

        // Custom equality operator to compare chunk coordinates
        bool operator==(const chunk_coordinates& other) const {
            return x == other.x && y == other.y;
        }
    };

    // Custom hash function for ChunkCoordinates
    struct chunk_coordinates_hash {
        std::size_t operator()(const chunk_coordinates& key) const {
            return std::hash<int>()(key.x) ^ (std::hash<int>()(key.y) << 1); // XOR hash of x and y
        }
    };
    sf::RenderWindow& window_; // Reference to the window

    quad quadtree_; // Quadtree for collision detection
    std::unordered_map<chunk_coordinates, chunk, chunk_coordinates_hash> chunks_; // Hash map of chunks

    // Background
    sf::Texture background_texture_;
    sf::Music background_music_;

    // Player
    player player_;
    camera camera_;

    // Meteors
    std::vector<meteor> meteors_;

    //Projectile manager
    projectile_manager projectile_manager_;

    // UI
    sf::RenderTexture ui_layer_; // Render texture for UI
    label score_; // Score label
    label fps_text_; // FPS counter
    label position_; // Player position label
    label chunk_; // Player chunk label

    sf::Clock score_clock_; // Clock to count down till sc

    // FPS counter
    sf::Clock fps_clock_; // Clock to count frames
    unsigned int frames_ = 0; // Number of frames in the last second
    float last_time_ = 0; // Time of the last frame

    bool is_paused_ = false; // If the game paused or not

public:
    explicit space(sf::RenderWindow& pwindow); // Constructor

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
