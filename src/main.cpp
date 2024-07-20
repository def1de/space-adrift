#include <SFML/Graphics.hpp>
#include "scenes/space.cpp"

bool isBelowBottomBoundary(const sf::Sprite& object, const sf::RenderWindow& window);

int main()
{
    Space space;
    space.run();
    return 1;
}