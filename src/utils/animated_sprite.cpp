#include "animated_sprite.hpp"
#include <iostream>

animated_sprite::animated_sprite(const std::string& texture_path, int frame_width, int frame_height, float frame_duration) : current_frame_(0) {
    std::cout << "Loading texture from " << texture_path << std::endl;
    add_animation("idle", texture_path, frame_width, frame_height, frame_duration, true);
    set_animation("idle");
}

void animated_sprite::add_animation(const std::string& name, const std::string &texture_path, int frame_width, int frame_height, float frame_duration, bool loop) {
    animation new_animation;
    new_animation.name = name;
    new_animation.texture.loadFromFile(texture_path);
    int frames_x = new_animation.texture.getSize().x / frame_width;
    int frames_y = new_animation.texture.getSize().y / frame_height;

    new_animation.frame_duration = frame_duration;
    new_animation.loop = loop;

    for (int y = 0; y < frames_y; ++y) {
        for (int x = 0; x < frames_x; ++x) {
            new_animation.frames.emplace_back(x * frame_width, y * frame_height, frame_width, frame_height);
        }
    }

    animation_list_.push_back(new_animation);
    std::cout << "Added animation " << name << " with " << new_animation.frames.size() << " frames" << std::endl;
}


void animated_sprite::set_animation(const std::string& name) {
    for (const auto& animation : animation_list_) {
        if (animation.name == name) {
            current_animation_ = animation;
            current_frame_ = 0;
            setTexture(current_animation_.texture);
            setTextureRect(current_animation_.frames[0]);
            return;
        }
    }
}

void animated_sprite::update() {
    if (animation_clock_.getElapsedTime().asSeconds() >= current_animation_.frame_duration) {
        // Move to the next frame
        current_frame_ = (current_frame_ + 1) % current_animation_.frames.size();
        setTextureRect(current_animation_.frames[current_frame_]);
        animation_clock_.restart();
    }
}