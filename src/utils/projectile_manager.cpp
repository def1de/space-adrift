#include "projectile_manager.hpp"

projectile_manager::projectile_manager(sf::RenderWindow& pwindow, quad& pquadtree) : window_(pwindow), quadtree_(pquadtree) {
    if(!buffer_.loadFromFile(ASSETS_DIR "/projectile_shot.ogg")) {
        throw std::runtime_error("Failed to load projectile shot sound.");
    } else {
        sound_.setBuffer(buffer_);
    }
}

void projectile_manager::append(sf::Vector2f position, float angle_deg) {
    // create a new projectile
    projectiles_.emplace_back(position, angle_deg,
        TEXTURE_PATH, FRAME_WIDTH, FRAME_HEIGHT, FRAME_DURATION);
    sound_.play(); // play the shoot sound
}

void projectile_manager::update() {
    for(auto& projectile : projectiles_) {
        projectile.update();

        std::vector<meteor*> nearby_meteors;
        const sf::FloatRect projectile_range = projectile.getGlobalBounds();

        quadtree_.retrieve(nearby_meteors, projectile_range);

        for (meteor* meteor : nearby_meteors) {
            if (projectile.check_collision(meteor->get_radius(), meteor->getPosition())) {
                projectile.out();
                meteor->out();
            }
        }
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