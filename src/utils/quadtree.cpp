#include "quadtree.hpp"
#include "../objects/meteor.cpp"

// Constructor, Destructor, and Method Implementations

Quad::Quad(sf::FloatRect _boundary)
        :boundary(_boundary), n(nullptr),
        topLeftTree(nullptr), topRightTree(nullptr),
        botLeftTree(nullptr), botRightTree(nullptr) {}

Quad::~Quad() {
    clear();
}

void Quad::insert(Node* node)
{
    if (!inBoundary(node->pos)) {
        expandBoundary(node->pos);  // If the node is out of the boundary, expand the boundary
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

void Quad::expandBoundary(const sf::Vector2f& pos) {
    // Calculate the center of the current boundary
    float centerX = boundary.left + boundary.width / 2.0f;
    float centerY = boundary.top + boundary.height / 2.0f;

    // Calculate the new boundary that encompasses the point (pos)
    while (!inBoundary(pos)) {
        float halfWidth = boundary.width / 2.0f;
        float halfHeight = boundary.height / 2.0f;

        // If pos is left of the center, expand to the left
        if (pos.x < boundary.left) {
            boundary.left -= halfWidth;
        }
        // If pos is right of the center, expand to the right
        if (pos.x > boundary.left + boundary.width) {
            boundary.width += halfWidth;
        }

        // If pos is above the center, expand upwards
        if (pos.y < boundary.top) {
            boundary.top -= halfHeight;
        }
        // If pos is below the center, expand downwards
        if (pos.y > boundary.top + boundary.height) {
            boundary.height += halfHeight;
        }

        // Double the size of the boundary in both directions
        boundary.width *= 1.05;
        boundary.height *= 1.05;
    }
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