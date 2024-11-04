#include "menu.hpp"
#include "space.hpp"
#include <cmath>
#include <iostream>

menu::menu(sf::RenderWindow& window) :
window_(window),
planet_(ASSETS_DIR "/menu_planet.png", 250, 250, 0.1f),
play_button_(window, 3, {window.getSize().x*0.75f, window.getSize().y / 2.0f - 50.0f}, ASSETS_DIR "/buttons/play_idle.png", ASSETS_DIR "/buttons/play_hover.png", ASSETS_DIR "/buttons/play_active.png"),
exit_button_(window, 3, {window.getSize().x*0.75f, window.getSize().y / 2.0f + 50.0f}, ASSETS_DIR "/buttons/quit_idle.png", ASSETS_DIR "/buttons/quit_hover.png", ASSETS_DIR "/buttons/quit_active.png")
{
    // Load background
    background_texture_.loadFromFile(ASSETS_DIR "/menu_background.png");
    background_.setTexture(background_texture_);

    // Load background music
    if (!background_music_.openFromFile(ASSETS_DIR "/menu.ogg")) {
        std::cerr << "Failed to load background music." << std::endl;
    } else {
        background_music_.setLoop(true);
        background_music_.play();
    }

    // Load animated planet
    planet_.setOrigin(planet_.getLocalBounds().width / 2.0f, planet_.getLocalBounds().height / 2.0f);
    planet_.setScale(6.0f, 6.0f);
    planet_.setPosition(150.0f, window.getSize().y / 2.0f);

    // Load player
    player_texture.loadFromFile(ASSETS_DIR "/player.png");
    player_.setTexture(player_texture);
    player_.setOrigin(player_.getLocalBounds().width / 2.0f, player_.getLocalBounds().height / 2.0f);
    player_.setScale(5.0f, 5.0f);
    player_.setPosition(window.getSize().x*0.25+300.0f, window.getSize().y / 2.0f);

    // Determine orbit radius
    const sf::FloatRect player_bounds = player_.getGlobalBounds();
    const sf::FloatRect planet_bounds = planet_.getGlobalBounds();
    radius_ = (player_bounds.width + planet_bounds.width) / 2.0f + 50.0f;
    std::cout << "Radius: " << radius_ << std::endl;

    // Load title
    title_texture_.loadFromFile(ASSETS_DIR "/menu_title.png");
    title_.setTexture(title_texture_);
    title_.setOrigin(title_.getLocalBounds().width / 2.0f, title_.getLocalBounds().height / 2.0f);
    title_.setScale(2.0f, 2.0f);
    title_.setPosition(window.getSize().x*0.75f, window.getSize().y / 4.0f);

    play_button_.set_callback([&window, this]() {
        space space(window);
        background_music_.stop();
        space.run();
        background_music_.play();
    });

    exit_button_.set_callback([&window]() {
        window.close();
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
    // Update animated background
    planet_.update();

    // Update player
    sf::Vector2f planet_position = planet_.getPosition();
    player_.setPosition(planet_position.x + radius_ * std::sin(player_angle_), planet_position.y + radius_ * std::cos(player_angle_));
    player_.setRotation(player_angle_ * -180.0f / M_PI + 90.0f);
    player_angle_ += 0.0025f;
}

void menu::draw() const {
    window_.clear();
    window_.draw(background_);
    // Draw animated background
    window_.draw(planet_);
    window_.draw(player_);
    // Draw UI elements
    // window_.draw(title_);
    play_button_.draw();
    exit_button_.draw();
    window_.display();
}

