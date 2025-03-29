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

    m_instructionText.setFont(Assets::getInstance().getFont("main"));
    m_instructionText.setCharacterSize(36);
    m_instructionText.setFillColor(sf::Color::White);

    m_instructionText.setString(
        "How to Play:\n\n"
        "W / Up Arrow    : Move Up\n"
        "S / Down Arrow  : Move Down\n"
        "A / Left Arrow  : Move Left\n"
        "D / Right Arrow : Move Right\n\n"
        "Avoid colliding with the walls.\n"
        "Eat fruits to grow, shrink, slow down, and get score.\n\n"
        "Press ESC to return to the Menu."
    );

    sf::FloatRect bounds = m_instructionText.getLocalBounds();

    m_instructionText.setOrigin( bounds.left + bounds.width * 0.5f, bounds.top + 0.f);

    sf::Vector2u winSize = _game->window().getSize();
    float centerX = winSize.x / 2.f;
    float topMargin = 200.f;
    m_instructionText.setPosition(centerX, topMargin);

    m_backgroundSprite.setTexture(Assets::getInstance().getTexture("logo"));
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
    _game->window().draw(m_instructionText);
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
