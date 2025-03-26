#ifndef QUADTREE_HPP
#define QUADTREE_HPP

#include <vector> // Include the vector library to work with vectors
#include <SFML/Graphics.hpp> // Include the SFML graphics library to work with graphics
#include "../objects/meteor.hpp" // Include the meteor object

struct node {
    sf::Vector2f pos; // Position of the meteor
    meteor* meteor_ptr;  // Store a pointer to the meteor object
    float radius; // Radius of the meteor

    // Constructor
    node(const sf::Vector2f pos, meteor* meteor, const float radius)
        : pos(pos), meteor_ptr(meteor), radius(radius) {}
};

class quad {
    sf::FloatRect boundary_; // Boundary of this quad

    // Contains details of a node (only one meteor at this level)
    const node* n_; // Node of this quad

    quad* top_left_tree_; // Pointer to top left child
    quad* top_right_tree_; // Pointer to top right child
    quad* bot_left_tree_; // Pointer to bottom left child
    quad* bot_right_tree_; // Pointer to bottom right child

public:
    // Constructor
    explicit quad(sf::FloatRect boundary);

    ~quad(); // Destructor

    void insert(const node* node); // Insert a meteor into the quadtree
    void clear(); // Clear the quadtree
    void retrieve(std::vector<meteor*>& return_objects, sf::FloatRect range) const; // Retrieve meteor objects in a range
    [[nodiscard]] bool in_boundary(const sf::Vector2f& point) const; // Check if a point is in the boundary
    // [[nodiscard]] means that the function should return a value, and that value should not be ignored.

    static bool in_range(const sf::FloatRect& range, const sf::FloatRect& range2); // Check if two ranges intersect

    void expand_boundary(const sf::Vector2f& pos); // Expand the boundary if a quadtree is out of the boundary
};

#endif