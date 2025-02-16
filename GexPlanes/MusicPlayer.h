//
// Created by David Burchill on 2022-11-29.
//

#ifndef SFMLCLASS_MUSICPLAYER_H
#define SFMLCLASS_MUSICPLAYER_H

#include <map>                  // Include the map container from the standard library
#include <string>               // Include the string library
#include <SFML/Audio/Music.hpp> // Include the SFML music class

using String = std::string; // Type alias for std::string

// Singleton class to manage music playback
class MusicPlayer
{
private:
    MusicPlayer();              // Private constructor for singleton pattern
    ~MusicPlayer() = default;   // Default destructor

public:
    static MusicPlayer& getInstance(); // Get the singleton instance of the MusicPlayer

    // Delete copy and move constructors and assignment operators
    MusicPlayer(const MusicPlayer&) = delete;
    MusicPlayer(MusicPlayer&&) = delete;
    MusicPlayer& operator=(const MusicPlayer&) = delete;
    MusicPlayer& operator=(MusicPlayer&&) = delete;

    // Methods to manage music playback
    void addSong(const std::string& name, const std::string& path); // Add a song to the playlist
    void play(String theme);                                        // Play a song by name
    void stop();                                                    // Stop the current song
    void setPaused(bool paused);                                    // Pause or resume the current song
    void setVolume(float volume);                                   // Set the volume of the music

private:
    sf::Music m_music;                      // SFML music object for playback
    std::map<String, String> m_filenames;   // Map of song names to file paths
    float m_volume{25};                     // Volume level
};

#endif //SFMLCLASS_MUSICPLAYER_H
