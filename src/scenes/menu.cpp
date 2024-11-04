#include "menu.hpp"
#include "space.hpp"

menu::menu(sf::RenderWindow& window) :
window_(window),
play_button_(window, 3, {window.getSize().x / 2.0f, window.getSize().y / 2.0f - 50.0f}, ASSETS_DIR "/buttons/play_idle.png", ASSETS_DIR "/buttons/play_hover.png", ASSETS_DIR "/buttons/play_active.png"),
exit_button_(window, 3, {window.getSize().x / 2.0f, window.getSize().y / 2.0f + 50.0f}, ASSETS_DIR "/buttons/quit_idle.png", ASSETS_DIR "/buttons/quit_hover.png", ASSETS_DIR "/buttons/quit_active.png")
{
    // Load background
    background_texture_.loadFromFile(ASSETS_DIR "/menu_background.png");
    background_.setTexture(background_texture_);

    play_button_.set_callback([&window]() {
        space space(window);
        space.run();
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

        draw();
    }
}

void menu::draw() {
    window_.clear();
    window_.draw(background_);
    // Draw animated background

    // Draw UI elements
    play_button_.draw();
    exit_button_.draw();
    window_.display();
}

