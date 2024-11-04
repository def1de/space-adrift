#include "scenes/menu.hpp"
#include <SFML/Graphics.hpp>

int main()
{
    sf::RenderWindow window(sf::VideoMode::getDesktopMode(), "CMake SFML Project", sf::Style::Fullscreen);
    window.setFramerateLimit(165);

    menu menu(window);
    menu.run();
}