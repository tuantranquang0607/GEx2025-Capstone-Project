#ifndef BREAKOUT_GAME_H
#define BREAKOUT_GAME_H

#include <memory>
#include <SFML/Graphics/RenderWindow.hpp>

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
    std::unique_ptr<Assets> m_assets;
    std::unique_ptr<StateMan> m_states;
    std::unique_ptr<sf::RenderWindow> m_window;

    Context()
    {
        m_assets = std::make_unique<Assets>();
        m_states = std::make_unique<StateMan>();
        m_window = std::make_unique<sf::RenderWindow>();
    }
};

class Game
{
public:
    Game();
    ~Game();

    void Run();

private:
    std::shared_ptr<Context> m_context;
    const sf::Time TIME_PER_FRAME = sf::seconds(1.f / 60.f);
};

#endif // BREAKOUT_GAME_H