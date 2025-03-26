#include "space.hpp" // Include the header file
#include "../utils/random.hpp" // Random number generator
#include "../utils/texture_manager.hpp" // Texture manager

space::space(sf::RenderWindow& pwindow) :
// Initialize the attributes
window_(pwindow),
quadtree_(sf::FloatRect(0, 0, 5000, 5000)),
projectile_manager_(pwindow, quadtree_),
player_(window_, projectile_manager_),
camera_(window_),
score_(sf::Vector2f(10.f, 10.f), "Score: ", 0),
fps_text_(sf::Vector2f(10.f, 58.f), "FPS: ", 0),
position_(sf::Vector2f(10.f, 82.f), "Position: ", 0),
chunk_(sf::Vector2f(10.f, 106.f), "Chunk: ", 0)
{
    // Load the background texture
    background_texture_ = *texture_manager::get_texture(ASSETS_DIR "/background.png");

    // Create the UI layer
    ui_layer_.create(window_.getSize().x, window_.getSize().y);

    if(!background_music_.openFromFile(ASSETS_DIR "/soundtrack.ogg")) {
        // If the background music does not load, print an error message
        std::cerr << "Failed to load background music." << std::endl;
    } else {
        background_music_.setLoop(true); // Loop the background music
        background_music_.play(); // Play the background music
    }
}

void space::update_chunks() {

    const sf::Vector2f player_position = player_.getPosition(); // Get the player's position
    const sf::Vector2u image_size = background_texture_.getSize(); // Get the size of the background image

    // Calculate the number of background tiles needed to cover the viewport
    const int tiles_x = static_cast<int>(std::ceil(window_.getSize().x / static_cast<float>(image_size.x))) + 2;
    const int tiles_y = static_cast<int>(std::ceil(window_.getSize().y / static_cast<float>(image_size.y))) + 2;

    // Calculate the starting position of the chunks based on the universe center
    const int start_x = std::floor(player_position.x / image_size.x);
    const int start_y = std::floor(player_position.y / image_size.y);

    // Generate new chunks
    for (int i = -tiles_x; i <= tiles_x; ++i) {
        for (int j = -tiles_y; j <= tiles_y; ++j) {
            const int coord_x = start_x + i;
            const int coord_y = start_y + j;

            // Check if the chunk already exists
            if (chunk_coordinates coords = {coord_x, coord_y}; chunks_.find(coords) == chunks_.end()) {
                chunk chunk;
                chunk.position = sf::Vector2f((start_x + i) * CHUNK_SIZE, (start_y + j) * CHUNK_SIZE);

                // Randomly spawn meteors
                const int num_meteors = random(0, 2);
                for (int m = 0; m < num_meteors; ++m) {
                    meteor meteor{}; // Create a new meteor
                    // Randomly position the meteor within the chunk
                    const float meteor_x = chunk.position.x + random(0, CHUNK_SIZE - static_cast<int>(meteor.getGlobalBounds().width));
                    const float meteor_y = chunk.position.y + random(0, CHUNK_SIZE - static_cast<int>(meteor.getGlobalBounds().height));
                    meteor.setPosition(meteor_x, meteor_y); // Set the position of the meteor
                    chunk.meteors.push_back(meteor); // Add the meteor to the chunk
                }

                chunks_[coords] = chunk; // Add the chunk to the hash map
                update_quadtree(); // Update the quadtree after adding a new chunk
            } else {
                // iterate through the meteors in the chunk
                auto&[position, meteors] = chunks_[coords];
                for (auto& meteor : meteors) {
                    meteor.update(); // Update the meteor
                }
            }
        }
    }
}

void space::update_quadtree() {
    quadtree_.clear();  // Clear the quadtree before each update
    for (auto&[fst, chunk] : chunks_) {
        // Get the position and meteors in the chunk
        auto&[position, meteors] = chunk;
        for (auto& meteor : meteors) {
            // Create a new node for each meteor if meteor is not out
            if (!meteor.is_out()) {
                const node* new_node = new node(meteor.getPosition(), &meteor, meteor.get_radius());
                quadtree_.insert(new_node);  // Insert the meteor into the quadtree
            }
        }
    }
}

void space::run() {
    bool is_running = true;
    while (is_running) // Main game loop
    {
        is_running = update(); // Update the game state
    }
    // reset camera view
    window_.setView(window_.getDefaultView());
}

bool space::update() {
    // Handle events
    for (auto event = sf::Event{}; window_.pollEvent(event);) {
        if (event.type == sf::Event::Closed || sf::Keyboard::isKeyPressed(sf::Keyboard::Q)) {
            // Exit the scene if the close button is pressed or Q is pressed
            return false;
        }
    }

    std::vector<meteor*> nearby_meteors; // Vector to store nearby meteors
    const sf::FloatRect player_range = player_.getGlobalBounds();  // Use player's bounding box

    // Retrieve nearby meteors from the quadtree
    quadtree_.retrieve(nearby_meteors, player_range);

    // Check for collisions
    for (const meteor* meteor : nearby_meteors) {
        // Check for collision between player and meteor
        if (!meteor->is_out() && player_.check_collision(meteor->get_radius(), meteor->getPosition())) {
            background_music_.stop(); // Stop the background music
            player_.die(); // Kill the player
        }
    }
    // Update the camera position based on the player's position
    camera_.update(player_.get_player_position());

    update_chunks(); // Update the chunks

    player_.update(); // Update the player

    // Update player position label
    const sf::Vector2f player_position = player_.getPosition();
    const std::string player_pos = "(" + std::to_string(player_position.x) + ", " + std::to_string(player_position.y) + ")";
    position_.update_custom_value(player_pos);
    // Update player chunk label
    const std::string chunk_pos = "(" + std::to_string(player_position.x / CHUNK_SIZE) + ", " + std::to_string(player_position.y / CHUNK_SIZE) + ")";
    chunk_.update_custom_value(chunk_pos);

    // Update the score
    if (score_clock_.getElapsedTime().asSeconds() >= 1.0) {
        // if the second has passed, increment the score
        score_.increment();
        score_.update_value();
        score_clock_.restart(); // Restart the clock
    }

    fps(); // Update the FPS counter
    draw(); // Draw the game objects

    return player_.status(); // Return the player status
}

void space::draw() {
    window_.clear(); // Clear the window

    draw_chunks(); // Draw the chunks

    player_.draw(); // Draw the player

    draw_ui(); // Draw the user interface
    window_.display(); // Display the window
}

void space::draw_chunks() {
    // Draw all background tiles first
    for (const auto&[fst, snd] : chunks_) { // Iterate through all chunks
        // Get the position and meteors in the chunk
        const auto&[position, meteors] = snd;

        sf::Sprite background_tile; // Create a new sprite for the background tile
        background_tile.setTexture(background_texture_); // Set the texture of the background tile
        background_tile.setPosition(position); // Set the position of the background tile
        window_.draw(background_tile); // Draw the background tile
    }

    // Draw all meteors after background tiles
    for (const auto&[fst, snd] : chunks_) { // Iterate through all chunks
        const auto&[position, meteors] = snd; // Get the position and meteors in the chunk

        for (const auto& meteor : meteors) { // Iterate through all meteors in the chunk
            window_.draw(meteor); // Draw the meteor
        }
    }
}

// Draw UI elements using a fixed view
void space::draw_ui() {
    const sf::View original_view = window_.getView(); // Get the original view of the window
    window_.setView(window_.getDefaultView()); // Set the view to the default view

    ui_layer_.clear(sf::Color::Transparent); // Clear the UI layer
    ui_layer_.draw(score_); // Draw the score
    ui_layer_.draw(fps_text_); // Draw the FPS counter
    ui_layer_.draw(position_); // Draw the player position
    ui_layer_.draw(chunk_); // Draw the player chunk
    ui_layer_.display(); // Display the UI layer

    const sf::Sprite ui_sprite(ui_layer_.getTexture()); // Create a sprite from the UI layer
    window_.draw(ui_sprite); // Draw the UI sprite

    window_.setView(original_view); // Reset the view to the original view
}

void space::fps() {
    frames_++; // Increment the frame count
    if (fps_clock_.getElapsedTime().asSeconds() - last_time_ >= 1.0f) {
        // if a second has passed, update the FPS counter
        fps_text_.setString("FPS: " + std::to_string(frames_)); // Set the FPS counter
        frames_ = 0; // Reset the frame count
        last_time_ += 1.0f; // Update the last time
    }
}
