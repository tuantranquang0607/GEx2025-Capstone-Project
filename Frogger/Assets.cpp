//
// Created by David Burchill on 2023-10-31.
//

#include "Assets.h"
#include "MusicPlayer.h"
#include <iostream>
#include <cassert>
#include <fstream>
#include "json.hpp"

Assets::Assets() {
}

Assets& Assets::getInstance() {
    static Assets instance; // Meyers Singleton implementation
    return instance;
}

void Assets::addFont(const std::string&fontName, const std::string&path) {
    std::unique_ptr<sf::Font> font(new sf::Font);
    if (!font->loadFromFile(path))
        throw std::runtime_error("Load failed - " + path);

    auto rc = m_fontMap.insert(std::make_pair(fontName, std::move(font)));
    if (!rc.second)
        assert(0); // big problems if insert fails

    std::cout << "Loaded font: " << path << std::endl;
}

void Assets::addSound(const std::string&soundName, const std::string&path) {
    std::unique_ptr<sf::SoundBuffer> sb(new sf::SoundBuffer);
    if (!sb->loadFromFile(path))
        throw std::runtime_error("Load failed - " + path);

    auto rc = m_soundEffects.insert(std::make_pair(soundName, std::move(sb)));
    if (!rc.second)
        assert(0); // big problems if insert fails

    std::cout << "Loaded sound effect: " << path << std::endl;
}

void Assets::addTexture(const std::string&textureName, const std::string&path, bool smooth) {
    m_textures[textureName] = sf::Texture();
    if (!m_textures[textureName].loadFromFile(path)) {
        std::cerr << "Could not load texture file: " << path << std::endl;
        m_textures.erase(textureName);
    }
    else {
        m_textures.at(textureName).setSmooth(smooth);
        std::cout << "Loaded texture: " << path << std::endl;
    }
}

void Assets::addSprite(const std::string&spriteName, const std::string&tn, sf::IntRect tr) {
    m_spriteMap[spriteName] = {tn, tr};
}

const sf::Font& Assets::getFont(const std::string&fontName) const {
    auto found = m_fontMap.find(fontName);
    assert(found != m_fontMap.end());
    return *found->second;
}


const sf::SoundBuffer& Assets::getSound(const std::string&soundName) const {
    auto found = m_soundEffects.find(soundName);
    assert(found != m_soundEffects.end());
    return *found->second;
}


const sf::Texture& Assets::getTexture(const std::string&textureName) const {
    return m_textures.at(textureName);
}


const Assets::Sprite& Assets::getSprt(const std::string& spriteName) const {
    return m_spriteMap.at(spriteName);
}



void Assets::loadSounds(const std::string& path) {
    std::ifstream confFile(path);
    if (confFile.fail()) {
        std::cerr << "Open file " << path << " failed\n";
        confFile.close();
        exit(1);
    }

    std::string token{""};
    confFile >> token;
    while (confFile) {
        if (token == "Sound") {
            std::string name, path;
            confFile >> name >> path;
            addSound(name, path);
        }
        else {
            // ignore rest of line and continue
            std::string buffer;
            std::getline(confFile, buffer);
        }
        confFile >> token;
    }
    confFile.close();

}






void Assets::loadFonts(const std::string&path) {
    std::ifstream confFile(path);
    if (confFile.fail()) {
        std::cerr << "Open file " << path << " failed\n";
        confFile.close();
        exit(1);
    }

    std::string token{""};
    confFile >> token;
    while (confFile) {
        if (token == "Font") {
            std::string name, path;
            confFile >> name >> path;
            addFont(name, path);
        }
        else {
            // ignore rest of line and continue
            std::string buffer;
            std::getline(confFile, buffer);
        }
        confFile >> token;
    }
    confFile.close();
}


void Assets::loadTextures(const std::string&path) {
    // Read Config file
    std::ifstream confFile(path);
    if (confFile.fail()) {
        std::cerr << "Open file: " << path << " failed\n";
        confFile.close();
        exit(1);
    }

    std::string token{""};
    confFile >> token;
    while (confFile) {
        if (token == "Texture") {
            std::string name, path;
            confFile >> name >> path;
            addTexture(name, path);
        }
        else {
            // ignore rest of line and continue
            std::string buffer;
            std::getline(confFile, buffer);
        }
        confFile >> token;
    }
    confFile.close();
}

void Assets::loadSprts(const std::string&path) {
    // Read Config file
    std::ifstream confFile(path);
    if (confFile.fail()) {
        std::cerr << "Open file: " << path << " failed\n";
        confFile.close();
        exit(1);
    }

    std::string token{""};
    confFile >> token;
    while (confFile) {
        if (token == "Sprite") {
            sf::IntRect rect;
            std::string spName, txName;
            confFile >> spName >> txName >> rect.left >> rect.top >> rect.width >> rect.height;
            addSprite(spName, txName, rect);
        }
        else {
            // ignore rest of line and continue
            std::string buffer;
            std::getline(confFile, buffer);
        }
        confFile >> token;
    }
    confFile.close();
}


void Assets::loadFromFile(const std::string path) {
    loadFonts(path);
    loadTextures(path);
    loadSprts(path);
    loadSounds(path);
    /*loadJson(path);*/
    /*loadAnimations(path);*/  // requires loadJson be run first
}
