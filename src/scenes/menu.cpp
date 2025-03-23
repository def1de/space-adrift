#include "menu.hpp"
#include "space.hpp"
#include <cmath>

menu::menu(sf::RenderWindow& window) :
window_(window),
planet_(ASSETS_DIR "/menu_planet.png", 250, 250, 0.1f),
play_button_(window, 3, {window.getSize().x*0.75f, window.getSize().y / 2.0f - 50.0f},
    ASSETS_DIR "/buttons/play_idle.png",
    ASSETS_DIR "/buttons/play_hover.png",
    ASSETS_DIR "/buttons/play_active.png"),
exit_button_(window, 3, {window.getSize().x*0.75f, window.getSize().y / 2.0f + 50.0f},
    ASSETS_DIR "/buttons/quit_idle.png",
    ASSETS_DIR "/buttons/quit_hover.png",
    ASSETS_DIR "/buttons/quit_active.png")
{
    // Load background
    background_texture_.loadFromFile(ASSETS_DIR "/menu_background.png");
    background_.setTexture(background_texture_);

    // Load background music
    if (!background_music_.openFromFile(ASSETS_DIR "/menu.ogg")) {
        std::cerr << "Failed to load background music." << std::endl;
    } else {
        background_music_.setLoop(true); // loop the music
        background_music_.play(); // start the music
    }

    // Set up animated planet
    planet_.setOrigin(planet_.getLocalBounds().width / 2.0f, planet_.getLocalBounds().height / 2.0f);
    planet_.setScale(6.0f, 6.0f);
    planet_.setPosition(150.0f, window.getSize().y / 2.0f);

    // Load player's ship
    player_texture.loadFromFile(ASSETS_DIR "/player.png");
    player_.setTexture(player_texture);
    player_.setOrigin(player_.getLocalBounds().width / 2.0f, player_.getLocalBounds().height / 2.0f);
    player_.setScale(5.0f, 5.0f);
    player_.setPosition(window.getSize().x*0.25+300.0f, window.getSize().y / 2.0f);

    // Determine orbit radius
    const sf::FloatRect player_bounds = player_.getGlobalBounds();
    const sf::FloatRect planet_bounds = planet_.getGlobalBounds();
    radius_ = (player_bounds.width + planet_bounds.width) / 2.0f + 50.0f;

    play_button_.set_callback([&window, this]() {
        space space(window); // create a space object
        background_music_.stop(); // stop the background music
        space.run(); // run the space scene
        background_music_.play(); // resume the background music when the space scene is closed
    });

    exit_button_.set_callback([&window]() {
        window.close(); // close the window
    });
}

void menu::run() {
    while (window_.isOpen()) {
        sf::Event event;
        while (window_.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window_.close();
            }
            play_button_.handle_event(event);
            exit_button_.handle_event(event);
        }

        update();
        draw();
    }
}

void menu::update() {
    planet_.update(); // update the planet animation

    sf::Vector2f planet_position = planet_.getPosition(); // get the planet position
    player_angle_ += 0.0025f; // increment the player angle
    // set the player position based on the planet position, the radius and the player angle
    player_.setPosition(planet_position.x + radius_ * std::sin(player_angle_), planet_position.y + radius_ * std::cos(player_angle_));
    player_.setRotation(player_angle_ * -180.0f / M_PI + 90.0f); // set player's rotation in degrees

    // Update buttons
    play_button_.update();
    exit_button_.update();
}

void menu::draw() const {
    window_.clear(); // clear the window
    window_.draw(background_);
    window_.draw(planet_);
    window_.draw(player_);
    // Draw UI elements
    play_button_.draw();
    exit_button_.draw();
    window_.display();
}

