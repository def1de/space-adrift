#include "scenes/menu.hpp" // Include the menu header file
#include <SFML/Graphics.hpp> // Include the SFML Graphics module for the window

int main()
{
    // Create a window
    sf::RenderWindow window(sf::VideoMode::getDesktopMode(), "Space Adrift", sf::Style::Fullscreen);
    window.setFramerateLimit(165); // Limit the frame rate to 165 frames per second

    menu menu(window); // Create a menu instance
    menu.run(); // Run the menu
}