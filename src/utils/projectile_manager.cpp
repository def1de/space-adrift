#include "projectile_manager.hpp"

projectile_manager::projectile_manager(sf::RenderWindow& pwindow) : window_(pwindow) {}

void projectile_manager::append(sf::Vector2f position, float angle_deg) {
    projectiles_.emplace_back(position, angle_deg, TEXTURE_PATH, FRAME_WIDTH, FRAME_HEIGHT, FRAME_DURATION);
}

void projectile_manager::update() {
    for(auto& projectile : projectiles_) {
        projectile.update();
    }

    projectiles_.erase(std::remove_if(projectiles_.begin(), projectiles_.end(), [](const projectile& projectile) {
        return projectile.is_out();
    }), projectiles_.end());
}

void projectile_manager::draw() {
    for(auto& projectile : projectiles_) {
        window_.draw(projectile);
    }
}