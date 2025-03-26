#ifndef SOUND_MANAGER_HPP
#define SOUND_MANAGER_HPP

#include <SFML/Audio.hpp> // Allows to load sounds and sound buffers
#include <memory> // Allows to use shared_ptr
#include <map> // Allows to use a hash map

namespace sound_manager {
    // A hash map to store the shared pointers of the sounds
    extern std::map<std::string, std::shared_ptr<sf::Sound>> sounds_;
    // A hash map to store the shared pointers of the sound buffers
    extern std::map<std::string, std::shared_ptr<sf::SoundBuffer>> sound_buffers_;
    // Function to get the shared pointer of the sound
    std::shared_ptr<sf::Sound> get_sound(const std::string &sound_path);
}

#endif //SOUND_MANAGER_HPP