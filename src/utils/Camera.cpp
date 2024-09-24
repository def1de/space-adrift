#include "Camera.hpp"

camera::camera(sf::RenderWindow& pwindow) : window_(pwindow) {
    view_ = window_.getDefaultView();
}

void camera::update(const sf::Vector2f& player_position) {
    view_.setCenter(player_position);
    window_.setView(view_);
}

sf::View camera::get_view() const {
    return view_;
}