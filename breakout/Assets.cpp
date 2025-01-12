//
// Created by David Burchill on 2023-10-31.
//

#include "Assets.h"
#include <iostream>
#include <cassert>
#include <fstream>

Assets::Assets()
{}

Assets &Assets::getInstance() {
    static Assets instance;          // Meyers Singleton implementation
    return instance;
}

void Assets::addFont(const std::string &fontName, const std::string &path) {
    std::unique_ptr<sf::Font> font(new sf::Font);
    if (!font->loadFromFile(path))
        throw std::runtime_error("Load failed - " + path);

    auto rc = _fontMap.insert(std::make_pair(fontName, std::move(font)));
    if (!rc.second) assert(0); // big problems if insert fails

    std::cout << "Loaded font: " << path << std::endl;
}
 

const sf::Font& Assets::getFont(const std::string &fontName) const {
    auto found = _fontMap.find(fontName);
    assert(found != _fontMap.end());
    return *found->second;
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
            config.clear(); // clear error on stream
            std::cout << "*** Error reading config file\n";
        }
        config >> token;
    }
    config.close();
}
