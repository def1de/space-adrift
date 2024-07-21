#include <array>
#include <vector>
#include <memory>
#include <SFML/Graphics.hpp>

class Quadtree {
public:
    int MAX_OBJECTS = 10;
    int MAX_LEVELS = 5;
    int level;
    std::vector<sf::Sprite> objects;
    sf::FloatRect bounds;
    std::array<std::unique_ptr<Quadtree>, 4> nodes;

    Quadtree(int pLevel, sf::FloatRect pBounds) : level(pLevel), bounds(pBounds) {
        for (auto& node : nodes) {
            node = nullptr;
        }
    }

    void clear() {
        objects.clear();
        for (auto& node : nodes) {
            if (node != nullptr) {
                node->clear();
                node.reset();
            }
        }
    }

    void split() {
        float subWidth = bounds.width / 2;
        float subHeight = bounds.height / 2;
        float x = bounds.left;
        float y = bounds.top;

        nodes[0] = std::make_unique<Quadtree>(level+1, sf::FloatRect(x + subWidth, y, subWidth, subHeight));
        nodes[1] = std::make_unique<Quadtree>(level+1, sf::FloatRect(x, y, subWidth, subHeight));
        nodes[2] = std::make_unique<Quadtree>(level+1, sf::FloatRect(x, y + subHeight, subWidth, subHeight));
        nodes[3] = std::make_unique<Quadtree>(level+1, sf::FloatRect(x + subWidth, y + subHeight, subWidth, subHeight));
    }

    int getIndex(const sf::Sprite& sprite) const {
        int index = -1;
        double verticalMidpoint = bounds.left + (bounds.width / 2);
        double horizontalMidpoint = bounds.top + (bounds.height / 2);

        bool topQuadrant = (sprite.getPosition().y < horizontalMidpoint && sprite.getPosition().y + sprite.getGlobalBounds().height < horizontalMidpoint);
        bool bottomQuadrant = (sprite.getPosition().y > horizontalMidpoint);

        if (sprite.getPosition().x < verticalMidpoint && sprite.getPosition().x + sprite.getGlobalBounds().width < verticalMidpoint) {
            if (topQuadrant) {
                index = 1;
            } else if (bottomQuadrant) {
                index = 2;
            }
        } else if (sprite.getPosition().x > verticalMidpoint) {
            if (topQuadrant) {
                index = 0;
            } else if (bottomQuadrant) {
                index = 3;
            }
        }

        return index;
    }

    void insert(const sf::Sprite& sprite) {
        if (nodes[0] != nullptr) {
            int index = getIndex(sprite);

            if (index != -1) {
                nodes[index]->insert(sprite);
                return;
            }
        }

        objects.push_back(sprite);

        if (objects.size() > MAX_OBJECTS && level < MAX_LEVELS) {
            if (nodes[0] == nullptr) {
                split();
            }

            int i = 0;
            while (i < objects.size()) {
                int index = getIndex(objects[i]);
                if (index != -1) {
                    nodes[index]->insert(objects[i]);
                    objects.erase(objects.begin() + i);
                } else {
                    i++;
                }
            }
        }
    }

    std::vector<sf::Sprite> retrieve(std::vector<sf::Sprite>& returnObjects, const sf::Sprite& sprite) {
        int index = getIndex(sprite);
        if (index != -1 && nodes[0] != nullptr) {
            nodes[index]->retrieve(returnObjects, sprite);
        }

        returnObjects.insert(returnObjects.end(), objects.begin(), objects.end());

        return returnObjects;
    }
};