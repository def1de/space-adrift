#include "space.hpp"
#include "../utils/random.hpp"

space::space() :
window_(sf::VideoMode::getDesktopMode(), "CMake SFML Project", sf::Style::Fullscreen),
quadtree_(sf::FloatRect(0, 0, 5000, 5000)),
player_(window_),
camera_(window_),
score_(sf::Vector2f(10.f, 10.f), "Score: ", 0),
fps_text_(sf::Vector2f(10.f, 58.f), "FPS: ", 0),
position_(sf::Vector2f(10.f, 82.f), "Position: ", 0),
chunk_(sf::Vector2f(10.f, 106.f), "Chunk: ", 0)
{
    window_.setFramerateLimit(165);

    if(!background_texture_.loadFromFile(ASSETS_DIR "/background.png")) {
        std::cout << "Error loading background texture" << std::endl;
    }

    ui_layer_.create(window_.getSize().x, window_.getSize().y);

    if (!meteor_texture_.loadFromFile(ASSETS_DIR "/meteor.png")) {
        std::cerr << "Failed to load meteor texture." << std::endl;
    }

    if(!background_music_.openFromFile(ASSETS_DIR "/soundtrack.ogg")) {
        std::cerr << "Failed to load background music." << std::endl;
    } else {
        background_music_.setLoop(true);
        background_music_.play();
    }
}

void space::update_chunks() {

    const sf::Vector2f player_position = player_.getPosition();
    const sf::Vector2u image_size = background_texture_.getSize();

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
                const int num_meteors = random(1, 3);
                for (int m = 0; m < num_meteors; ++m) {
                    meteor meteor(meteor_texture_);
                    const float meteor_x = chunk.position.x + random(0, CHUNK_SIZE - static_cast<int>(meteor.getGlobalBounds().width));
                    const float meteor_y = chunk.position.y + random(0, CHUNK_SIZE - static_cast<int>(meteor.getGlobalBounds().height));
                    meteor.setPosition(meteor_x, meteor_y);
                    chunk.meteors.push_back(meteor);
                }

                chunks_[coords] = chunk;
                update_quadtree(); // Update the quadtree after adding a new chunk
            } else {
                // iterate through the meteors in the chunk
                auto&[position, meteors] = chunks_[coords];
                for (auto& meteor : meteors) {
                    meteor.update();
                }
            }
        }
    }
}

void space::update_quadtree() {
    quadtree_.clear();  // Clear the quadtree before each update
    for (auto&[fst, snd] : chunks_) {
        auto&[position, meteors] = snd;
        for (auto& meteor : meteors) {
            const node* new_node = new node(meteor.getPosition(), &meteor, meteor.get_radius());
            quadtree_.insert(new_node);  // Insert the meteor into the quadtree
        }
    }
}

void space::run() {
    while (window_.isOpen())
    {
        update();
    }
}

void space::update() {
    for (auto event = sf::Event{}; window_.pollEvent(event);) {
        if (event.type == sf::Event::Closed) {
            window_.close();
        }
    }

    if (is_paused_) {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
            is_paused_ = false;
        }
        return;
    }

    std::vector<meteor*> nearby_meteors;
    const sf::FloatRect player_range = player_.getGlobalBounds();  // Use player's bounding box

    quadtree_.retrieve(nearby_meteors, player_range);

    // Check for collisions
    for (const meteor* meteor : nearby_meteors) {
        if (player_.check_collision(meteor->get_radius(), meteor->getPosition())) {
            is_paused_ = true;
            //Draw a dummy circle of the radius of the meteor that collided with the player
        }
    }

    projectiles_ = player_.update_player();
    camera_.update(player_.get_player_position());

    update_chunks();

    const sf::Vector2f player_position = player_.getPosition();
    const std::string player_pos = "(" + std::to_string(player_position.x) + ", " + std::to_string(player_position.y) + ")";
    position_.update_custom_value(player_pos);
    const std::string chunk_pos = "(" + std::to_string(player_position.x / CHUNK_SIZE) + ", " + std::to_string(player_position.y / CHUNK_SIZE) + ")";
    chunk_.update_custom_value(chunk_pos);


    if (score_clock_.getElapsedTime().asSeconds() >= 1.0) {
        score_.increment();
        score_clock_.restart();
    }

    score_.update_value();

    fps();
    draw();
}

void space::draw() {
    window_.clear();

    draw_chunks();

    // Draw game objects
    window_.draw(player_);
    for (const auto& meteor : meteors_) {
        window_.draw(meteor);
    }
    for (const auto& projectile : projectiles_) {
        window_.draw(projectile);
    }

    draw_ui();
    window_.display();
}

void space::draw_chunks() {
    // Draw all background tiles first
    for (const auto&[fst, snd] : chunks_) {
        const auto&[position, meteors] = snd;

        sf::Sprite background_tile;
        background_tile.setTexture(background_texture_);
        background_tile.setPosition(position);
        window_.draw(background_tile);
    }

    // Draw all meteors after background tiles
    for (const auto&[fst, snd] : chunks_) {
        const auto&[position, meteors] = snd;

        for (const auto& meteor : meteors) {
            window_.draw(meteor);
        }
    }
}

    // Draw UI elements using a fixed view
void space::draw_ui() {
    const sf::View original_view = window_.getView();
    window_.setView(window_.getDefaultView());

    ui_layer_.clear(sf::Color::Transparent);
    ui_layer_.draw(score_);
    ui_layer_.draw(fps_text_);
    ui_layer_.draw(position_);
    ui_layer_.draw(chunk_);
    ui_layer_.display();

    const sf::Sprite ui_sprite(ui_layer_.getTexture());
    window_.draw(ui_sprite);

    window_.setView(original_view);
}

void space::fps() {
    frames_++;
    if (fps_clock_.getElapsedTime().asSeconds() - last_time_ >= 1.0f) {
        fps_text_.setString("FPS: " + std::to_string(frames_));
        frames_ = 0;
        last_time_ += 1.0f;
    }
}
