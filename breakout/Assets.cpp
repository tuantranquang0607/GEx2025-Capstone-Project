//
// Created by David Burchill on 2023-10-31.
//

#include <iostream>
#include <cassert>
#include <fstream>

#include "MusicPlayer.h"
#include "Assets.h"

Assets::Assets()
{}

Assets &Assets::getInstance() {
    static Assets instance;
    return instance;
}

void Assets::addFont(const std::string &fontName, const std::string &path) {
    std::unique_ptr<sf::Font> font(new sf::Font);
    if (!font->loadFromFile(path))
        throw std::runtime_error("Load failed - " + path);

    auto rc = _fontMap.insert(std::make_pair(fontName, std::move(font)));
    if (!rc.second) assert(0);

    std::cout << "Loaded font: " << path << std::endl;
}
 
const sf::Font& Assets::getFont(const std::string &fontName) const {
    auto found = _fontMap.find(fontName);
    assert(found != _fontMap.end());
    return *found->second;
}

void Assets::addSound(const std::string& soundName, const std::string& path)
{
    std::unique_ptr<sf::SoundBuffer> sb(new sf::SoundBuffer);
    if (!sb->loadFromFile(path))
        throw std::runtime_error("Load failed - " + path);

    auto rc = _soundEffects.insert(std::make_pair(soundName, std::move(sb)));
    if (!rc.second) assert(0);

    std::cout << "Loaded sound effect: " << path << std::endl;
}

const sf::SoundBuffer& Assets::getSound(const std::string& soundName) const
{
    auto found = _soundEffects.find(soundName);
    assert(found != _soundEffects.end());
    return *found->second;
}

void Assets::loadSounds(const std::string& path)
{
    std::ifstream confFile(path);
    if (confFile.fail())
    {
        std::cerr << "Open file " << path << " failed\n";
        confFile.close();
        exit(1);
    }

    std::string token{ "" };
    confFile >> token;
    while (confFile)
    {
        if (token == "Sound")
        {
            std::string name, path;
            confFile >> name >> path;
            addSound(name, path);
        }
        else
        {
            std::string buffer;
            std::getline(confFile, buffer);
        }
        confFile >> token;
    }
    confFile.close();
}

void Assets::loadFromFile(const std::string path) {

    std::ifstream config(path);
    if (config.fail()) {
        std::cerr << "Open file " << path << " failed\n";
        config.close();
        exit(1);
    }

    std::string token{""};
    config >> token;
    while (!config.eof()) {
        if (token == "Font") {
            std::string font_name, path;
            config >> font_name >> path;
            Assets::getInstance().addFont(font_name, path);
        }  else if (token[0] == '#') {
            std::string tmp;
            std::getline(config, tmp);
            std::cout << tmp << "\n";
        }

        if (config.fail()) {
            config.clear();
            std::cout << "*** Error reading config file\n";
        }
        config >> token;
    }
    config.close();
}
