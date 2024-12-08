#ifndef BUTTON_HPP
#define BUTTON_HPP

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <functional>
#include <memory>

class button {
private:
    struct textures {
        std::shared_ptr<sf::Texture> idle;
        std::shared_ptr<sf::Texture> hover;
        std::shared_ptr<sf::Texture> active;
    };

    sf::RenderWindow& window_;

    sf::RectangleShape shape_;
    sf::Font font_;
    sf::Text text_;
    std::function<void()> callback_;

    textures textures_;
    sf::Sprite sprite_;

    // Button sounds
    std::shared_ptr<sf::Sound> sound_;
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
