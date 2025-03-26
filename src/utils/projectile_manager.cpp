#include "projectile_manager.hpp" // Include the projectile manager header file
#include "sound_manager.hpp" // Include the sound manager header file (for the sound of the projectile)

projectile_manager::projectile_manager(sf::RenderWindow& pwindow, quad& pquadtree) : window_(pwindow), quadtree_(pquadtree) {
    sound_ = sound_manager::get_sound(ASSETS_DIR "/projectile_shot.ogg"); // Get the sound of the projectile
}

void projectile_manager::append(sf::Vector2f position, float angle_deg) {
    // Append a new projectile to the vector
    projectiles_.emplace_back(position, angle_deg, TEXTURE_PATH, FRAME_WIDTH, FRAME_HEIGHT, FRAME_DURATION);
    sound_->play(); // Play the sound of the projectile
}

void projectile_manager::update() {
    for(auto& projectile : projectiles_) { // Iterate through all the projectiles
        projectile.update(); // Update the projectile

        std::vector<meteor*> nearby_meteors; // Vector of all nearby meteors
        const sf::FloatRect projectile_range = projectile.getGlobalBounds(); // Get the global bounds of the projectile

        quadtree_.retrieve(nearby_meteors, projectile_range); // Retrieve all nearby meteors from the quadtree

        for (meteor* meteor : nearby_meteors) { // Iterate through all nearby meteors
            if (projectile.check_collision(meteor->get_radius(), meteor->getPosition())) {
                // If the projectile collides with the meteor
                projectile.out(); // Mark the projectile for removal
                meteor->out(); // Mark the meteor for removal
            }
        }
    }

    // Remove all projectiles that are marked for removal.
    projectiles_.erase(std::remove_if(projectiles_.begin(), projectiles_.end(),
        [](const projectile& projectile) { // Lambda function to check if the projectile is out
        return projectile.is_out();
    }), projectiles_.end());
}

void projectile_manager::draw() {
    for(auto& projectile : projectiles_) { // Iterate through all the projectiles
        window_.draw(projectile); // Draw the projectile
    }
}