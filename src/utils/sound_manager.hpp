#ifndef SOUND_MANAGER_HPP
#define SOUND_MANAGER_HPP

#include <SFML/Audio.hpp>
#include <memory>
#include <map>

namespace sound_manager {
    extern std::map<std::string, std::shared_ptr<sf::Sound>> sounds_;
    extern std::map<std::string, std::shared_ptr<sf::SoundBuffer>> sound_buffers_;
    std::shared_ptr<sf::Sound> get_sound(const std::string &sound_path);
}

#endif //SOUND_MANAGER_HPP
