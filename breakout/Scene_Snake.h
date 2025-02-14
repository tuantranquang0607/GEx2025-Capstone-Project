#pragma once

#include "Scene.h"
#include "EntityManager.h"
#include "GameEngine.h"

#include <memory>
#include <array>

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>

#include "Game.h"
#include "State.h"
#include "Snake.h"

class Scene_Snake : public Scene
{
public:
    Scene_Snake(std::shared_ptr<Context>& context, GameEngine* gameEngine);
    ~Scene_Snake();

    void Init() override;
    void ProcessInput() override;
    void Update(sf::Time deltaTime) override;
    void Draw() override;
    void Pause() override;
    void Start() override;

private:
    std::shared_ptr<Context> m_context;

    sf::Sprite m_grass;
    sf::Sprite m_food;

    std::array<sf::Sprite, 4> m_walls;

    Snake m_snake;

    /*sf::Text m_scoreText;
    int m_score;*/

    sf::Vector2f m_snakeDirection;
    sf::Time m_elapsedTime;

    bool m_isPaused;
};


//#pragma once
//
//#include "Scene.h"
//
//class Scene_Snake : public Scene
//{
//public:
//    Scene_Snake(GameEngine* gameEngine) : Scene(gameEngine) {}
//
//    void update(sf::Time dt) override;
//    void sRender() override;
//    void sDoAction(const Command& action) override;
//};


