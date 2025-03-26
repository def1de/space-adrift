#include "menu.hpp" // Include the menu header file
#include "space.hpp" // Include the space header file for the space scene
#include "../utils/texture_manager.hpp" // Include the texture manager header file for loading textures
#include <cmath> // Include the cmath library for mathematical functions
#include <iostream> // Include the iostream library for input/output

menu::menu(sf::RenderWindow& window) :
// Initialize the attributes
window_(window),
planet_(ASSETS_DIR "/menu_planet.png", 250, 250, 0.1f),
play_button_(window, 3, {window.getSize().x*0.75f, window.getSize().y / 2.0f - 50.0f}, ASSETS_DIR "/buttons/play_idle.png", ASSETS_DIR "/buttons/play_hover.png", ASSETS_DIR "/buttons/play_active.png"),
exit_button_(window, 3, {window.getSize().x*0.75f, window.getSize().y / 2.0f + 50.0f}, ASSETS_DIR "/buttons/quit_idle.png", ASSETS_DIR "/buttons/quit_hover.png", ASSETS_DIR "/buttons/quit_active.png")
{
    // Load background
    background_.setTexture(*texture_manager::get_texture(ASSETS_DIR "/menu_background.png"));

    // Load background music
    if (!background_music_.openFromFile(ASSETS_DIR "/menu.ogg")) {
        // If the background music fails to load, print an error message
        std::cerr << "Failed to load background music." << std::endl;
    } else {
        background_music_.setLoop(true); // Loop the background music
        background_music_.play(); // Play the background music
    }

    // Load animated planet
    planet_.setOrigin(planet_.getLocalBounds().width / 2.0f, planet_.getLocalBounds().height / 2.0f);
    planet_.setScale(6.0f, 6.0f);
    planet_.setPosition(150.0f, window.getSize().y / 2.0f);

    // Load player
    player_.setTexture(*texture_manager::get_texture(ASSETS_DIR "/player.png"));
    player_.setOrigin(player_.getLocalBounds().width / 2.0f, player_.getLocalBounds().height / 2.0f);
    player_.setScale(5.0f, 5.0f);
    player_.setPosition(window.getSize().x*0.25+300.0f, window.getSize().y / 2.0f);

    // Determine orbit radius
    const sf::FloatRect player_bounds = player_.getGlobalBounds();
    const sf::FloatRect planet_bounds = planet_.getGlobalBounds();
    radius_ = (player_bounds.width + planet_bounds.width) / 2.0f + 50.0f;

    // Load title
    title_.setTexture(*texture_manager::get_texture(ASSETS_DIR "/menu_title.png"));
    title_.setOrigin(title_.getLocalBounds().width / 2.0f, title_.getLocalBounds().height / 2.0f);
    title_.setScale(2.0f, 2.0f);
    title_.setPosition(window.getSize().x*0.75f, window.getSize().y / 4.0f);

    // Set play button callback
    play_button_.set_callback([&window, this]() {
        space space(window); // instantiate the space scene
        background_music_.stop(); // stop the background music
        space.run(); // run the space scene
        background_music_.play(); // resume the background music when the space scene is closed
    });

    // Set exit button callback
    exit_button_.set_callback([&window]() {
        window.close(); // close the window
    });
}

void menu::run() {
    // Run the menu while the window is open
    while (window_.isOpen()) {
        sf::Event event; // Create an event object
        while (window_.pollEvent(event)) { // Poll for events
            if (event.type == sf::Event::Closed) { // If the close event is triggered
                window_.close(); // Close the window
            }
            play_button_.handle_event(event); // Handle events for the play button
            exit_button_.handle_event(event); // Handle events for the exit button
        }

        update(); // Update the menu
        draw(); // Draw the menu
    }
}

void menu::update() {
    // Update animated planet
    planet_.update();

    // Update player
    sf::Vector2f planet_position = planet_.getPosition();
    player_.setPosition(planet_position.x + radius_ * std::sin(player_angle_), planet_position.y + radius_ * std::cos(player_angle_));
    player_.setRotation(player_angle_ * -180.0f / M_PI + 90.0f);
    player_angle_ += 0.0025f;

    // Update UI elements
    play_button_.update();
    exit_button_.update();
}

void menu::draw() const {
    window_.clear();
    window_.draw(background_);
    // Draw animated background
    window_.draw(planet_);
    window_.draw(player_);
    // Draw UI elements
    play_button_.draw();
    exit_button_.draw();
    window_.display();
}

