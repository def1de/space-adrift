#ifndef QUADTREE_HPP
#define QUADTREE_HPP

#include <vector>
#include <SFML/Graphics.hpp>

class Meteor;  // Forward declaration

struct Node {
    sf::Vector2f pos;
    Meteor* meteor;  // Store a pointer to the meteor object
    float radius;

    Node(sf::Vector2f _pos, Meteor* _meteor, float _radius)
        : pos(_pos), meteor(_meteor), radius(_radius) {}
};

class Quad {
    sf::FloatRect boundary; // Boundary of this quad

    // Contains details of a node (only one meteor at this level)
    Node* n;

    sf::Vector2f topLeft;
    sf::Vector2f botRight;

    std::vector<Meteor*> objects;  // Store objects (meteors)
    Quad* topLeftTree;
    Quad* topRightTree;
    Quad* botLeftTree;
    Quad* botRightTree;

public:
    Quad(sf::FloatRect _boundary);

    ~Quad();

    void insert(Node* node);
    void clear();
    void retrieve(std::vector<Meteor*>& returnObjects, sf::FloatRect range);
    bool inBoundary(const sf::Vector2f& point);
    bool inRange(const sf::FloatRect& range1, const sf::FloatRect& range2);

    void expandBoundary(const sf::Vector2f& pos);
};

#endif // QUADTREE_HPP