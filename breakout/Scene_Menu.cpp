#include "Scene_Menu.h"
#include "Scene_Snake.h" 
#include "MusicPlayer.h"

#include <memory>
#include <iostream>

void Scene_Menu::onEnd()
{
	_game->window().close();
}

Scene_Menu::Scene_Menu(GameEngine* gameEngine) : Scene(gameEngine)
{
	init();
}

void Scene_Menu::init()
{
	MusicPlayer::getInstance().play("menuTheme");
	MusicPlayer::getInstance().setVolume(75);

	// Register actions
	registerAction(sf::Keyboard::W, "UP");
	registerAction(sf::Keyboard::Up, "UP");

	registerAction(sf::Keyboard::S, "DOWN");
	registerAction(sf::Keyboard::Down, "DOWN");

	registerAction(sf::Keyboard::D, "PLAY");
	registerAction(sf::Keyboard::Right, "PLAY");

	registerAction(sf::Keyboard::Escape, "QUIT");

	// Load the background texture and set the sprite
	if (!_backgroundTexture.loadFromFile("../assets/logo.png"))
	{
		std::cout << "Failed to load background image!" << std::endl;
	}
	_backgroundSprite.setTexture(_backgroundTexture);

	// Scale background to fit window size
	sf::Vector2u windowSize = _game->window().getSize();
	_backgroundSprite.setScale(
		static_cast<float>(windowSize.x) / _backgroundTexture.getSize().x,
		static_cast<float>(windowSize.y) / _backgroundTexture.getSize().y
	);

	// Strings
	_title = "Snappy Snake";

	_menuStrings.push_back("Play");
	_menuStrings.push_back("About the game");
	_menuStrings.push_back("About the developer");

	_levelPaths.push_back("../level1.txt");
	_levelPaths.push_back("../level2.txt");
	_levelPaths.push_back("../level3.txt");

	const size_t CHAR_SIZE{ 100 };
	_menuText.setCharacterSize(CHAR_SIZE);
	_menuText.setFont(Assets::getInstance().getFont("main"));
}

void Scene_Menu::update(sf::Time dt)
{
	_entityManager.update();
}

void Scene_Menu::sRender()
{
	static const sf::Color selelctedColor(255, 255, 255);
	static const sf::Color normalColor(0, 0, 0);
	static const sf::Color backgroundColor(100, 100, 255);

	sf::RenderTexture image;

	sf::Text footer("UP: W   DOWN: S   PLAY: D   QUIT: EXCAPE", Assets::getInstance().getFont("main"), 20);

	footer.setFillColor(normalColor);
	auto winSize = _game->windowSize();
	footer.setPosition(32, winSize.y - 32);

	_game->window().clear(backgroundColor);

	_game->window().draw(_backgroundSprite);	// Draw the background first

	// title
	_menuText.setFillColor(normalColor);
	_menuText.setPosition(10, 10);
	_menuText.setString(_title);

	_game->window().draw(_menuText);

	for (size_t i{0}; i < _menuStrings.size(); ++i)
	{
		_menuText.setFillColor((i == _menuIndex) ? selelctedColor : normalColor);
		_menuText.setPosition(32, 32 + (i + 1) * 96);
		_menuText.setString(_menuStrings.at(i));

		_game->window().draw(_menuText);
	}

	_game->window().draw(footer);
	_game->window().display();
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
			/*_game->changeScene("PLAY", std::make_shared<Scene_Snake>(_game));*/

			//if (_menuIndex < _levelPaths.size())
			//{
			//	// Start the Snake game window
			//	std::shared_ptr<Scene_Snake> snakeScene = std::make_shared<Scene_Snake>(_game, _levelPaths[_menuIndex]);
			//	// Launch the separate Snake game window
			//	snakeScene->launchSnakeGameWindow(); 
			//}
		}
		else if (action.name() == "QUIT")
		{
			onEnd();
		}
	}
}
