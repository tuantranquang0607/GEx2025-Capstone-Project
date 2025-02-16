//
// Created by David Burchill on 2022-11-29.
//

#ifndef SFMLCLASS_SOUNDPLAYER_H
#define SFMLCLASS_SOUNDPLAYER_H

#include <SFML/Audio/SoundBuffer.hpp>   // Include the SFML SoundBuffer class
#include <SFML/Audio/Sound.hpp>         // Include the SFML Sound class
#include <SFML/System/Vector2.hpp>      // Include the SFML Vector2 class

#include <map>                          // Include the map container from the standard library
#include <list>                         // Include the list container from the standard library
#include <string>                       // Include the string library
#include <memory>                       // Include the memory library for smart pointers

using String = std::string; // Type alias for std::string

// Singleton class to manage sound playback
class SoundPlayer 
{
protected:
    SoundPlayer();                      // Protected constructor for singleton pattern

private:
    std::list<sf::Sound> m_sounds;      // List to store active sounds

public:
    static SoundPlayer& getInstance();  // Get the singleton instance of the SoundPlayer

    // Delete copy and move constructors and assignment operators
    SoundPlayer(const SoundPlayer&) = delete;
    SoundPlayer(SoundPlayer&&) = delete;
    SoundPlayer& operator=(const SoundPlayer&) = delete;
    SoundPlayer& operator=(SoundPlayer&&) = delete;

public:
    // Methods to manage sound playback
    void play(String effect);                           // Play a sound effect
    void play(String effect, sf::Vector2f position);    // Play a sound effect at a specific position
    void removeStoppedSounds();                         // Remove sounds that have stopped playing
    void setListnerPosition(sf::Vector2f position);     // Set the listener's position
    void setListnerDirection(sf::Vector2f position);    // Set the listener's direction
    sf::Vector2f getListnerPosition() const;            // Get the listener's position

    bool isEmpty() const;                               // Check if there are no active sounds
};

#endif //SFMLCLASS_SOUNDPLAYER_H
