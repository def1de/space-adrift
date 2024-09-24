#include "AnimatedSprite.hpp"

animated_sprite::animated_sprite(const std::string& texture_path, int frame_width, int frame_height, const float frame_duration)
    : current_frame_(0), frame_duration_(frame_duration) {
    if (!texture_.loadFromFile(texture_path)) {
        return;
    }
    setTexture(texture_);

    // Calculate the number of frames in the texture
    const sf::Vector2u texture_size = texture_.getSize();
    const int num_frames_x = texture_size.x / frame_width;
    const int num_frames_y = texture_size.y / frame_height;

    // Define the frames
    for (int y = 0; y < num_frames_y; ++y) {
        for (int x = 0; x < num_frames_x; ++x) {
            frames_.emplace_back(x * frame_width, y * frame_height, frame_width, frame_height);
        }
    }

    // Set the initial frame
    setTextureRect(frames_[current_frame_]);
}

void animated_sprite::update() {
    if (animation_clock_.getElapsedTime().asSeconds() >= frame_duration_) {
        // Move to the next frame
        current_frame_ = (current_frame_ + 1) % frames_.size();
        setTextureRect(frames_[current_frame_]);
        animation_clock_.restart();
    }
}