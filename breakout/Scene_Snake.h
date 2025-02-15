#pragma once

#include <memory>
#include <SFML/Graphics/RenderWindow.hpp>

#include "Assets1.h"
#include "Assets.h"
#include "StateMan.h"

enum AssetID
{
    MAIN_FONT = 0,
    GRASS,
    FOOD,
    WALL,
    SNAKE
};

struct Context
{
    std::unique_ptr<Assets1> m_assets;
    std::unique_ptr<StateMan> m_states;
    std::unique_ptr<sf::RenderWindow> m_window;

    Context()
    {
        m_assets = std::make_unique<Assets1>();
        m_states = std::make_unique<StateMan>();
        m_window = std::make_unique<sf::RenderWindow>();
    }
};

class Scene_Snake
{
public:
    Scene_Snake();
    ~Scene_Snake();

    void Run();

private:
    std::shared_ptr<Context> m_context;
    const sf::Time TIME_PER_FRAME = sf::seconds(1.f / 60.f);
};