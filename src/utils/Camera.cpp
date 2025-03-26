#include "Camera.hpp" // Include the camera header file

camera::camera(sf::RenderWindow& pwindow) : window_(pwindow) {
    view_ = window_.getDefaultView(); // Set the view to the default view of the window
}

void camera::update(const sf::Vector2f& player_position) {
    view_.setCenter(player_position); // Center the view on the player
    window_.setView(view_); // Set the view of the window to the camera view
}

sf::View camera::get_view() const {
    return view_; // Return the camera view
}