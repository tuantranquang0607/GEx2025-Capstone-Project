#include "Scene_Snake.h"
#include "Components.h"
#include "Physics.h"
#include "Utilities.h"
#include "MusicPlayer.h"
#include "Assets.h"
#include "SoundPlayer.h"
#include "GameEngine.h"
#include "Scene_Menu.h"

#include <random>
#include <fstream>
#include <iostream>

namespace
{
	std::random_device rd;
	std::mt19937 rng(rd());
}

Scene_Snake::Scene_Snake(GameEngine* gameEngine, const std::string& levelPath)
	: Scene(gameEngine)
{
	init(levelPath);
}

void Scene_Snake::init(const std::string& levelPath)
{
	loadLevel(levelPath);
	registerActions();

	sf::Vector2f spawnPos{ _game->windowSize().x / 2.f, _game->windowSize().y / 2.f };

	spawnPlayer(spawnPos);
	spawnWalls();
	spawnApple();

	MusicPlayer::getInstance().play("gameTheme");
	MusicPlayer::getInstance().setVolume(100);
}

void Scene_Snake::update(sf::Time dt)
{
	if (_lives < 1)
	{
		SoundPlayer::getInstance().play("death", _player->getComponent<CTransform>().pos);
	}

	sUpdate(dt);
}

void Scene_Snake::sRender()
{
	for (auto e : _entityManager.getEntities("bkg"))
	{
		if (e->getComponent<CSprite>().has)
		{
			auto& sprite = e->getComponent<CSprite>().sprite;
			_game->window().draw(sprite);
		}
	}

	for (auto& e : _entityManager.getEntities())
	{
		if (e->getTag() == "bkg")
			continue;

		auto& tfm = e->getComponent<CTransform>();

		if (e->hasComponent<CSprite>())
		{
			auto& sprite = e->getComponent<CSprite>().sprite;
			auto& tfm = e->getComponent<CTransform>();
			sprite.setPosition(tfm.pos);

			_game->window().draw(sprite);
		}

		if (_drawAABB && e->hasComponent<CBoundingBox>())
		{
			auto box = e->getComponent<CBoundingBox>();
			sf::RectangleShape rect;
			rect.setSize(sf::Vector2f{ box.size.x, box.size.y });
			centerOrigin(rect);
			rect.setPosition(e->getComponent<CTransform>().pos);
			rect.setFillColor(sf::Color(0, 0, 0, 0));
			rect.setOutlineColor(sf::Color{ 0, 255, 0 });
			rect.setOutlineThickness(2.f);
			_game->window().draw(rect);
		}
	}

	drawScore();

	if (_lives < 1)
	{
		drawGameOver();
		return;
	}
}

void Scene_Snake::sDoAction(const Command& command)
{
	int gridCount = 31;
	float gridSize = static_cast<float>(_game->window().getSize().x) / gridCount;

	if (command.type() == "START")
	{
		if (command.name() == "PAUSE")
		{
			setPaused(!_isPaused);
		}
		else if (command.name() == "QUIT")
		{
			_game->quitLevel();
		}
		else if (command.name() == "BACK")
		{
			_game->backLevel();
		}
		else if (command.name() == "TOGGLE_TEXTURE")
		{
			_drawTextures = !_drawTextures;
		}
		else if (command.name() == "TOGGLE_COLLISION")
		{
			_drawAABB = !_drawAABB;
		}
		else if (command.name() == "TOGGLE_CAMOUTLINE")
		{
			_drawCam = !_drawCam;
		}
		else if (command.name() == "LEFT")
		{
			_player->getComponent<CTransform>().vel = sf::Vector2f(-gridSize, 0);
		}
		else if (command.name() == "RIGHT")
		{
			_player->getComponent<CTransform>().vel = sf::Vector2f(gridSize, 0);
		}
		else if (command.name() == "UP")
		{
			_player->getComponent<CTransform>().vel = sf::Vector2f(0, -gridSize);
		}
		else if (command.name() == "DOWN")
		{
			_player->getComponent<CTransform>().vel = sf::Vector2f(0, gridSize);
		}
	}
	else if (command.type() == "END")
	{
		_player->getComponent<CInput>().dir = 0;
	}
}

void Scene_Snake::registerActions()
{
	registerAction(sf::Keyboard::Z, "ZOOMOUT");
	registerAction(sf::Keyboard::X, "ZOOMIN");

	registerAction(sf::Keyboard::P, "PAUSE");
	registerAction(sf::Keyboard::Escape, "BACK");
	registerAction(sf::Keyboard::Q, "QUIT");

	registerAction(sf::Keyboard::C, "TOGGLE_COLLISION");
	registerAction(sf::Keyboard::T, "TOGGLE_TEXTURE");
	registerAction(sf::Keyboard::V, "TOGGLE_CAMOUTLINE");

	registerAction(sf::Keyboard::A, "LEFT");
	registerAction(sf::Keyboard::Left, "LEFT");
	registerAction(sf::Keyboard::D, "RIGHT");
	registerAction(sf::Keyboard::Right, "RIGHT");
	registerAction(sf::Keyboard::W, "UP");
	registerAction(sf::Keyboard::Up, "UP");
	registerAction(sf::Keyboard::S, "DOWN");
	registerAction(sf::Keyboard::Down, "DOWN");
}

void Scene_Snake::spawnWalls()
{
	int gridCount = 31;
	float gridSize = static_cast<float>(_game->window().getSize().x) / gridCount;

	for (int x = 0; x < gridCount; x++)
	{
		sf::Vector2f pos(x * gridSize + gridSize / 2.f, gridSize / 2.f);
		auto wall = _entityManager.addEntity("wall");
		wall->addComponent<CTransform>(pos);
		auto& sprite = wall->addComponent<CSprite>(Assets::getInstance().getTexture("wall")).sprite;
		centerOrigin(sprite);
		float scaleX = gridSize / sprite.getLocalBounds().width;
		float scaleY = gridSize / sprite.getLocalBounds().height;
		sprite.setScale(scaleX, scaleY);
		wall->addComponent<CBoundingBox>(sf::Vector2f(gridSize, gridSize));
	}

	for (int x = 0; x < gridCount; x++)
	{
		sf::Vector2f pos(x * gridSize + gridSize / 2.f, (gridCount - 1) * gridSize + gridSize / 2.f);
		auto wall = _entityManager.addEntity("wall");
		wall->addComponent<CTransform>(pos);
		auto& sprite = wall->addComponent<CSprite>(Assets::getInstance().getTexture("wall")).sprite;
		centerOrigin(sprite);
		float scaleX = gridSize / sprite.getLocalBounds().width;
		float scaleY = gridSize / sprite.getLocalBounds().height;
		sprite.setScale(scaleX, scaleY);
		wall->addComponent<CBoundingBox>(sf::Vector2f(gridSize, gridSize));
	}

	for (int y = 1; y < gridCount - 1; y++)
	{
		sf::Vector2f pos(gridSize / 2.f, y * gridSize + gridSize / 2.f);
		auto wall = _entityManager.addEntity("wall");
		wall->addComponent<CTransform>(pos);
		auto& sprite = wall->addComponent<CSprite>(Assets::getInstance().getTexture("wall")).sprite;
		centerOrigin(sprite);
		float scaleX = gridSize / sprite.getLocalBounds().width;
		float scaleY = gridSize / sprite.getLocalBounds().height;
		sprite.setScale(scaleX, scaleY);
		wall->addComponent<CBoundingBox>(sf::Vector2f(gridSize, gridSize));
	}

	for (int y = 1; y < gridCount - 1; y++)
	{
		sf::Vector2f pos((gridCount - 1) * gridSize + gridSize / 2.f, y * gridSize + gridSize / 2.f);
		auto wall = _entityManager.addEntity("wall");
		wall->addComponent<CTransform>(pos);
		auto& sprite = wall->addComponent<CSprite>(Assets::getInstance().getTexture("wall")).sprite;
		centerOrigin(sprite);
		float scaleX = gridSize / sprite.getLocalBounds().width;
		float scaleY = gridSize / sprite.getLocalBounds().height;
		sprite.setScale(scaleX, scaleY);
		wall->addComponent<CBoundingBox>(sf::Vector2f(gridSize, gridSize));
	}
}

void Scene_Snake::spawnApple()
{
	float cellSize = static_cast<float>(_game->window().getSize().x) / gridCount;

	std::uniform_int_distribution<int> dist(1, gridCount - 2);

	int cellX = dist(rng);
	int cellY = dist(rng);

	sf::Vector2f pos(cellX * cellSize + cellSize / 2.f, cellY * cellSize + cellSize / 2.f);
	sf::Vector2f snakePos = _player->getComponent<CTransform>().pos;

	int snakeCellX = static_cast<int>(snakePos.x / cellSize);
	int snakeCellY = static_cast<int>(snakePos.y / cellSize);

	if (snakeCellX == cellX && snakeCellY == cellY)
	{
		spawnApple();
		return;
	}

	auto apple = _entityManager.addEntity("apple");
	apple->addComponent<CTransform>(pos);
	auto& sprite = apple->addComponent<CSprite>(Assets::getInstance().getTexture("apple")).sprite;
	centerOrigin(sprite);

	float scaleX = cellSize / sprite.getLocalBounds().width;
	float scaleY = cellSize / sprite.getLocalBounds().height;
	sprite.setScale(scaleX, scaleY);

	apple->addComponent<CBoundingBox>(sf::Vector2f(cellSize, cellSize));
}

void Scene_Snake::checkWallCollision()
{
	if (!_player->hasComponent<CBoundingBox>())
		return;

	for (auto& wall : _entityManager.getEntities("wall"))
	{
        if (!wall->hasComponent<CBoundingBox>())
		{
			continue;
		}
		
		sf::Vector2f overlap = Physics::getOverlap(_player, wall);

		if (overlap.x > 0 && overlap.y > 0)
		{
			std::cout << "Collision with wall detected! Resetting game to menu.\n";
			_game->changeScene("MENU", std::make_shared<Scene_Menu>(_game), true);
			return;
		}
	}
}

void Scene_Snake::spawnPlayer(sf::Vector2f pos)
{
	_player = _entityManager.addEntity("player");

	_player->addComponent<CTransform>(pos);

	auto& sprite = _player->addComponent<CSprite>(Assets::getInstance().getTexture("snake")).sprite;

	sf::FloatRect bounds = sprite.getLocalBounds();

	sf::FloatRect scaledBounds = sprite.getGlobalBounds();
	_player->addComponent<CBoundingBox>(sf::Vector2f(scaledBounds.width, scaledBounds.height));

	_player->addComponent<CState>().state = "Alive";

	int gridCount = 21;
	float gridSize = static_cast<float>(_game->window().getSize().x) / gridCount;
	_player->getComponent<CTransform>().vel = sf::Vector2f(0, -gridSize);
}

void Scene_Snake::playerMovement(sf::Time dt)
{
	float gridSize = static_cast<float>(_game->window().getSize().x) / 31.f;

	sf::Vector2f movementDelta(0.f, 0.f);
	auto& pos = _player->getComponent<CTransform>().pos;

	if (_player->getComponent<CInput>().dir == 1)
	{
		movementDelta.y -= gridSize;
		_player->getComponent<CInput>().dir = 0;
		SoundPlayer::getInstance().play("hop", pos);
	}
	if (_player->getComponent<CInput>().dir == 2)
	{
		movementDelta.y += gridSize;
		_player->getComponent<CInput>().dir = 0;
		SoundPlayer::getInstance().play("hop", pos);
	}
	if (_player->getComponent<CInput>().dir == 4)
	{
		movementDelta.x -= gridSize;
		_player->getComponent<CInput>().dir = 0;
		SoundPlayer::getInstance().play("hop", pos);
	}
	if (_player->getComponent<CInput>().dir == 8)
	{
		movementDelta.x += gridSize;
		_player->getComponent<CInput>().dir = 0;
		SoundPlayer::getInstance().play("hop", pos);
	}

	if (movementDelta != sf::Vector2f(0.f, 0.f))
	{
		pos += movementDelta;
		std::cout << "Snake moved to (" << pos.x << ", " << pos.y << ")\n";
	}
}

void Scene_Snake::adjustPlayerPosition()
{
	auto& player_pos = _player->getComponent<CTransform>().pos;
	auto& playerSize = _player->getComponent<CBoundingBox>().size;
	auto& playerHalfSize = _player->getComponent<CBoundingBox>().halfSize;

	if (player_pos.x < playerHalfSize.x)
	{
		player_pos.x = playerHalfSize.x;
	}

	if (player_pos.x > (_game->windowSize().x - playerHalfSize.x))
	{
		player_pos.x = _game->windowSize().x - playerHalfSize.x;
	}

	if (player_pos.y < playerHalfSize.y)
	{
		player_pos.y = playerHalfSize.y;
	}

	if (player_pos.y > _game->windowSize().y - playerHalfSize.y)
	{
		player_pos.y = _game->windowSize().y - playerHalfSize.y;
	}
}

void Scene_Snake::loadLevel(const std::string& path)
{
	std::ifstream config(path);
	if (config.fail())
	{
		std::cerr << "Open file " << path << " failed\n";
		config.close();
		exit(1);
	}

	std::string token{ "" };
	config >> token;
	while (!config.eof())
	{
		if (token == "Bkg")
		{
			std::string name;
			sf::Vector2f pos;
			config >> name >> pos.x >> pos.y;
			auto e = _entityManager.addEntity("bkg");

			auto& sprite = e->addComponent<CSprite>(Assets::getInstance().getTexture(name)).sprite;
			sprite.setOrigin(0.f, 0.f);
			sprite.setPosition(pos);

			sf::Vector2u windowSize = _game->window().getSize();
			sf::Vector2u textureSize = Assets::getInstance().getTexture(name).getSize();
			float scaleX = static_cast<float>(windowSize.x) / textureSize.x;
			float scaleY = static_cast<float>(windowSize.y) / textureSize.y;
			sprite.setScale(scaleX, scaleY);
		}
		else if (token == "World")
		{
			config >> _worldBounds.width >> _worldBounds.height;
		}
		config >> token;
	}

	config.close();
}

void Scene_Snake::sMovement(sf::Time dt)
{
	playerMovement(dt);

	for (auto& e : _entityManager.getEntities())
	{
		auto& tfm = e->getComponent<CTransform>();
		tfm.pos += tfm.vel * dt.asSeconds();
		tfm.angle += tfm.angVel * dt.asSeconds();
	}
}

void Scene_Snake::sCollisions()
{
	checkWallCollision();
}

void Scene_Snake::sUpdate(sf::Time dt)
{
	if (_isPaused)
		return;
	SoundPlayer::getInstance().removeStoppedSounds();
	_entityManager.update();

	sMovement(dt);
	sCollisions();
	adjustPlayerPosition();
}

void Scene_Snake::onEnd()
{
	_game->changeScene("MENU", nullptr, false);
}

void Scene_Snake::drawScore()
{
	int totalScore = 0;
	for (int i = 0; i < 11; ++i)
	{
		totalScore += _scoredHeights[i];
	}
	_score = totalScore;

	std::string str = std::to_string(_scoreTotal + _score);
	sf::Text text = sf::Text("SCORE: " + str, Assets::getInstance().getFont("Arial"), 15);

	text.setPosition(10.f, 10.f);
	_game->window().draw(text);
}

void Scene_Snake::getScore()
{
	auto pos = _player->getComponent<CTransform>().pos.y;
	int posY = static_cast<int>(pos);
}

void Scene_Snake::drawGameOver()
{
	std::string str = "GAME OVER";
	sf::Text text = sf::Text(str, Assets::getInstance().getFont("Arial"), 60);
	centerOrigin(text);
	text.setPosition(240.f, 300.f);
	_game->window().draw(text);

	std::string strEsc = "Press ESC";
	sf::Text textEsc = sf::Text(strEsc, Assets::getInstance().getFont("Arial"), 40);
	centerOrigin(textEsc);
	textEsc.setPosition(240.f, 340.f);
	_game->window().draw(textEsc);
}
