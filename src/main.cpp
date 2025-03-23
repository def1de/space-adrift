#include "scenes/menu.hpp"
#include <SFML/Graphics.hpp>

int main()
{
    // create the window
    sf::RenderWindow window(sf::VideoMode::getDesktopMode(), "Space Adrift", sf::Style::Fullscreen);
    window.setFramerateLimit(165); // limit the frame rate to 165 fps

    menu menu(window); // instantiate the menu object
    menu.run(); // run the menu
}