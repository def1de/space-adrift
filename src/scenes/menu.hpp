
#ifndef MENU_HPP
#define MENU_HPP

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "../objects/button.hpp"

class menu {
private:
    sf::RenderWindow& window_;

    sf::Sprite background_;
    sf::Texture background_texture_;

    button play_button_;
    button exit_button_;

    sf::Music background_music_;

public:
    explicit menu(sf::RenderWindow& window);
    void run();
    void draw();
};

#endif
