//
// Created by David Burchill on 2022-11-29.
//

#include "SoundPlayer.h"            // Include the SoundPlayer header
#include "Assets.h"                 // Include the Assets header

#include <SFML/System/Vector2.hpp>  // Include the SFML Vector2 header
#include <SFML/Audio/Listener.hpp>  // Include the SFML Listener header
#include <cassert>                  // Include the cassert library for assertions
#include <cmath>                    // Include the cmath library for mathematical functions
#include <stdexcept>                // Include the stdexcept library for runtime_error

namespace {
    // Sound coordinate system, point of view of a player in front of the screen:
    // X = left; Y = up; Z = back (out of the screen);
    const float volume = 100.f;                                                                     // Default volume
    const float ListenerZ = 300.f;                                                                  // Z position of the listener
    const float Attenuation = 1.f;                                                                  // Attenuation factor
    const float MinDistance2D = 200.f;                                                              // Minimum distance in 2D
    const float MinDistance3D = std::sqrt(MinDistance2D * MinDistance2D + ListenerZ * ListenerZ);   // Minimum distance in 3D
}

// Constructor to initialize the SoundPlayer
SoundPlayer::SoundPlayer() {
    // Listener points towards the screen (default in SFML)
    sf::Listener::setDirection(0.f, 0.f, -1.f);
}

// Get the singleton instance of the SoundPlayer
SoundPlayer &SoundPlayer::getInstance() {
    static SoundPlayer instance; // Meyers Singleton implementation
    return instance;
}

// Play a sound effect
void SoundPlayer::play(String effect) {
    play(effect, getListnerPosition()); // Play the sound at the listener's position
}

// Play a sound effect at a specific position
void SoundPlayer::play(String effect, sf::Vector2f position) {
    m_sounds.push_back(sf::Sound());                            // Add a new sound to the list
    sf::Sound &sound = m_sounds.back();                         // Get a reference to the new sound

    sound.setBuffer(Assets::getInstance().getSound(effect));    // Set the sound buffer

    sound.setPosition(position.x,  0.f, -position.y);           // Set the position of the sound
    sound.setAttenuation(Attenuation);                          // Set the attenuation factor
    sound.setMinDistance(MinDistance3D);                        // Set the minimum distance

    sound.play(); // Play the sound
}

// Remove sounds that have stopped playing
void SoundPlayer::removeStoppedSounds() {
    m_sounds.remove_if([](const sf::Sound &s) {
        return s.getStatus() == sf::Sound::Stopped; // Remove sounds that have stopped
    });
}

// Set the listener's position
void SoundPlayer::setListnerPosition(sf::Vector2f position) {
    sf::Listener::setPosition(position.x, -position.y, ListenerZ); // Set the listener's position
}

// Set the listener's direction
void SoundPlayer::setListnerDirection(sf::Vector2f position) {
    // SFML default listener direction is (0,0,-1)
    sf::Listener::setDirection(position.x, 0 ,-position.y); // Set the listener's direction
}

// Get the listener's position
sf::Vector2f SoundPlayer::getListnerPosition() const {
    sf::Vector3f pos = sf::Listener::getPosition(); // Get the listener's position
    return sf::Vector2f(pos.x, -pos.y);             // Return the position as a 2D vector
}

// Check if the sound list is empty
bool SoundPlayer::isEmpty() const {
    return m_sounds.empty(); // Return true if the sound list is empty
}
