#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <SFML/Graphics.hpp>

class camera {
private:
    sf::View view_;
    sf::RenderWindow& window_;

public:
    explicit camera(sf::RenderWindow& pwindow);

    void update(const sf::Vector2f& player_position);

    sf::View get_view() const;
};

#endif
