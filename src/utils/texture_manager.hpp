#ifndef TEXTURE_MANAGER_HPP
#define TEXTURE_MANAGER_HPP

#include <SFML/Graphics.hpp>
#include <memory>
#include <map>
#include <stdexcept>

namespace texture_manager {
    extern std::map<std::string, std::shared_ptr<sf::Texture>> textures_;
    std::shared_ptr<sf::Texture> get_texture(const std::string &texture_path);
}



#endif //TEXTURE_MANAGER_HPP
