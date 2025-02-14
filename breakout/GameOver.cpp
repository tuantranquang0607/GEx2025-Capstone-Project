#include "GameOver.h"
#include "Scene_Snake.h"

#include <SFML/Window/Event.hpp>

GameOver::GameOver(std::shared_ptr<Context>& context) :
    m_context(context),
    m_isRetryButtonSelected(true),
    m_isRetryButtonPressed(false),
    m_isExitButtonSelected(false),
    m_isExitButtonPressed(false)
{
}

GameOver::~GameOver()
{
}

void GameOver::Init()
{
    // Title
    m_gameOverTitle.setFont(m_context->m_assets->getFont("main"));
    m_gameOverTitle.setString("Game Over");
    m_gameOverTitle.setOrigin(m_gameOverTitle.getLocalBounds().width / 2.5, m_gameOverTitle.getLocalBounds().height / 2);
    m_gameOverTitle.setPosition(m_context->m_window->getSize().x / 2, m_context->m_window->getSize().y / 2 - 125.f);

    // Play Button
    m_retryButton.setFont(m_context->m_assets->getFont("main"));
    m_retryButton.setString("Retry");
    m_retryButton.setOrigin(m_retryButton.getLocalBounds().width / 2, m_retryButton.getLocalBounds().height / 2);
    m_retryButton.setPosition(m_context->m_window->getSize().x / 2, m_context->m_window->getSize().y / 2 - 25.f);
    m_retryButton.setCharacterSize(50);

    // Exit Button
    m_exitButton.setFont(m_context->m_assets->getFont("main"));
    m_exitButton.setString("Exit");
    m_exitButton.setOrigin(m_exitButton.getLocalBounds().width / 2, m_exitButton.getLocalBounds().height / 2);
    m_exitButton.setPosition(m_context->m_window->getSize().x / 2, m_context->m_window->getSize().y / 2 + 25.f);
    m_exitButton.setCharacterSize(50);
}

void GameOver::ProcessInput()
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
            switch (event.key.code)
            {
            case sf::Keyboard::Up:
            {
                if (!m_isRetryButtonSelected)
                {
                    m_isRetryButtonSelected = true;
                    m_isExitButtonSelected = false;
                }
                break;
            }
            case sf::Keyboard::W:
            {
                if (!m_isRetryButtonSelected)
                {
                    m_isRetryButtonSelected = true;
                    m_isExitButtonSelected = false;
                }
                break;
            }

            case sf::Keyboard::Down:
            {
                if (!m_isExitButtonSelected)
                {
                    m_isRetryButtonSelected = false;
                    m_isExitButtonSelected = true;
                }
                break;
            }
            case sf::Keyboard::S:
            {
                if (!m_isExitButtonSelected)
                {
                    m_isRetryButtonSelected = false;
                    m_isExitButtonSelected = true;
                }
                break;
            }

            case sf::Keyboard::Return:
            {
                m_isRetryButtonPressed = false;
                m_isExitButtonPressed = false;

                if (m_isRetryButtonSelected)
                {
                    m_isRetryButtonPressed = true;
                }
                else
                {
                    m_isExitButtonPressed = true;
                }

                break;
            }

            default:
            {
                break;
            }
            }
        }
    }
}

void GameOver::Update(sf::Time deltaTime)
{
    if (m_isRetryButtonSelected)
    {
        m_retryButton.setFillColor(sf::Color::Green);
        m_exitButton.setFillColor(sf::Color::White);
    }
    else
    {
        m_exitButton.setFillColor(sf::Color::Green);
        m_retryButton.setFillColor(sf::Color::White);
    }

    if (m_isRetryButtonPressed)
    {
        m_context->m_states->Add(std::make_unique<Scene_Snake>(m_context), true);
    }
    else if (m_isExitButtonPressed)
    {
        m_context->m_states->PopAll();
    }
}

void GameOver::Draw()
{
    m_context->m_window->clear(sf::Color::Black);
    m_context->m_window->draw(m_gameOverTitle);
    m_context->m_window->draw(m_retryButton);
    m_context->m_window->draw(m_exitButton);
    m_context->m_window->display();
}