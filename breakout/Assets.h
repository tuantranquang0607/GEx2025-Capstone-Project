//
// Created by David Burchill on 2023-10-31.
//

#ifndef BREAKOUT_ASSETS_H
#define BREAKOUT_ASSETS_H

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include <map>


class Assets {

private:
    Assets();
    ~Assets() = default;

public:
    static Assets& getInstance();

    Assets(const Assets&)               = delete;
    Assets(Assets&&)                    = delete;
    Assets& operator=(const Assets&)    = delete;
    Assets& operator=( Assets&&)        = delete;

private:
    std::map<std::string, std::unique_ptr<sf::Font>>            _fontMap;
    std::map<std::string, std::unique_ptr<sf::SoundBuffer>>     _soundEffects;

    void loadSounds(const std::string& path);
 
public:
    void                    loadFromFile(const std::string path);
    void                    addFont(const std::string &fontName, const std::string &path);
    const sf::Font&         getFont(const std::string &fontName) const;   
    void addSound(const std::string& soundEffectName, const std::string& path);
    const sf::SoundBuffer& getSound(const std::string& fontName) const;
};


#endif
