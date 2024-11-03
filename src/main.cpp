#include "scenes/space.hpp"
#include "objects/button.hpp"
#include <SFML/Graphics.hpp>

// int main()
// {
//     space space;
//     space.run();
//     return 1;
// }

int main() {
    sf::RenderWindow window(sf::VideoMode::getDesktopMode(), "CMake SFML Project", sf::Style::Fullscreen);
    window.setFramerateLimit(165);

    sf::Vector2f play_button_pos = {window.getSize().x/2.0f, window.getSize().y/2.0f-50.0f};
    button play_button(3, play_button_pos, ASSETS_DIR "/buttons/play_idle.png", ASSETS_DIR "/buttons/play_hover.png", ASSETS_DIR "/buttons/play_active.png");
    play_button.set_callback([&window]() {
        space space(window);
        space.run();
    });

    sf::Vector2f exit_button_pos = {window.getSize().x/2.0f, window.getSize().y/2.0f+50.0f};
    button exit_button(3, exit_button_pos, ASSETS_DIR "/buttons/quit_idle.png", ASSETS_DIR "/buttons/quit_hover.png", ASSETS_DIR "/buttons/quit_active.png");
    exit_button.set_callback([&window]() {
        window.close();
    });

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            play_button.handle_event(event, window);
            exit_button.handle_event(event, window);
        }

        window.clear();
        play_button.draw(window);
        exit_button.draw(window);
        window.display();
    }

    return 0;
}