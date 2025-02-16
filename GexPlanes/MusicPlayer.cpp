//
// Created by David Burchill on 2022-11-29.
//

#include "MusicPlayer.h"    // Include the MusicPlayer header
#include <stdexcept>        // Include the stdexcept library for runtime_error

// Constructor to initialize the MusicPlayer
MusicPlayer::MusicPlayer() 
{
    m_filenames["menuTheme"] = "../assets/Music/8bitTheme.ogg";     // Add the menu theme to the filenames map
    m_filenames["gameTheme"] = "../assets/Music/theme.wav";  // Add the game theme to the filenames map
}

// Add a song to the MusicPlayer
void MusicPlayer::addSong(const std::string& name, const std::string& path) 
{
    m_filenames[name] = path; // Add the song to the filenames map
}

// Get the singleton instance of the MusicPlayer
MusicPlayer &MusicPlayer::getInstance() 
{
    static MusicPlayer instance; // Meyers Singleton implementation
    return instance;
}

// Play a song by name
void MusicPlayer::play(String theme) 
{
    if (!m_music.openFromFile(m_filenames[theme]))              // Open the music file
        throw std::runtime_error("Music could not open file");  // Throw an error if the file could not be opened

    m_music.setVolume(m_volume);                                // Set the volume of the music
    m_music.setLoop(true);                                      // Set the music to loop
    m_music.play();                                             // Play the music
}

// Stop the current song
void MusicPlayer::stop() 
{
    m_music.stop(); // Stop the music
}

// Pause or resume the current song
void MusicPlayer::setPaused(bool paused) 
{
    if (paused)
        m_music.pause(); // Pause the music
    else
        m_music.play(); // Resume the music
}

// Set the volume of the music
void MusicPlayer::setVolume(float volume) 
{
    m_volume = volume;              // Set the volume
    m_music.setVolume(m_volume);    // Apply the volume to the music
}
