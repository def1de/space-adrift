#include "texture_manager.hpp"

namespace texture_manager {
    std::map<std::string, std::shared_ptr<sf::Texture>> textures_;

    std::shared_ptr<sf::Texture> get_texture(const std::string &texture_path) {
        auto search = textures_.find(texture_path);
        if (search != textures_.end()) {
            return search->second; // Return the shared pointer if the texture is already loaded
        }

        auto texture = std::make_shared<sf::Texture>();
        if (!texture->loadFromFile(texture_path)) {
            throw std::runtime_error("Failed to load texture: " + texture_path);
        }

        textures_[texture_path] = texture; // Store the shared pointer in the map
        return texture;
    }
}