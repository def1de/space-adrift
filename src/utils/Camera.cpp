#include <SFML/Graphics.hpp> // Include the SFML graphics library to work with the window view

class Camera {
private:
    sf::View view; // The view that the camera will follow
    sf::RenderWindow& window; // The window that the camera will render to

public:
    explicit Camera(sf::RenderWindow& pwindow) : window(pwindow) {
        view = window.getDefaultView(); // Get the starting view for the camera
    }

    void update(const sf::Vector2f& playerPosition) {
        view.setCenter(playerPosition); // Center the camera view on the player
        window.setView(view); // Apply the view to the window
    }

    sf::View getView() const {
        return view; // Return the current view
    }
};