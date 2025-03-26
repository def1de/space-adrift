#include "animated_sprite.hpp" // Include the animated sprite header file
#include "texture_manager.hpp" // Include the texture manager header file

animated_sprite::animated_sprite(const std::string& texture_path, int frame_width, int frame_height, float frame_duration) : current_frame_(0) {
    add_animation("idle", texture_path, frame_width, frame_height, frame_duration, true);
    set_animation("idle");
}

void animated_sprite::add_animation(const std::string& name, const std::string &texture_path,
    int frame_width, int frame_height, float frame_duration, bool loop)
{
    animation new_animation; // Create a new animation
    new_animation.name = name; // Set the name of the animation
    // Load the texture using the texture manager
    new_animation.texture = texture_manager::get_texture(texture_path);
    // Calculate the number of frames in the texture
    int frames_x = new_animation.texture->getSize().x / frame_width;
    int frames_y = new_animation.texture->getSize().y / frame_height;

    // Set the frame duration and loop flag
    new_animation.frame_duration = frame_duration;
    new_animation.loop = loop;

    // Create the frames for the animation
    for (int y = 0; y < frames_y; ++y) {
        for (int x = 0; x < frames_x; ++x) {
            // Add the frame to the animation
            new_animation.frames.emplace_back(x * frame_width, y * frame_height, frame_width, frame_height);
        }
    }

    animation_list_.push_back(new_animation); // Add the animation to the list
}

void animated_sprite::set_animation(const std::string& name) {
    // Find the animation with the given name
    for (const auto& animation : animation_list_) { // Iterate through the animation pool
        if (animation.name == name && &animation != &current_animation_) {
            // If the animation is found and is different from the current animation
            current_animation_ = animation; // Set the current animation
            current_frame_ = 0; // Reset the current frame
            setTexture(*current_animation_.texture); // Set the texture of the sprite
            setTextureRect(current_animation_.frames[0]); // Set the texture rectangle to the first frame
            return; // Exit the loop
        }
    }
}

void animated_sprite::update() {
    if(is_animation_done()) {
        return; // Do not update the animation if it is done
    }
    if (animation_clock_.getElapsedTime().asSeconds() >= current_animation_.frame_duration) {
        // If the frame duration has passed, move to the next frame
        current_frame_ = (current_frame_ + 1) % current_animation_.frames.size();

        setTextureRect(current_animation_.frames[current_frame_]); // Set the texture rectangle to the new current frame
        animation_clock_.restart(); // Restart the animation clock
    }
}

bool animated_sprite::is_animation_done() const {
    // Check if the animation is done, by comparing the current frame with the last frame and the loop flag
    return (current_frame_ == current_animation_.frames.size()-1) && !current_animation_.loop;
}