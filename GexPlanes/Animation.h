//
// Created by David Burchill on 2024-11-26.
//

#ifndef ANIMATION_H
#define ANIMATION_H

#include <SFML/Graphics.hpp>                                // Include the SFML graphics library
#include <vector>                                           // Include the vector container from the standard library

// Class to handle animations
class Animation 
{
public:
    std::string                 _name{"none"};              // Name of the animation
    std::vector<sf::IntRect>    _frames;                    // Vector to store frames of the animation
    size_t                      _currentFrame{0};           // Index of the current frame
    sf::Time                    _timePerFrame;              // Time duration for each frame
    sf::Time                    _countDown{sf::Time::Zero}; // Countdown timer for frame switching
    bool                        _isRepeating{true};         // Flag to indicate if the animation repeats
    bool                        _hasEnded{false};           // Flag to indicate if the animation has ended
    sf::Sprite                  _sprite;                    // Sprite to display the animation

public:
    // Default constructor
    Animation() = default;

    // Parameterized constructor to initialize the animation
    Animation(const std::string& name, const sf::Texture& t, std::vector<sf::IntRect> frames, sf::Time tpf, bool repeats=true);

    void                    update(sf::Time dt);            // Update the animation based on elapsed time
    bool                    hasEnded() const;               // Check if the animation has ended
    const std::string&      getName() const;                // Get the name of the animation
    sf::Sprite&             getSprite();                    // Get the sprite of the animation
    sf::Vector2f            getBB() const;                  // Get the bounding box of the animation
};

#endif // ANIMATION_H
