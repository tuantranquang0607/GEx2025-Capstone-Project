#include "Scene_Snake.h"
#include "Scene_Menu.h"
#include "Scene_Snake.h"

#include <SFML/Graphics.hpp>

Scene_Snake::Scene_Snake() : 
	m_context(std::make_shared<Context>())
{
	m_context->m_window->create(sf::VideoMode(900, 900), "Snappy Snake", sf::Style::Close);
}

Scene_Snake::~Scene_Snake()
{
}

void Scene_Snake::Run()
{
    sf::CircleShape shape(100.f);
    shape.setFillColor(sf::Color::Green);

    sf::Clock clock;
    sf::Time timeSinceLastFrame = sf::Time::Zero;

    while (m_context->m_window->isOpen())
    {
        sf::CircleShape shape(100.f);
        shape.setFillColor(sf::Color::Green);

        sf::Clock clock;
        sf::Time timeSinceLastFrame = sf::Time::Zero;

        while (m_context->m_window->isOpen())
        {
            timeSinceLastFrame += clock.restart();

            while (timeSinceLastFrame > TIME_PER_FRAME)
            {
                timeSinceLastFrame -= TIME_PER_FRAME;

                m_context->m_states->ProcessStateChange();

                if (!m_context->m_states->IsEmpty())
                {
                    m_context->m_states->GetCurrent()->ProcessInput();
                    m_context->m_states->GetCurrent()->Update(TIME_PER_FRAME);
                    m_context->m_states->GetCurrent()->Draw();
                }
                else
                {
                    m_context->m_window->close();
                }
            }
        }
    }
}
