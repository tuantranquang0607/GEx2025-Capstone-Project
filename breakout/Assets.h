//
// Created by David Burchill on 2023-10-31.
//

#ifndef BREAKOUT_ASSETS_H
#define BREAKOUT_ASSETS_H

#include <map>
#include <memory>
#include <string>

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Font.hpp>


class Assets {

private:
    /*Assets();
    ~Assets() = default;*/

    std::map<std::string, std::unique_ptr<sf::Font>>            _fontMap;
    std::map<std::string, std::unique_ptr<sf::SoundBuffer>>     _soundEffects;
    std::map<int, std::unique_ptr<sf::Texture>> m_textures;

    void   loadSounds(const std::string& path);

public:
    Assets();
    ~Assets() = default;

    static Assets& getInstance();

    Assets(const Assets&)               = delete;
    Assets(Assets&&)                    = delete;
    Assets& operator=(const Assets&)    = delete;
    Assets& operator=( Assets&&)        = delete;
 
    void    loadFromFile(const std::string path);
    void    addFont(const std::string &fontName, const std::string &path);
    void    addSound(const std::string& soundEffectName, const std::string& path);
    void    AddTexture(int id, const std::string& filePath, bool wantRepeated = false);

    const   sf::Font&         getFont(const std::string &fontName) const;
    const   sf::SoundBuffer& getSound(const std::string& fontName) const;
};


#endif
