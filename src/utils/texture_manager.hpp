//
// Created by illia on 12/6/24.
//

#ifndef TEXTURE_MANAGER_HPP
#define TEXTURE_MANAGER_HPP

#include <SFML/Graphics.hpp>

namespace texture_manager {
    sf::Texture &get_texture(const std::string &texture_path);
}



#endif //TEXTURE_MANAGER_HPP
