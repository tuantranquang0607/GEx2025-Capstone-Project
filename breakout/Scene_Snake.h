#pragma once

#include "Scene.h"
#include "EntityManager.h"
#include "GameEngine.h"

class Scene_Snake : public Scene
{
private:
    // Add any members related to the Snake scene, like game state, snake objects, etc.

public:
    Scene_Snake(GameEngine* gameEngine, const std::string& levelPath);

    void onEnd() override;
    void update(sf::Time dt) override;
    void sDoAction(const Command& action) override;
    void sRender() override;

    void launchSnakeGameWindow();
};


