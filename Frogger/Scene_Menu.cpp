#include "Scene_Menu.h"
#include "Scene_Snake.h"
#include "MusicPlayer.h"
#include <memory>
#include "Scene_Instruction.h"
#include "Scene_DisplayHighScore.h"

void Scene_Menu::onEnd()
{
	_game->window().close();
}

Scene_Menu::Scene_Menu(GameEngine* gameEngine)
	: Scene(gameEngine)
{
	init();
}

void Scene_Menu::init()
{
	MusicPlayer::getInstance().play("gameTheme");
	MusicPlayer::getInstance().setVolume(90);

	registerAction(sf::Keyboard::W, "UP");
	registerAction(sf::Keyboard::Up, "UP");
	registerAction(sf::Keyboard::S, "DOWN");
	registerAction(sf::Keyboard::Down, "DOWN");
	registerAction(sf::Keyboard::D, "PLAY");
	registerAction(sf::Keyboard::Escape, "QUIT");

	_title = "Snappy Snake";
	_menuStrings.push_back("Play");
	_menuStrings.push_back("How to play");
	_menuStrings.push_back("High Score");

	_levelPaths.push_back("../assets/level1.txt");

	_menuText.setFont(Assets::getInstance().getFont("main"));

	const size_t CHAR_SIZE{ 64 };
	_menuText.setCharacterSize(CHAR_SIZE);

	_backgroundSprite.setTexture(Assets::getInstance().getTexture("logo"));

	sf::Vector2u winSize = _game->window().getSize();

	sf::Vector2u textureSize = Assets::getInstance().getTexture("logo").getSize();
	_backgroundSprite.setScale(
		static_cast<float>(winSize.x) / textureSize.x,
		static_cast<float>(winSize.y) / textureSize.y
	);
}

void Scene_Menu::update(sf::Time dt)
{
	_entityManager.update();
}

void Scene_Menu::sRender()
{
    sf::View view = _game->window().getView();
    view.setCenter(_game->window().getSize().x / 2.f, _game->window().getSize().y / 2.f);
    _game->window().setView(view);

    static const sf::Color selectedColor(255, 255, 255);
    static const sf::Color normalColor(128, 128, 128);
    static const sf::Color backgroundColor(100, 100, 255);

    sf::Text footer("UP: W    DOWN: S   PLAY:D    QUIT: ESC", Assets::getInstance().getFont("main"), 45);
    footer.setFillColor(normalColor);
    sf::FloatRect footerBounds = footer.getLocalBounds();
    footer.setPosition((_game->window().getSize().x - footerBounds.width) / 2.f, (_game->window().getSize().y - footerBounds.height) / 2.f);

    _game->window().clear(backgroundColor);
    _game->window().draw(_backgroundSprite);

    _menuText.setFillColor(normalColor);
    _menuText.setString(_title);
    sf::FloatRect titleBounds = _menuText.getLocalBounds();
    _menuText.setPosition((_game->window().getSize().x - titleBounds.width) / 2.f, 10);
    _game->window().draw(_menuText);

    for (size_t i{ 0 }; i < _menuStrings.size(); ++i)
    {
        _menuText.setFillColor((i == _menuIndex ? selectedColor : normalColor));
        _menuText.setString(_menuStrings.at(i));
        sf::FloatRect textBounds = _menuText.getLocalBounds();
        _menuText.setPosition((_game->window().getSize().x - textBounds.width) / 2.f, 32 + (i + 1) * 96);
        _game->window().draw(_menuText);
    }

    _game->window().draw(footer);
	//m_game->window().display();
}

void Scene_Menu::sDoAction(const Command& action)
{
	if (action.type() == "START")
	{
		if (action.name() == "UP")
		{
			_menuIndex = (_menuIndex + _menuStrings.size() - 1) % _menuStrings.size();
		}
		else if (action.name() == "DOWN")
		{
			_menuIndex = (_menuIndex + 1) % _menuStrings.size();
		}
		else if (action.name() == "PLAY")
		{
			if (_menuIndex == 0)
			{
				_game->changeScene("PLAY", std::make_shared<Scene_Snake>(_game, _levelPaths[0]));
			}
			else if (_menuIndex == 1)
			{
				_game->changeScene("INSTRUCTION", std::make_shared<Scene_Instruction>(_game), false);
			}
			else if (_menuIndex == 2)
			{
				_game->changeScene("HIGHSCORES", std::make_shared<Scene_DisplayHighScore>(_game), false);
			}
		}
		else if (action.name() == "QUIT")
		{
			_game->quit();  // This will close the window and stop the game
		}
	}
}
