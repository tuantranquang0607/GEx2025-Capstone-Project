// Scene_Snake.cpp: Implementation of the Scene_Snake class, handling the gameplay scene.

#include "Scene_Snake.h"           // Include the header for Scene_Snake class definition.
#include "Components.h"            // Include ECS component definitions.
#include "Physics.h"               // Include physics-related functions.
#include "Utilities.h"             // Include various utility functions (e.g., centerOrigin).
#include "MusicPlayer.h"           // Include MusicPlayer for background music control.
#include "Assets.h"                // Include Assets manager to load textures, fonts, animations, etc.
#include "SoundPlayer.h"           // Include SoundPlayer for sound effect handling.
#include "GameEngine.h"            // Include GameEngine for window management and scene switching.

#include <random>                  // Include random library for random number generation.
#include <fstream>                 // Include file stream library for reading level files.
#include <iostream>                // Include IO stream library for debugging output.

// Anonymous namespace for local random number generation utilities
namespace
{
	std::random_device rd;         // Seed for random number generation.
	std::mt19937 rng(rd());        // Mersenne Twister random number generator.
}

// Constructor for Scene_Snake:
// Initializes the gameplay scene using the provided GameEngine pointer and level file path.
Scene_Snake::Scene_Snake(GameEngine* gameEngine, const std::string& levelPath)
	: Scene(gameEngine)              // Initialize the base Scene with the game engine.
{
	init(levelPath);                // Call the initialization function with the level file path.
}

// Initialization function for the gameplay scene.
// Loads level data, registers input actions, spawns the player, and starts background music.
void Scene_Snake::init(const std::string& levelPath)
{
	loadLevel(levelPath);           // Load level configuration (world bounds, background, etc.) from file.
	registerActions();              // Set up the keyboard input mappings for gameplay.

	// Determine the spawn position: center of the window.
	sf::Vector2f spawnPos{ _game->windowSize().x / 2.f, _game->windowSize().y / 2.f };

	spawnPlayer(spawnPos);          // Create the player entity and position it at spawnPos.

	// Start playing the game theme music and set its volume.
	MusicPlayer::getInstance().play("gameTheme");
	MusicPlayer::getInstance().setVolume(100);
}

// Update function called every frame with the elapsed time (dt).
// Processes game state updates, such as checking player lives and triggering animations.
void Scene_Snake::update(sf::Time dt)
{
	// If the player has no lives left, trigger the death animation and play the death sound.
	if (_lives < 1)
	{
		SoundPlayer::getInstance().play("death", _player->getComponent<CTransform>().pos);
	}

	sUpdate(dt);                    // Call the main scene update function to process movement, collisions, etc.
}

// Render function to draw all game entities and HUD elements.
void Scene_Snake::sRender()
{
	// Draw background entities (tagged as "bkg").
	for (auto e : _entityManager.getEntities("bkg"))
	{
		if (e->getComponent<CSprite>().has)
		{
			auto& sprite = e->getComponent<CSprite>().sprite;
			_game->window().draw(sprite);  // Draw the background sprite.
		}
	}

	// Draw all other entities (skip those tagged as "bkg" since they're already drawn).
	for (auto& e : _entityManager.getEntities())
	{
		if (e->getTag() == "bkg")
			continue;

		// Get the entity's animation component and its transform.
		auto& tfm = e->getComponent<CTransform>();

		// Draw the entity's sprite if it has one.
		if (e->hasComponent<CSprite>())
		{
			auto& sprite = e->getComponent<CSprite>().sprite;
			// Position the sprite based on the transform
			auto& tfm = e->getComponent<CTransform>();
			sprite.setPosition(tfm.pos);

			_game->window().draw(sprite);
		}

		// Optionally, if collision debugging is enabled, draw the bounding box.
		if (_drawAABB && e->hasComponent<CBoundingBox>())
		{
			auto box = e->getComponent<CBoundingBox>();
			sf::RectangleShape rect;
			rect.setSize(sf::Vector2f{ box.size.x, box.size.y });
			centerOrigin(rect);  // Center the origin so the rectangle aligns with the entity.
			rect.setPosition(e->getComponent<CTransform>().pos);
			rect.setFillColor(sf::Color(0, 0, 0, 0));         // Transparent fill.
			rect.setOutlineColor(sf::Color{ 0, 255, 0 });      // Green outline for visibility.
			rect.setOutlineThickness(2.f);
			_game->window().draw(rect);  // Draw the bounding box.
		}
	}

	drawScore();  // Render the player's score on the screen.

	// If the player is out of lives, render the "Game Over" screen and exit the render function.
	if (_lives < 1)
	{
		drawGameOver();
		return;
	}
}

// Process input commands (actions) from the user.
void Scene_Snake::sDoAction(const Command& command)
{
	// If the action is a key press ("START" type)
	if (command.type() == "START")
	{
		if (command.name() == "PAUSE")
		{
			setPaused(!_isPaused);  // Toggle the pause state.
		}
		else if (command.name() == "QUIT")
		{
			_game->quitLevel();     // Quit the current level.
		}
		else if (command.name() == "BACK")
		{
			_game->backLevel();     // Return to the previous scene or menu.
		}
		else if (command.name() == "TOGGLE_TEXTURE")
		{
			_drawTextures = !_drawTextures;  // Toggle texture drawing (for debugging).
		}
		else if (command.name() == "TOGGLE_COLLISION")
		{
			_drawAABB = !_drawAABB;  // Toggle drawing collision boxes.
		}
		else if (command.name() == "TOGGLE_CAMOUTLINE")
		{
			_drawCam = !_drawCam;    // Toggle drawing camera outlines (if implemented).
		}
		// Handle player movement commands.
		else if (command.name() == "LEFT")
		{
			_player->getComponent<CInput>().dir |= CInput::dirs::LEFT;
		}
		else if (command.name() == "RIGHT")
		{
			_player->getComponent<CInput>().dir |= CInput::dirs::RIGHT;
		}
		else if (command.name() == "UP")
		{
			_player->getComponent<CInput>().dir |= CInput::dirs::UP;
		}
		else if (command.name() == "DOWN")
		{
			_player->getComponent<CInput>().dir |= CInput::dirs::DOWN;
		}
	}
	// If the action is a key release ("END" type), reset the player's movement direction.
	else if (command.type() == "END")
	{
		_player->getComponent<CInput>().dir = 0;
	}
}

// Register input actions (keyboard mappings) for the gameplay scene.
void Scene_Snake::registerActions()
{
	// Register zoom actions (if applicable).
	registerAction(sf::Keyboard::Z, "ZOOMOUT");
	registerAction(sf::Keyboard::X, "ZOOMIN");

	// Register pause, back, and quit actions.
	registerAction(sf::Keyboard::P, "PAUSE");
	registerAction(sf::Keyboard::Escape, "BACK");
	registerAction(sf::Keyboard::Q, "QUIT");

	// Register toggle actions for debugging visuals.
	registerAction(sf::Keyboard::C, "TOGGLE_COLLISION");
	registerAction(sf::Keyboard::T, "TOGGLE_TEXTURE");
	registerAction(sf::Keyboard::V, "TOGGLE_CAMOUTLINE");

	// Register actions for player movement, mapping both letter keys and arrow keys.
	registerAction(sf::Keyboard::A, "LEFT");
	registerAction(sf::Keyboard::Left, "LEFT");
	registerAction(sf::Keyboard::D, "RIGHT");
	registerAction(sf::Keyboard::Right, "RIGHT");
	registerAction(sf::Keyboard::W, "UP");
	registerAction(sf::Keyboard::Up, "UP");
	registerAction(sf::Keyboard::S, "DOWN");
	registerAction(sf::Keyboard::Down, "DOWN");
}

// Spawn the player entity at the specified position.
void Scene_Snake::spawnPlayer(sf::Vector2f pos)
{
	// Create the player entity with tag "player"
	_player = _entityManager.addEntity("player");

	// Add a transform component and initialize it with the spawn position
	_player->addComponent<CTransform>(pos);

	// Add a sprite component using the "snake" texture.
	// Make sure "snake" is loaded via your config.txt.
	auto& sprite = _player->addComponent<CSprite>(Assets::getInstance().getTexture("snake")).sprite;

	// Center the sprite's origin for proper positioning and rotation.
	centerOrigin(sprite);

	// ---- New Code for Scaling the Snake to Match the Grid Cell ----

	// Define the grid count (number of cells per row/column). 
	// This should match your game's current grid. For instance, if you changed your grid to 50×50:
	int gridCount = 31;  // Change this to 21 if you want a 21×21 grid, or set it dynamically.

	// Compute the grid cell size. We assume a square window, so both width and height are the same.
	float gridSize = static_cast<float>(_game->window().getSize().x) / gridCount;

	// Get the snake sprite's original size (its local bounds)
	sf::FloatRect bounds = sprite.getLocalBounds();

	// Calculate the scale factor so that the sprite's width becomes equal to gridSize.
	// (If your snake image isn't square, you can compute separate factors for x and y.)
	float scaleFactor = gridSize / bounds.width;

	// Apply the scaling factor to the sprite.
	sprite.setScale(scaleFactor, scaleFactor);

	// ---- End of New Scaling Code ----

	// Recalculate the snake's bounding box using the scaled sprite's global bounds.
	sf::FloatRect scaledBounds = sprite.getGlobalBounds();
	_player->addComponent<CBoundingBox>(sf::Vector2f(scaledBounds.width, scaledBounds.height));

	// Set the player's state to "Alive"
	_player->addComponent<CState>().state = "Alive";
}

// Handle the player's movement based on input and update the position accordingly.
void Scene_Snake::playerMovement(sf::Time dt)
{
	// Compute grid cell size based on the window width (21 columns)
	float gridSize = static_cast<float>(_game->window().getSize().x) / 31.f;

	// Initialize movement vector
	sf::Vector2f movementDelta(0.f, 0.f);
	auto& pos = _player->getComponent<CTransform>().pos;  // Get player's current position

	// Check input directions (assuming your input system uses bit flags:
	// 1 for UP, 2 for DOWN, 4 for LEFT, 8 for RIGHT)
	if (_player->getComponent<CInput>().dir == 1)
	{ // UP
		movementDelta.y -= gridSize;
		_player->getComponent<CInput>().dir = 0;  // Reset input flag
		SoundPlayer::getInstance().play("hop", pos);
	}
	if (_player->getComponent<CInput>().dir == 2)
	{ // DOWN
		movementDelta.y += gridSize;
		_player->getComponent<CInput>().dir = 0;
		SoundPlayer::getInstance().play("hop", pos);
	}
	if (_player->getComponent<CInput>().dir == 4)
	{ // LEFT
		movementDelta.x -= gridSize;
		_player->getComponent<CInput>().dir = 0;
		SoundPlayer::getInstance().play("hop", pos);
	}
	if (_player->getComponent<CInput>().dir == 8)
	{ // RIGHT
		movementDelta.x += gridSize;
		_player->getComponent<CInput>().dir = 0;
		SoundPlayer::getInstance().play("hop", pos);
	}

	// If movement occurred, update the player's position by exactly one grid cell.
	if (movementDelta != sf::Vector2f(0.f, 0.f))
	{
		pos += movementDelta;
		std::cout << "Snake moved to (" << pos.x << ", " << pos.y << ")\n";
	}
}

// Adjust the player's position to ensure it stays within the window boundaries.
void Scene_Snake::adjustPlayerPosition()
{
	auto& player_pos = _player->getComponent<CTransform>().pos;    // Reference to player's position.
	auto& playerSize = _player->getComponent<CBoundingBox>().size;   // Reference to player's size.
	auto& playerHalfSize = _player->getComponent<CBoundingBox>().halfSize;  // Reference to half the player's size.

	// Clamp the x-coordinate: ensure it is not less than half the width.
	if (player_pos.x < playerHalfSize.x)
	{
		player_pos.x = playerHalfSize.x;
	}

	// Clamp the x-coordinate: ensure it does not exceed the window width minus half the player's width.
	if (player_pos.x > (_game->windowSize().x - playerHalfSize.x))
	{
		player_pos.x = _game->windowSize().x - playerHalfSize.x;
	}

	// Clamp the y-coordinate: ensure it is not less than half the height.
	if (player_pos.y < playerHalfSize.y)
	{
		player_pos.y = playerHalfSize.y;
	}

	// Clamp the y-coordinate: ensure it does not exceed the window height minus half the player's height.
	if (player_pos.y > _game->windowSize().y - playerHalfSize.y)
	{
		player_pos.y = _game->windowSize().y - playerHalfSize.y;
	}
}

// Load level configuration from a file specified by 'path'.
void Scene_Snake::loadLevel(const std::string& path)
{
	std::ifstream config(path);  // Open the level configuration file.
	if (config.fail())
	{
		std::cerr << "Open file " << path << " failed\n";
		config.close();
		exit(1);  // Exit if the file cannot be opened.
	}

	std::string token{ "" };
	config >> token;  // Read the first token from the file.
	while (!config.eof())
	{  // Continue until the end of the file.
		if (token == "Bkg")
		{  // If the token is "Bkg", it defines a background entity.
			std::string name;
			sf::Vector2f pos;
			config >> name >> pos.x >> pos.y;  // Read the texture name and position.
			auto e = _entityManager.addEntity("bkg");  // Add a background entity.

			// Add a CSprite component using the texture from Assets.
			auto& sprite = e->addComponent<CSprite>(Assets::getInstance().getTexture(name)).sprite;
			sprite.setOrigin(0.f, 0.f);   // Set the sprite's origin to the top-left.
			sprite.setPosition(pos);      // Position the sprite as specified.

			// Scale the background sprite to fill the window.
			sf::Vector2u windowSize = _game->window().getSize();
			sf::Vector2u textureSize = Assets::getInstance().getTexture(name).getSize();
			float scaleX = static_cast<float>(windowSize.x) / textureSize.x;
			float scaleY = static_cast<float>(windowSize.y) / textureSize.y;
			sprite.setScale(scaleX, scaleY);
		}
		else if (token == "World")
		{  // If the token is "World", it defines the game world's dimensions.
			config >> _worldBounds.width >> _worldBounds.height;
		}
		config >> token;  // Read the next token.
	}

	config.close();  // Close the level configuration file.
}

// Update the scene's systems: movement, collision, animations, etc.
void Scene_Snake::sMovement(sf::Time dt)
{
	playerMovement(dt);  // Update the player's movement based on input.

	// For each entity, update its position and rotation based on its velocity and angular velocity.
	for (auto& e : _entityManager.getEntities())
	{
		auto& tfm = e->getComponent<CTransform>();
		/*tfm.prevPos = tfm.pos;*/  // Optionally, store the previous position (commented out).
		tfm.pos += tfm.vel * dt.asSeconds();  // Update position using velocity.
		tfm.angle += tfm.angVel * dt.asSeconds();  // Update angle using angular velocity.
	}
}

// Process collisions between entities (currently not implemented).
void Scene_Snake::sCollisions()
{
	// Collision detection and response logic would be implemented here.
}

// Overall update function that runs systems if the scene is not paused.
void Scene_Snake::sUpdate(sf::Time dt)
{
	if (_isPaused)
		return;  // Do nothing if the scene is paused.
	SoundPlayer::getInstance().removeStoppedSounds();  // Clean up finished sound effects.
	_entityManager.update();  // Update the entity manager (remove dead entities, etc.).

	sMovement(dt);   // Process movement updates.
	sCollisions();   // Process collision detection (if implemented).
	adjustPlayerPosition();  // Ensure the player stays within window bounds.
}

// Called when the scene ends; switches back to the menu.
void Scene_Snake::onEnd()
{
	_game->changeScene("MENU", nullptr, false);  // Change the current scene to the menu scene.
}



// Draw the player's score on the screen.
void Scene_Snake::drawScore()
{
	int totalScore = 0;
	// Sum up the score contributions from different height segments.
	for (int i = 0; i < 11; ++i)
	{
		totalScore += _scoredHeights[i];
	}
	_score = totalScore;

	// Create a string from the total score and prepare a text object.
	std::string str = std::to_string(_scoreTotal + _score);
	sf::Text text = sf::Text("SCORE: " + str, Assets::getInstance().getFont("Arial"), 15);

	text.setPosition(10.f, 10.f);  // Position the score text in the top-left corner.
	_game->window().draw(text);   // Draw the score text.
}

// Update the score based on the player's vertical position and animation state.
void Scene_Snake::getScore()
{
	auto pos = _player->getComponent<CTransform>().pos.y;  // Get the player's y-coordinate.
	int posY = static_cast<int>(pos);
	
}

// Render the "Game Over" screen with the message and instructions.
void Scene_Snake::drawGameOver()
{

	std::string str = "GAME OVER";
	sf::Text text = sf::Text(str, Assets::getInstance().getFont("Arial"), 60);
	centerOrigin(text);                // Center the text.
	text.setPosition(240.f, 300.f);      // Position the text on screen.
	_game->window().draw(text);         // Draw the "Game Over" text.

	std::string strEsc = "Press ESC";
	sf::Text textEsc = sf::Text(strEsc, Assets::getInstance().getFont("Arial"), 40);
	centerOrigin(textEsc);             // Center the instruction text.
	textEsc.setPosition(240.f, 340.f);   // Position below the main text.
	_game->window().draw(textEsc);      // Draw the instruction text.
}

