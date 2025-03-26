#include "sound_manager.hpp" // Include the sound manager header file

namespace sound_manager {
    // A hash map to store the shared pointers of the sounds
    std::map<std::string, std::shared_ptr<sf::Sound>> sounds_;
    // A hash map to store the shared pointers of the sound buffers
    std::map<std::string, std::shared_ptr<sf::SoundBuffer>> sound_buffers_;

    std::shared_ptr<sf::Sound> get_sound(const std::string &sound_path){
        auto search = sounds_.find(sound_path); // Search for the sound in the map
        if (search != sounds_.end()) {
            // Return the shared pointer if the sound is already loaded
            return search->second;
        }

        // Load the sound from the file if it is not already loaded
        auto sound_buffer = std::make_shared<sf::SoundBuffer>(); // Create a shared pointer to the sound buffer
        if (!sound_buffer->loadFromFile(sound_path)) { // Load the sound buffer from the file
            // Throw an exception if the sound buffer fails to load
            throw std::runtime_error("Failed to load sound buffer: " + sound_path);
        }

        // Store the shared pointer of the buffer in the map
        sound_buffers_[sound_path] = sound_buffer;

        // Create a shared pointer to the sound
        auto sound = std::make_shared<sf::Sound>();
        sound->setBuffer(*sound_buffer); // Set the sound buffer to the sound
        sounds_[sound_path] = sound; // Store the shared pointer of the sound in the map
        return sound; // Return the shared pointer to the sound
    }
}


