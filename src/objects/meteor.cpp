#include "meteor.hpp" // Include the meteor header
#include "../utils/random.hpp" // Include the random number generator
#include "../utils/sound_manager.hpp" // Include the sound manager

meteor::meteor(): animated_sprite(ASSETS_DIR "/meteor.png", 96, 96, 0.1f) {
    setScale(3.f, 3.f); // Scale the meteor
    speed_ = random(1, 3); // Randomize the speed of the meteor

    const sf::FloatRect bounds = getLocalBounds(); // Get the local bounds of the meteor
    radius_ = bounds.width / 2; // Set the radius of the meteor to half of the width
    setOrigin(bounds.width / 2, bounds.height / 2); // Set the origin of the meteor to the center

    // Add the explode animation
    add_animation("explode", ASSETS_DIR "/meteor_explode.png", 96, 96, 0.1f, false);

    // Obtain the sound pointer
    sound_ = sound_manager::get_sound(ASSETS_DIR "/meteor_explosion.ogg");
}

float meteor::get_radius() const {
    return radius_; // Return the radius of the meteor
}

void meteor::update() {
    if(!is_out_) { // Only update if the meteor is not out
        set_animation("idle"); // Set the idle animation
        setRotation(getRotation()+.05f*speed_); // Rotate the meteor
    }
    animated_sprite::update(); // Update the animation despite the meteor being out or not
}

void meteor::out() {
    if(is_out_) {
        return; // Return if the meteor is already marked as out
    }
    is_out_ = true; // Mark the meteor as out
    set_animation("explode"); // Set the explode animation
    // Ensure the sound is not already playing
    if (sound_->getStatus() == sf::Sound::Playing) {
        sound_->stop(); // Stop the sound if it is playing
    }
    sound_->play(); // Play the explosion sound
}

bool meteor::is_out() const {
    return is_out_; // Return the flag that indicates if the meteor should be removed
}