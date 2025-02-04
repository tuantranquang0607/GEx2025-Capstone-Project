#include "Scene_Menu.h"
#include <memory>

void Scene_Menu::onEnd()
{
	_game->window().close();
}

Scene_Menu::Scene_Menu(GameEngine* gameEngine) : Scene(gameEngine)
{
	init();
}



void Scene_Menu:: init()
{
	// Register actions
	registerAction(sf::Keyboard::W, "UP");
	registerAction(sf::Keyboard::Up, "UP");

	registerAction(sf::Keyboard::S, "DOWN");
	registerAction(sf::Keyboard::Down, "DOWN");

	registerAction(sf::Keyboard::D, "PLAY");
	registerAction(sf::Keyboard::Right, "PLAY");

	registerAction(sf::Keyboard::Escape, "QUIT");

	// Strings
	_title = "Snappy Snake";

	_menuStrings.push_back("Play");
	_menuStrings.push_back("About the game");
	_menuStrings.push_back("About the developer");

	_levelPaths.push_back("../level1.txt");
	_levelPaths.push_back("../level2.txt");
	_levelPaths.push_back("../level3.txt");

	const size_t CHAR_SIZE{ 64 };
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

	sf::Text footer("UP: W   DOWN: S   PLAY: D   QUIT: EXCAPE", Assets::getInstance().getFont("main"), 20);

	footer.setFillColor(normalColor);
	auto winSize = _game->windowSize();
	footer.setPosition(32, winSize.y - 32);

	_game->window().clear(backgroundColor);

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
			//_game->changeScene("PLAY", std::make_shared<Scene_Breakout>(_game, _levelPaths[_menuIndex]));    
		}    
		else if (action.name() == "QUIT")
		{       
			onEnd();    
		}
	}
}
