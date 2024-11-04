#ifndef BUTTON_HPP
#define BUTTON_HPP

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <functional>

class button {
private:
    struct textures {
        sf::Texture idle;
        sf::Texture hover;
        sf::Texture active;
    };

    sf::RenderWindow& window_;

    sf::RectangleShape shape_;
    sf::Font font_;
    sf::Text text_;
    std::function<void()> callback_;

    textures textures_;
    sf::Sprite sprite_;

    // Button sounds
    sf::SoundBuffer sound_buffer_;
    sf::Sound sound_;
    bool do_callback_ = false;
    sf::Clock sound_clock_;
    float sound_duration_;

public:
    button(sf::RenderWindow& window, float scale, const sf::Vector2f& position, const std::string& idle_path, const std::string& hover_path, const std::string& active_path);
    void set_callback(std::function<void()> callback);
    void handle_event(const sf::Event& event);
    void update();
    void draw() const;
};

#endif
