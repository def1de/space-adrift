#include "sound_manager.hpp"
#include <iostream>

namespace sound_manager {
    std::map<std::string, std::shared_ptr<sf::Sound>> sounds_;
    std::map<std::string, std::shared_ptr<sf::SoundBuffer>> sound_buffers_;

    std::shared_ptr<sf::Sound> get_sound(const std::string &sound_path) {
        auto search = sounds_.find(sound_path);
        if (search != sounds_.end()) {
            return search->second; // Return the shared pointer if the texture is already loaded
        }

        auto sound_buffer = std::make_shared<sf::SoundBuffer>();
        if (!sound_buffer->loadFromFile(sound_path)) {
            throw std::runtime_error("Failed to load sound buffer: " + sound_path);
        }

        sound_buffers_[sound_path] = sound_buffer; // Store the shared pointer of the buffer in the map

        auto sound = std::make_shared<sf::Sound>();
        sound->setBuffer(*sound_buffer);
        sounds_[sound_path] = sound; // Store the shared pointer of the sound in the map
        return sound;
    }
}