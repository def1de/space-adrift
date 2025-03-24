#include "projectile_manager.hpp"

projectile_manager::projectile_manager(sf::RenderWindow& pwindow, quad& pquadtree) : window_(pwindow), quadtree_(pquadtree) {}

void projectile_manager::append(sf::Vector2f position, float angle_deg) {
    // Append a new projectile to the vector
    projectiles_.emplace_back(position, angle_deg, TEXTURE_PATH,
        FRAME_WIDTH, FRAME_HEIGHT, FRAME_DURATION);
}

void projectile_manager::update() {
    for(auto& projectile : projectiles_) { // Go through each projectile
        projectile.update(); // Update the projectile

        std::vector<meteor*> nearby_meteors; // Vector for nearby meteors
        const sf::FloatRect projectile_range = projectile.getGlobalBounds();

        // Retrieve nearby meteors
        quadtree_.retrieve(nearby_meteors, projectile_range);

        for (meteor* meteor : nearby_meteors) { //  Go through each nearby meteor
            // Check if the projectile collides with the meteor
            if (projectile.check_collision(meteor->get_radius(), meteor->getPosition())) {
                projectile.out(); // Flag the projectile for removal
                meteor->out(); // Remove the meteor
            }
        }
    }

    // Remove projectiles marked for removal
    projectiles_.erase(std::remove_if(projectiles_.begin(), projectiles_.end(),
        [](const projectile& projectile) { // lambda function to remove projectiles marked for removal
            return projectile.is_out(); // Check if the projectile is out
        }
    ), projectiles_.end());
}

void projectile_manager::draw() {
    for(auto& projectile : projectiles_) { // Go through each projectile
        window_.draw(projectile); // draw the projectile on the window
    }
}