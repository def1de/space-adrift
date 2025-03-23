
#ifndef MENU_HPP
#define MENU_HPP

#include <SFML/Graphics.hpp> // SFML graphics library
#include <SFML/Audio.hpp> // SFML audio library
#include "../objects/button.hpp" // button object
#include "../utils/AnimatedSprite.hpp" // animated sprite object for the planet

class menu {
private:
    sf::RenderWindow& window_; // pointer to the window

    sf::Sprite background_; // background sprite
    sf::Texture background_texture_; // background texture

    animated_sprite planet_; // animated planet sprite

    sf::Sprite player_; // player sprite
    sf::Texture player_texture; // player texture
    float player_angle_ = 0; // player angle
    float radius_ = 0; // radius

    button play_button_; // play button
    button exit_button_; // exit button

    sf::Music background_music_; // background music

public:
    explicit menu(sf::RenderWindow& window);
    void run();
    void update();
    void draw() const;
};

#endif
