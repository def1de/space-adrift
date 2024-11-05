#ifndef PROJECTILE_MANAGER_HPP
#define PROJECTILE_MANAGER_HPP

#define TEXTURE_PATH ASSETS_DIR "/player_projectile.png"
#define FRAME_WIDTH 4
#define FRAME_HEIGHT 16
#define FRAME_DURATION 0.1f

#include <SFML/Graphics.hpp>
#include <vector>
#include "../objects/projectile.hpp"
#include "../utils/quadtree.hpp"

class projectile_manager {
private:
    std::vector<projectile> projectiles_ = {};
    sf::RenderWindow& window_;
    quad& quadtree_;

public:
    explicit projectile_manager(sf::RenderWindow& pwindow, quad& pquadtree);

    void append(sf::Vector2f position, float angle_deg);
    void update();
    void draw();
};

#endif
