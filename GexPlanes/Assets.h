//
// Created by David Burchill on 2023-10-31.
//

#ifndef BREAKOUT_ASSETS_H
#define BREAKOUT_ASSETS_H

#include "Animation.h"          // Include the Animation class

#include <SFML/Graphics.hpp>    // Include the SFML graphics library
#include <SFML/Audio.hpp>       // Include the SFML audio library
#include <map>                  // Include the map container from the standard library

// Structure to hold sprite information
struct SpriteRec 
{
    std::string texName; // Name of the texture
    sf::IntRect texRect; // Rectangle defining the texture region
};

// Structure to hold animation information
struct AnimationRec 
{
    std::string     texName;    // Name of the texture
    sf::Vector2i    frameSize;  // Size of each frame
    size_t          numbFrames; // Number of frames in the animation
    sf::Time        duration;   // Duration of the animation
    bool            repeat;     // Flag to indicate if the animation repeats
};

// Singleton class to manage game assets
class Assets 
{
private:
    // Private constructor for singleton pattern
    Assets();
    ~Assets() = default; // Default destructor

public:
    // Get the singleton instance of the Assets class
    static Assets& getInstance();

    // Delete copy and move constructors and assignment operators
    Assets(const Assets&)               = delete;
    Assets(Assets&&)                    = delete;
    Assets& operator=(const Assets&)    = delete;
    Assets& operator=(Assets&&)         = delete;

private:
    // Maps to store different types of assets
    std::map<std::string, std::unique_ptr<sf::Font>>            _fontMap;       // Map of fonts
    std::map<std::string, std::unique_ptr<sf::SoundBuffer>>     _soundEffects;  // Map of sound effects
    std::map<std::string, sf::Texture>                          _textures;      // Map of textures
    /*std::map<std::string, Animation>                            _animationMap;*/  // Map of animations
    std::map<std::string, std::vector<sf::IntRect>>             _frameSets;     // Map of frame sets

    // Private methods to load assets from files
    void loadFonts(const std::string& path);
    void loadTextures(const std::string& path);
    void loadSounds(const std::string& path);
    /*void loadJson(const std::string& path);
    void loadAnimations(const std::string& path);
    void loadSpriteRecs(const std::string& path);
    void loadAnimationRecs(const std::string& path);*/

public:
    // Public method to load all assets from a file
    void loadFromFile(const std::string path);

    // Methods to add individual assets
    void addFont(const std::string &fontName, const std::string &path);
    void addSound(const std::string &soundEffectName, const std::string &path);
    void addTexture(const std::string& textureName, const std::string& path, bool smooth = true);

    // Methods to add sprite and animation records
    /*void addSpriteRec(const std::string& name, SpriteRec sr);
    void addAnimationRec(const std::string& name, AnimationRec ar);*/

    // Methods to get individual assets
    const sf::Font&             getFont(const std::string &fontName) const;
    const sf::SoundBuffer&      getSound(const std::string &fontName) const;
    const sf::Texture&          getTexture(const std::string& textureName) const;
    /*const Animation&            getAnimation(const std::string& name) const;
    const SpriteRec&            getSpriteRec(const std::string& name) const;
    const AnimationRec&         getAnimationRec(const std::string name) const;*/
};

#endif //BREAKOUT_ASSETS_H
