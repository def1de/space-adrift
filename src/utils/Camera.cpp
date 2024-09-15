#include <SFML/Graphics.hpp>

class Camera {
private:
    sf::View view;
    sf::RenderWindow& window;

public:
    explicit Camera(sf::RenderWindow& pwindow) : window(pwindow) {
        view = window.getDefaultView();
    }

    void update(const sf::Vector2f& playerPosition) {
        view.setCenter(playerPosition);
        window.setView(view);
    }

    sf::View getView() const {
        return view;
    }
};