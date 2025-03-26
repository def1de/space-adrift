#ifndef TEXTURE_MANAGER_HPP
#define TEXTURE_MANAGER_HPP

#include <SFML/Graphics.hpp> // Allows to load textures
#include <memory> // Allows to use shared_ptr
#include <map> // Allows to use a hash map

// namespace that contains the texture manager
namespace texture_manager {
    // a hash map that contains the textures
    extern std::map<std::string, std::shared_ptr<sf::Texture>> textures_;
    // a function that returns a shared pointer to a texture
    std::shared_ptr<sf::Texture> get_texture(const std::string &texture_path);
}


#endif //TEXTURE_MANAGER_HPP
