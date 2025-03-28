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

void Scene_Snake::growSnake()
{
	auto tail = _snakeSegments.back();
	sf::Vector2f tailPos = tail->getComponent<CTransform>().pos;

	auto newSegment = _entityManager.addEntity("snakeSegment");
	newSegment->addComponent<CTransform>(tailPos);

	auto& sprite = newSegment->addComponent<CSprite>(Assets::getInstance().getTexture("snakeTail")).sprite;
	centerOrigin(sprite);
	float scaleX = gridSize / sprite.getLocalBounds().width;
	float scaleY = gridSize / sprite.getLocalBounds().height;
	sprite.setScale(scaleX, scaleY);

	newSegment->addComponent<CBoundingBox>(sf::Vector2f(gridSize, gridSize));

	_snakeSegments.push_back(newSegment);

	std::cout << "Snake grew!\n";
}

Scene_Snake::Scene_Snake(GameEngine* gameEngine, const std::string& levelPath) : Scene(gameEngine)
{
	gridCount = 31;
	gridSize = static_cast<float>(_game->window().getSize().x) / gridCount;

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
	spawnOrange();
	spawnBlueberry();

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
			_player->getComponent<CTransform>().vel = sf::Vector2f(-snakeSpeed * gridSize, 0);
		}
		else if (command.name() == "RIGHT")
		{
			_player->getComponent<CTransform>().vel = sf::Vector2f(snakeSpeed * gridSize, 0);
		}
		else if (command.name() == "UP")
		{
			_player->getComponent<CTransform>().vel = sf::Vector2f(0, -snakeSpeed * gridSize);
		}
		else if (command.name() == "DOWN")
		{
			_player->getComponent<CTransform>().vel = sf::Vector2f(0, snakeSpeed * gridSize);
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

	std::cout << "Walls spawned\n";
}

void Scene_Snake::spawnApple()
{
	std::uniform_int_distribution<int> dist(1, gridCount - 2);

	int cellX = dist(rng);
	int cellY = dist(rng);

	sf::Vector2f pos(cellX * gridSize + gridSize / 2.f, cellY * gridSize + gridSize / 2.f);
	sf::Vector2f snakePos = _player->getComponent<CTransform>().pos;

	int snakeCellX = static_cast<int>(snakePos.x / gridSize);
	int snakeCellY = static_cast<int>(snakePos.y / gridSize);

	if (snakeCellX == cellX && snakeCellY == cellY)
	{
		spawnApple();
		return;
	}

	auto apple = _entityManager.addEntity("apple");
	apple->addComponent<CTransform>(pos);
	auto& sprite = apple->addComponent<CSprite>(Assets::getInstance().getTexture("apple")).sprite;
	centerOrigin(sprite);

	float scaleX = gridSize / sprite.getLocalBounds().width;
	float scaleY = gridSize / sprite.getLocalBounds().height;
	sprite.setScale(scaleX, scaleY);

	apple->addComponent<CBoundingBox>(sf::Vector2f(gridSize, gridSize));

	std::cout << "Apple spawned at (" << pos.x << ", " << pos.y << ")\n";
}

void Scene_Snake::spawnOrange()
{
	// Random cell from 1 to gridCount - 2 (to avoid the walls)
	std::uniform_int_distribution<int> dist(1, gridCount - 2);
	int cellX = dist(rng);
	int cellY = dist(rng);

	if (isCellOccupied(cellX, cellY, gridSize))
	{
		spawnOrange(); // Try a new random position.
		return;
	}

	sf::Vector2f pos(cellX * gridSize + gridSize / 2.f, cellY * gridSize + gridSize / 2.f);
	auto orange = _entityManager.addEntity("orange");
	orange->addComponent<CTransform>(pos);
	auto& sprite = orange->addComponent<CSprite>(Assets::getInstance().getTexture("orange")).sprite;
	centerOrigin(sprite);
	float scaleX = gridSize / sprite.getLocalBounds().width;
	float scaleY = gridSize / sprite.getLocalBounds().height;
	sprite.setScale(scaleX, scaleY);
	orange->addComponent<CBoundingBox>(sf::Vector2f(gridSize, gridSize));

	std::cout << "Orange spawned at (" << pos.x << ", " << pos.y << ")\n";
}

void Scene_Snake::spawnBlueberry()
{
	std::uniform_int_distribution<int> dist(1, gridCount - 2);
	int cellX = dist(rng);
	int cellY = dist(rng);

	if (isCellOccupied(cellX, cellY, gridSize))
	{
		spawnBlueberry();
		return;
	}

	sf::Vector2f pos(cellX * gridSize + gridSize / 2.f, cellY * gridSize + gridSize / 2.f);
	auto blueberry = _entityManager.addEntity("blueberry");
	blueberry->addComponent<CTransform>(pos);
	auto& sprite = blueberry->addComponent<CSprite>(Assets::getInstance().getTexture("blueberry")).sprite;
	centerOrigin(sprite);
	float scaleX = gridSize / sprite.getLocalBounds().width;
	float scaleY = gridSize / sprite.getLocalBounds().height;
	sprite.setScale(scaleX, scaleY);
	blueberry->addComponent<CBoundingBox>(sf::Vector2f(gridSize, gridSize));

	std::cout << "Blueberry spawned at (" << pos.x << ", " << pos.y << ")\n";
}

void Scene_Snake::checkWallCollision()
{
	if (!_player->hasComponent<CBoundingBox>())
		return;

	for (auto& wall : _entityManager.getEntities("wall"))
	{
        if (!wall->hasComponent<CBoundingBox>())
			continue;
		
		sf::Vector2f overlap = Physics::getOverlap(_player, wall);

		if (overlap.x > 0 && overlap.y > 0)
		{
			std::cout << "Collision with wall detected! Resetting game to menu.\n";
			_game->changeScene("MENU", std::make_shared<Scene_Menu>(_game), true);
			return;
		}
	}
}

void Scene_Snake::checkAppleCollision()
{
	if (!_player->hasComponent<CBoundingBox>())
		return;

	for (auto& apple : _entityManager.getEntities("apple"))
	{
		if (!apple->hasComponent<CBoundingBox>())
			continue;

		sf::Vector2f overlap = Physics::getOverlap(_player, apple);

		if (overlap.x > 0 && overlap.y > 0)
		{
			std::cout << "Apple collision detected!\n";

			_scoreTotal += 10;

			apple->destroy();

			spawnApple();

			growSnake();
			growSnake();
			growSnake();
			growSnake();
			growSnake();
			growSnake();

			break;
		}
	}

}

void Scene_Snake::checkOrangeCollision()
{
	if (!_player->hasComponent<CBoundingBox>())
		return;

	for (auto& orange : _entityManager.getEntities("orange"))
	{
		if (!orange->hasComponent<CBoundingBox>())
			continue;

		sf::Vector2f overlap = Physics::getOverlap(_player, orange);

		if (overlap.x > 0 && overlap.y > 0)
		{
			std::cout << "Orange collision detected!\n";

			_scoreTotal += 20;

			orange->destroy();

			spawnOrange();

			break;
		}
	}
}

void Scene_Snake::checkBlueberryCollision()
{
	if (!_player->hasComponent<CBoundingBox>())
		return;

	for (auto& blueberry : _entityManager.getEntities("blueberry"))
	{
		if (!blueberry->hasComponent<CBoundingBox>())
			continue;

		sf::Vector2f overlap = Physics::getOverlap(_player, blueberry);

		if (overlap.x > 0 && overlap.y > 0)
		{
			std::cout << "Blueberry collision detected!\n";
			_scoreTotal += 30;

			blueberry->destroy();
			spawnBlueberry();
			break;
		}
	}
}

bool Scene_Snake::isCellOccupied(int cellX, int cellY, float cellSize)
{
	// Check the snake (player)
	sf::Vector2f playerPos = _player->getComponent<CTransform>().pos;
	int pCellX = static_cast<int>(playerPos.x / cellSize);
	int pCellY = static_cast<int>(playerPos.y / cellSize);
	if (pCellX == cellX && pCellY == cellY)
		return true;

	// Check apples
	for (auto& apple : _entityManager.getEntities("apple"))
	{
		sf::Vector2f pos = apple->getComponent<CTransform>().pos;
		int eCellX = static_cast<int>(pos.x / cellSize);
		int eCellY = static_cast<int>(pos.y / cellSize);
		if (eCellX == cellX && eCellY == cellY)
			return true;
	}

	// Check oranges
	for (auto& orange : _entityManager.getEntities("orange"))
	{
		sf::Vector2f pos = orange->getComponent<CTransform>().pos;
		int eCellX = static_cast<int>(pos.x / cellSize);
		int eCellY = static_cast<int>(pos.y / cellSize);
		if (eCellX == cellX && eCellY == cellY)
			return true;
	}

	// Check blueberries
	for (auto& blueberry : _entityManager.getEntities("blueberry"))
	{
		sf::Vector2f pos = blueberry->getComponent<CTransform>().pos;
		int eCellX = static_cast<int>(pos.x / cellSize);
		int eCellY = static_cast<int>(pos.y / cellSize);
		if (eCellX == cellX && eCellY == cellY)
			return true;
	}
	return false;
}

void Scene_Snake::spawnPlayer(sf::Vector2f pos)
{
	_player = _entityManager.addEntity("player");
	_player->addComponent<CTransform>(pos);

	auto& sprite = _player->addComponent<CSprite>(Assets::getInstance().getTexture("snake")).sprite;

	centerOrigin(sprite);

	float scaleX = gridSize / sprite.getLocalBounds().width;
	float scaleY = gridSize / sprite.getLocalBounds().height;
	sprite.setScale(scaleX, scaleY);

	sf::FloatRect newBounds = sprite.getGlobalBounds();
	_player->addComponent<CBoundingBox>(sf::Vector2f(newBounds.width, newBounds.height));

	_player->addComponent<CState>().state = "Alive";

	_player->getComponent<CTransform>().vel = sf::Vector2f(0, -snakeSpeed * gridSize);

	_snakeSegments.push_back(_player);
}

void Scene_Snake::playerMovement(sf::Time dt)
{
	/*float gridSize = static_cast<float>(_game->window().getSize().x) / 31.f;

	sf::Vector2f movement(0.f, 0.f);
	auto& pos = _player->getComponent<CTransform>().pos;

	if (_player->getComponent<CInput>().dir == 1)
	{
		movement.y -= gridSize;
		_player->getComponent<CInput>().dir = 0;
		SoundPlayer::getInstance().play("hop", pos);
	}
	if (_player->getComponent<CInput>().dir == 2)
	{
		movement.y += gridSize;
		_player->getComponent<CInput>().dir = 0;
		SoundPlayer::getInstance().play("hop", pos);
	}
	if (_player->getComponent<CInput>().dir == 4)
	{
		movement.x -= gridSize;
		_player->getComponent<CInput>().dir = 0;
		SoundPlayer::getInstance().play("hop", pos);
	}
	if (_player->getComponent<CInput>().dir == 8)
	{
		movement.x += gridSize;
		_player->getComponent<CInput>().dir = 0;
		SoundPlayer::getInstance().play("hop", pos);
	}

	if (movement != sf::Vector2f(0.f, 0.f))
	{
		pos += movement;
		std::cout << "Snake moved to (" << pos.x << ", " << pos.y << ")\n";
	}*/
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
	std::vector<sf::Vector2f> oldPositions(_snakeSegments.size());
	for (size_t i = 0; i < _snakeSegments.size(); i++)
		oldPositions[i] = _snakeSegments[i]->getComponent<CTransform>().pos;

	auto& headTfm = _snakeSegments[0]->getComponent<CTransform>();
	headTfm.pos += headTfm.vel * dt.asSeconds();

	for (size_t i = 1; i < _snakeSegments.size(); i++)
	{
		_snakeSegments[i]->getComponent<CTransform>().pos = oldPositions[i - 1];
	}
}

void Scene_Snake::sCollisions()
{
	checkWallCollision();
	checkAppleCollision();
	checkOrangeCollision();
	checkBlueberryCollision();
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
