#include "quadtree.hpp"

quad::quad(const sf::FloatRect boundary)
        :boundary_(boundary), n_(nullptr),
        top_left_tree_(nullptr), top_right_tree_(nullptr),
        bot_left_tree_(nullptr), bot_right_tree_(nullptr) {}

quad::~quad() {
    clear();
}

void quad::insert(const node* node) // NOLINT(*-no-recursion)
{
    if (!in_boundary(node->pos)) {
        expand_boundary(node->pos);  // If the node is out of the boundary, expand the boundary
    }

    // If we're at a leaf node and this quad is empty, place the meteor here
    if (!top_left_tree_ && !n_) {
        n_ = node;
        return;
    }

    // Subdivide if necessary and pass the node to the appropriate child
    if (!top_left_tree_) {
        // Subdivide the quad
        const float half_width = boundary_.width / 2.0f;
        const float half_height = boundary_.height / 2.0f;

        // Create the four children
        top_left_tree_ = new quad(sf::FloatRect(boundary_.left, boundary_.top, half_width, half_height));
        top_right_tree_ = new quad(sf::FloatRect(boundary_.left + half_width, boundary_.top, half_width, half_height));
        bot_left_tree_ = new quad(sf::FloatRect(boundary_.left, boundary_.top + half_height, half_width, half_height));
        bot_right_tree_ = new quad(sf::FloatRect(boundary_.left + half_width, boundary_.top + half_height, half_width, half_height));
    }

    // Insert the node into the appropriate child
    if (top_left_tree_->in_boundary(node->pos)) top_left_tree_->insert(node);
    else if (top_right_tree_->in_boundary(node->pos)) top_right_tree_->insert(node);
    else if (bot_left_tree_->in_boundary(node->pos)) bot_left_tree_->insert(node);
    else if (bot_right_tree_->in_boundary(node->pos)) bot_right_tree_->insert(node);
}

void quad::retrieve(std::vector<meteor*>& return_objects, const sf::FloatRect range) const // NOLINT(*-no-recursion)
{
    // If this quad does not intersect the range, skip
    if (!in_range(range, boundary_)) {
        return;
    }

    // If this quad contains a meteor, and it intersects the range, add it to the list
    if (n_ && range.intersects(sf::FloatRect(n_->pos.x, n_->pos.y, n_->meteor_ptr->getGlobalBounds().width, n_->meteor_ptr->getGlobalBounds().height))) {
        return_objects.push_back(n_->meteor_ptr);
    }

    // If this quad has children, check them as well
    if (top_left_tree_) top_left_tree_->retrieve(return_objects, range);
    if (top_right_tree_) top_right_tree_->retrieve(return_objects, range);
    if (bot_left_tree_) bot_left_tree_->retrieve(return_objects, range);
    if (bot_right_tree_) bot_right_tree_->retrieve(return_objects, range);
}

void quad::expand_boundary(const sf::Vector2f& pos) {
    // Calculate the new boundary that encompasses the point (pos)
    while (!in_boundary(pos)) {
        const float half_width = boundary_.width / 2.0f;
        const float half_height = boundary_.height / 2.0f;

        // If pos is left of the center, expand to the left
        if (pos.x < boundary_.left) {
            boundary_.left -= half_width;
        }
        // If pos is right of the center, expand to the right
        if (pos.x > boundary_.left + boundary_.width) {
            boundary_.width += half_width;
        }
        // If pos is above the center, expand upwards
        if (pos.y < boundary_.top) {
            boundary_.top -= half_height;
        }
        // If pos is below the center, expand downwards
        if (pos.y > boundary_.top + boundary_.height) {
            boundary_.height += half_height;
        }

        // Increase the size of the boundary in both directions
        boundary_.width += 5000.0f;
        boundary_.height += 5000.0f;
    }
}

bool quad::in_boundary(const sf::Vector2f& point) const {
    return boundary_.contains(point);
}

// ReSharper disable once CppParameterNamesMismatch
bool quad::in_range(const sf::FloatRect& range, const sf::FloatRect& boundary)
{
    return range.intersects(boundary);
}

void quad::clear() { // NOLINT(*-no-recursion)
    // Delete current node
    if (n_ != nullptr) {
        delete n_;
        n_ = nullptr;
    }

    // Recursively clear child quads
    if (top_left_tree_) {
        top_left_tree_->clear();
        delete top_left_tree_;
        top_left_tree_ = nullptr;
    }

    if (top_right_tree_) {
        top_right_tree_->clear();
        delete top_right_tree_;
        top_right_tree_ = nullptr;
    }

    if (bot_left_tree_) {
        bot_left_tree_->clear();
        delete bot_left_tree_;
        bot_left_tree_ = nullptr;
    }

    if (bot_right_tree_) {
        bot_right_tree_->clear();
        delete bot_right_tree_;
        bot_right_tree_ = nullptr;
    }
}