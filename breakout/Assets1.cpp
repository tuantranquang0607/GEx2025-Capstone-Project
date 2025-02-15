#include <iostream>

#include "Assets1.h"

Assets1::Assets1()
{
}

Assets1::~Assets1()
{
}

void Assets1::AddTexture(int id, const std::string& filePath, bool wantRepeated)
{
    auto texture = std::make_unique<sf::Texture>();

    if (texture->loadFromFile(filePath))
    {
        texture->setRepeated(wantRepeated);
        m_textures[id] = std::move(texture);
    }
    else
    {
        std::cout << "Failed to load texture from file: " << filePath << std::endl;
    }
}

void Assets1::AddFont(int id, const std::string& filePath)
{
    auto font = std::make_unique<sf::Font>();

    if (font->loadFromFile(filePath))
    {
        m_fonts[id] = std::move(font);
    }
    else
    {
        std::cout << "Failed to load font from file: " << filePath << std::endl;
    }
}

const sf::Texture& Assets1::GetTexture(int id) const
{
    return *(m_textures.at(id).get());
}

const sf::Font& Assets1::GetFont(int id) const
{
    return *(m_fonts.at(id).get());
}
