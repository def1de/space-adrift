#include "texture_manager.hpp" // Include the header file

namespace texture_manager {
    std::map<std::string, std::shared_ptr<sf::Texture>> textures_; // Define the hashmap of textures

    std::shared_ptr<sf::Texture> get_texture(const std::string &texture_path) {
        auto search = textures_.find(texture_path); // Search for the texture in the map
        if (search != textures_.end()) {
            // Return the shared pointer if the texture is already loaded
            return search->second;
        }

        // Load the texture from the file if it is not already loaded
        auto texture = std::make_shared<sf::Texture>(); // Create a shared pointer to the texture
        if (!texture->loadFromFile(texture_path)) { // Load the texture from the file
            // Throw an exception if the texture fails to load
            throw std::runtime_error("Failed to load texture: " + texture_path);
        }
        // Store the shared pointer of the texture in the map
        textures_[texture_path] = texture; // Store the shared pointer in the map
        return texture; // Return the shared pointer to the texture
    }
}


