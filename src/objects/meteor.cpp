#include "meteor.hpp"
#include "../utils/random.hpp"
#include <iostream>

meteor::meteor(): animated_sprite(ASSETS_DIR "/meteor.png", 96, 96, 0.1f) {
    setScale(3.f, 3.f);
    speed_ = random(1, 3);

    const sf::FloatRect bounds = getLocalBounds();
    radius_ = bounds.width / 2;
    setOrigin(bounds.width / 2, bounds.height / 2);

    add_animation("explode", ASSETS_DIR "/meteor_explode.png", 96, 96, 0.1f, false);

    if(!buffer_.loadFromFile(ASSETS_DIR "/meteor_explosion.ogg")) {
        throw std::runtime_error("Failed to load meteor explosion sound.");
    }
    sound_.setBuffer(buffer_);
}

float meteor::get_radius() const {
    return radius_;
}

void meteor::update() {
    if(!is_out_) {
        set_animation("idle");
        setRotation(getRotation()+.05f*speed_);
    }
    animated_sprite::update();
}

void meteor::out() {
    is_out_ = true;
    set_animation("explode");
    // Ensure the sound is not already playing
    if (sound_.getStatus() == sf::Sound::Playing) {
        sound_.stop();
    }
    sound_.setBuffer(buffer_);
    sound_.play();
}

bool meteor::is_out() const {
    return is_out_;
}