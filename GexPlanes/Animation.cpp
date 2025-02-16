//
// Created by David Burchill on 2024-11-26.
//

#include "Animation.h" // Include the Animation header
#include "Utilities.h" // Include utility functions

// Parameterized constructor to initialize the animation
Animation::Animation(const std::string &name, const sf::Texture &t, std::vector<sf::IntRect> frames, sf::Time tpf, bool repeats)
        : _name(name)                   // Initialize the name
        , _frames(frames)               // Initialize the frames
        , _timePerFrame(tpf)            // Initialize the time per frame
        , _isRepeating(repeats)         // Initialize the repeating flag
        , _countDown(sf::Time::Zero)    // Initialize the countdown timer
        , _sprite(t, _frames[0])        // Initialize the sprite with the first frame
{
    centerOrigin(_sprite);                                                      // Center the origin of the sprite

    std::cout << name << " tpf: " << _timePerFrame.asMilliseconds() << "ms\n";  // Print the animation name and time per frame
}

// Update the animation based on elapsed time
void Animation::update(sf::Time dt) 
{
    _countDown -= dt; // Decrease the countdown timer by the elapsed time

    if (_countDown < sf::Time::Zero)                            // If the countdown timer is less than zero
    {
        _countDown = _timePerFrame;                             // Reset the countdown timer
        _currentFrame += 1;                                     // Move to the next frame

        if (_currentFrame == _frames.size() && !_isRepeating)   // If the animation has reached the last frame and is not repeating
            return;                                             // Leave the animation on the last frame
        else
            _currentFrame = (_currentFrame % _frames.size());   // Loop back to the first frame if repeating

        _sprite.setTextureRect(_frames[_currentFrame]);         // Set the texture rectangle to the current frame
        centerOrigin(_sprite);                                  // Center the origin of the sprite
    }
}

// Check if the animation has ended
bool Animation::hasEnded() const {
    return (_currentFrame >= _frames.size()); // Return true if the current frame index is greater than or equal to the number of frames
}

// Get the name of the animation
const std::string &Animation::getName() const {
    return _name; // Return the name of the animation
}

// Get the sprite of the animation
sf::Sprite &Animation::getSprite() {
    return _sprite; // Return the sprite
}

// Get the bounding box of the current frame
sf::Vector2f Animation::getBB() const {
    return static_cast<sf::Vector2f>(_frames[_currentFrame].getSize()); // Return the size of the current frame as a vector
}
