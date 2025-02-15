#include "GamePlay.h"
#include "GameOver.h"
#include "PauseGame.h"

#include <SFML/Window/Event.hpp>

#include <stdlib.h>
#include <time.h>
#include <iostream>

GamePlay::GamePlay(std::shared_ptr<Context>& context) :
    m_context(context),
    m_snakeDirection({ 24.f, 0.f }),
    m_elapsedTime(sf::Time::Zero),
    /*m_score(0),*/
    m_isPaused(false)
{
    srand(time(nullptr));
}

GamePlay::~GamePlay()
{
}

void GamePlay::Init()
{
    m_context->m_assets->AddTexture(GRASS, "../assets/background.png", true);
    m_context->m_assets->AddTexture(FOOD, "../assets/apple.png");
    m_context->m_assets->AddTexture(WALL, "../assets/wall.png", true);
    m_context->m_assets->AddTexture(SNAKE, "../assets/snake.png");

    m_grass.setTexture(m_context->m_assets->GetTexture(GRASS));
    m_grass.setTextureRect(m_context->m_window->getViewport(m_context->m_window->getDefaultView()));

    for (auto& wall : m_walls)
    {
        wall.setTexture(m_context->m_assets->GetTexture(WALL));
    }

    m_walls[0].setTextureRect({ 0, 0, (int)m_context->m_window->getSize().x, 24 });
    m_walls[1].setTextureRect({ 0, 0, (int)m_context->m_window->getSize().x, 24 });
    m_walls[1].setPosition(0, m_context->m_window->getSize().y - 24);

    m_walls[2].setTextureRect({ 0, 0, 24, (int)m_context->m_window->getSize().y });
    m_walls[3].setTextureRect({ 0, 0, 24, (int)m_context->m_window->getSize().y });
    m_walls[3].setPosition(m_context->m_window->getSize().x - 24, 0);

    m_food.setTexture(m_context->m_assets->GetTexture(FOOD));
    m_food.setPosition(m_context->m_window->getSize().x / 2, m_context->m_window->getSize().y / 2);

    m_snake.Init(m_context->m_assets->GetTexture(SNAKE));

    /*m_scoreText.setFont(m_context->m_assets->GetFont(MAIN_FONT));
    m_scoreText.setString("Score : " + std::to_string(m_score));
    m_scoreText.setCharacterSize(48);*/
}

void GamePlay::ProcessInput()
{
    sf::Event event;

    while (m_context->m_window->pollEvent(event))
    {
        if (event.type == sf::Event::Closed)
        {
            m_context->m_states->PopAll();
        }
        else if (event.type == sf::Event::KeyPressed)
        {
            sf::Vector2f newDirection = m_snakeDirection;

            switch (event.key.code)
            {
            case sf::Keyboard::Up:
                newDirection = { 0.f, -24.f };
                break;
            case sf::Keyboard::W:
                newDirection = { 0.f, -24.f };
                break;

            case sf::Keyboard::Down:
                newDirection = { 0.f, 24.f };
                break;
            case sf::Keyboard::S:
                newDirection = { 0.f, 24.f };
                break;

            case sf::Keyboard::Left:
                newDirection = { -24.f, 0.f };
                break;
            case sf::Keyboard::A:
                newDirection = { -24.f, 0.f };
                break;

            case sf::Keyboard::Right:
                newDirection = { 24.f, 0.f };
                break;
            case sf::Keyboard::D:
                newDirection = { 24.f, 0.f };
                break;

            case sf::Keyboard::Escape:
                m_context->m_states->Add(std::make_unique<PauseGame>(m_context));
                break;

            default:
                break;
            }

            if (std::abs(m_snakeDirection.x) != std::abs(newDirection.x) || std::abs(m_snakeDirection.y) != std::abs(newDirection.y))
            {
                m_snakeDirection = newDirection;
            }
        }
    }
}

void GamePlay::Update(sf::Time deltaTime)
{
    if (!m_isPaused)
    {
        m_elapsedTime += deltaTime;

        if (m_elapsedTime.asSeconds() > 0.1)
        {
            for (auto& wall : m_walls)
            {
                if (m_snake.IsOn(wall))
                {
                    m_context->m_states->Add(std::make_unique<GameOver>(m_context), true);
                    break;
                }
            }

            if (m_snake.IsOn(m_food))
            {
                m_snake.Grow(m_snakeDirection);

                int x = 0;
                int y = 0;

                x = std::clamp<int>(rand() % m_context->m_window->getSize().x, 24, m_context->m_window->getSize().x - 2 * 24);
                y = std::clamp<int>(rand() % m_context->m_window->getSize().y, 24, m_context->m_window->getSize().y - 2 * 24);

                m_food.setPosition(x, y);

                /*m_score += 1;
                m_scoreText.setString("Score : " + std::to_string(m_score));*/
            }
            else
            {
                m_snake.Move(m_snakeDirection);
            }

            if (m_snake.IsSelfIntersecting())
            {
                m_context->m_states->Add(std::make_unique<GameOver>(m_context), true);
            }

            m_elapsedTime = sf::Time::Zero;
        }
    }
}

void GamePlay::Draw()
{
    m_context->m_window->clear();
    m_context->m_window->draw(m_grass);

    for (auto& wall : m_walls)
    {
        m_context->m_window->draw(wall);
    }

    m_context->m_window->draw(m_food);
    m_context->m_window->draw(m_snake);
    /*m_context->m_window->draw(m_scoreText);*/
    m_context->m_window->display();
}

void GamePlay::Pause()
{
    m_isPaused = true;
}

void GamePlay::Start()
{
    m_isPaused = false;
}
