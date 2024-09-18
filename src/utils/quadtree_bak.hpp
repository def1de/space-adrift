#include <SFML/Graphics.hpp>
#include "../objects/meteor.cpp"

struct Node {
    sf::Vector2f pos;
    Meteor* meteor;  // Store a pointer to the meteor object

    Node(sf::Vector2f _pos, Meteor* _meteor)
        : pos(_pos), meteor(_meteor) {}
};

class Quad {
    // Hold details of the boundary of this node (using float for more precision)
    sf::FloatRect boundary;

    // Contains details of a node (only one meteor at this level)
    Node* n;

    // Children of this tree
    Quad* topLeftTree;
    Quad* topRightTree;
    Quad* botLeftTree;
    Quad* botRightTree;

public:
    Quad(sf::FloatRect _boundary)
        : boundary(_boundary), n(nullptr),
          topLeftTree(nullptr), topRightTree(nullptr),
          botLeftTree(nullptr), botRightTree(nullptr) {}

    void insert(Node*);
    void retrieve(std::vector<Meteor*>& returnObjects, sf::FloatRect range);
    bool inBoundary(const sf::Vector2f& pos);
    bool inRange(const sf::FloatRect& range, const sf::FloatRect& boundary);
    void clear();
};

void Quad::insert(Node* node)
{
    if (!inBoundary(node->pos)) {
        return;  // If the node is out of the boundary, do nothing
    }

    // If we're at a leaf node and this quad is empty, place the meteor here
    if (!topLeftTree && !n) {
        n = node;
        return;
    }

    // Subdivide if necessary and pass the node to the appropriate child
    if (!topLeftTree) {
        float halfWidth = boundary.width / 2.0f;
        float halfHeight = boundary.height / 2.0f;

        topLeftTree = new Quad(sf::FloatRect(boundary.left, boundary.top, halfWidth, halfHeight));
        topRightTree = new Quad(sf::FloatRect(boundary.left + halfWidth, boundary.top, halfWidth, halfHeight));
        botLeftTree = new Quad(sf::FloatRect(boundary.left, boundary.top + halfHeight, halfWidth, halfHeight));
        botRightTree = new Quad(sf::FloatRect(boundary.left + halfWidth, boundary.top + halfHeight, halfWidth, halfHeight));
    }

    if (topLeftTree->inBoundary(node->pos)) topLeftTree->insert(node);
    else if (topRightTree->inBoundary(node->pos)) topRightTree->insert(node);
    else if (botLeftTree->inBoundary(node->pos)) botLeftTree->insert(node);
    else if (botRightTree->inBoundary(node->pos)) botRightTree->insert(node);
}

void Quad::retrieve(std::vector<Meteor*>& returnObjects, sf::FloatRect range)
{
    // If this quad does not intersect the range, skip
    if (!inRange(range, boundary)) {
        return;
    }

    // If this quad contains a meteor, and it intersects the range, add it to the list
    if (n && range.intersects(sf::FloatRect(n->pos.x, n->pos.y, n->meteor->getGlobalBounds().width, n->meteor->getGlobalBounds().height))) {
        returnObjects.push_back(n->meteor);
    }

    // If this quad has children, check them as well
    if (topLeftTree) topLeftTree->retrieve(returnObjects, range);
    if (topRightTree) topRightTree->retrieve(returnObjects, range);
    if (botLeftTree) botLeftTree->retrieve(returnObjects, range);
    if (botRightTree) botRightTree->retrieve(returnObjects, range);
}

bool Quad::inBoundary(const sf::Vector2f& pos)
{
    return boundary.contains(pos);
}

bool Quad::inRange(const sf::FloatRect& range, const sf::FloatRect& boundary)
{
    return range.intersects(boundary);
}

void Quad::clear() {
    // Delete current node
    if (n != nullptr) {
        delete n;
        n = nullptr;
    }

    // Recursively clear child quads
    if (topLeftTree) {
        topLeftTree->clear();
        delete topLeftTree;
        topLeftTree = nullptr;
    }

    if (topRightTree) {
        topRightTree->clear();
        delete topRightTree;
        topRightTree = nullptr;
    }

    if (botLeftTree) {
        botLeftTree->clear();
        delete botLeftTree;
        botLeftTree = nullptr;
    }

    if (botRightTree) {
        botRightTree->clear();
        delete botRightTree;
        botRightTree = nullptr;
    }
}

