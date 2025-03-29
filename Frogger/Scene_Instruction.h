#pragma once

#include "Scene.h"
#include <SFML/Graphics.hpp>

class Scene_Instruction : public Scene
{
private:
    sf::Text m_instructionText;
    sf::Sprite m_backgroundSprite;

    void init();

public:
    Scene_Instruction(GameEngine* gameEngine);

    void update(sf::Time dt) override;
    void sRender() override;
    void sDoAction(const Command& action) override;
    void onEnd() override {} // Not used here
};


