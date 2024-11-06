
#ifndef MENU_HPP
#define MENU_HPP

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "../objects/button.hpp"
#include "../utils/animated_sprite.hpp"

class menu {
private:
    sf::RenderWindow& window_;

    sf::Sprite background_;
    sf::Texture background_texture_;

    animated_sprite planet_;

    sf::Sprite player_;
    sf::Texture player_texture;
    float player_angle_ = 0;
    float radius_ = 0;

    sf::Sprite title_;
    sf::Texture title_texture_;

    button play_button_;
    button exit_button_;

    sf::Music background_music_;

public:
    explicit menu(sf::RenderWindow& window);
    void run();
    void update();
    void draw() const;
};

#endif
