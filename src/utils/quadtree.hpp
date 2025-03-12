#ifndef QUADTREE_HPP
#define QUADTREE_HPP

#include <vector>
#include <SFML/Graphics.hpp>
#include "../objects/meteor.hpp"

struct node {
    sf::Vector2f pos;
    meteor* meteor_ptr;  // Store a pointer to the meteor object
    float radius;

    node(const sf::Vector2f pos, meteor* meteor, const float radius)
        : pos(pos), meteor_ptr(meteor), radius(radius) {}
};

class quad {
    sf::FloatRect boundary_; // Boundary of this quad

    // Contains details of a node (only one meteor at this level)
    const node* n_;

    quad* top_left_tree_;
    quad* top_right_tree_;
    quad* bot_left_tree_;
    quad* bot_right_tree_;

public:
    explicit quad(sf::FloatRect boundary);

    ~quad();

    void insert(const node* node);
    void clear();
    void retrieve(std::vector<meteor*>& return_objects, sf::FloatRect range) const;
    [[nodiscard]] bool in_boundary(const sf::Vector2f& point) const;

    static bool in_range(const sf::FloatRect& range, const sf::FloatRect& range2);

    void expand_boundary(const sf::Vector2f& pos);
};

#endif