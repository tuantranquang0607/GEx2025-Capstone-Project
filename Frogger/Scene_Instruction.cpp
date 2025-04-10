#include "Scene_Instruction.h"
#include "Assets.h"
#include "GameEngine.h"
#include "Scene_Menu.h"  // So we can return to the menu

Scene_Instruction::Scene_Instruction(GameEngine* gameEngine)
    : Scene(gameEngine)
{
    init();
}

void Scene_Instruction::init()
{
    registerAction(sf::Keyboard::Escape, "BACK");

    sf::Vector2u winSize = _game->window().getSize();

    m_backgroundSprite.setTexture(Assets::getInstance().getTexture("instruction"));
    sf::Vector2u texSize = m_backgroundSprite.getTexture()->getSize();
    m_backgroundSprite.setScale( static_cast<float>(winSize.x) / texSize.x, static_cast<float>(winSize.y) / texSize.y);
}

void Scene_Instruction::update(sf::Time dt)
{
    _entityManager.update();
}

void Scene_Instruction::sRender()
{
    _game->window().clear(sf::Color(100, 100, 255));
    _game->window().draw(m_backgroundSprite);
}

void Scene_Instruction::sDoAction(const Command& action)
{
    if (action.type() == "START")
    {
        if (action.name() == "BACK")
        {
            _game->changeScene("MENU", std::make_shared<Scene_Menu>(_game), true);
        }
    }
}
