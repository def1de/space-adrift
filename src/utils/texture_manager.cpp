#include "texture_manager.hpp"

sf::Texture &texture_manager::get_texture(const std::string &texture_path) {
    static std::map<std::string, sf::Texture> textures;

    auto search = textures.find(texture_path);
    if (search != textures.end()) {
        return search->second; // Return the texture if it has already been loaded
    }

    sf::Texture texture;
    if (!texture.loadFromFile(texture_path)) {
        throw std::runtime_error("Failed to load texture: " + texture_path);
    }

    textures[texture_path] = texture; // Store the texture in the map
    return textures[texture_path];
}
