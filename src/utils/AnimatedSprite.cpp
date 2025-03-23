#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>

class AnimatedSprite : public sf::Sprite {
private:
    std::vector<sf::IntRect> frames; // Store the frames as rectangles
    size_t currentFrame; // Index of the current frame
    sf::Clock animationClock; // Clock to control the animation
    float frameDuration; // Duration of each frame in seconds
    sf::Texture texture; // Store texture as a member variable

public:
    AnimatedSprite(const std::string& texturePath, int frameWidth, int frameHeight, float frameDuration)
        : currentFrame(0), frameDuration(frameDuration) {
        if (!texture.loadFromFile(texturePath)) { // Load the texture
            std::cerr << "Error loading texture from " << texturePath << std::endl;
            return;
        }
        setTexture(texture); // Set the texture

        // Calculate the number of frames in the texture
        sf::Vector2u textureSize = texture.getSize();
        int numFramesX = textureSize.x / frameWidth;
        int numFramesY = textureSize.y / frameHeight;

        // Define the frames
        for (int y = 0; y < numFramesY; ++y) {
            for (int x = 0; x < numFramesX; ++x) {
                frames.emplace_back(x * frameWidth, y * frameHeight, frameWidth, frameHeight);
            }
        }

        // Set the initial frame
        setTextureRect(frames[currentFrame]);
    }

    void update() {
        // If the time since the last frame is greater than the frame duration
        if (animationClock.getElapsedTime().asSeconds() >= frameDuration) {
            // Move to the next frame
            currentFrame = (currentFrame + 1) % frames.size();
            setTextureRect(frames[currentFrame]);
            animationClock.restart(); // Restart the clock
        }
    }
};