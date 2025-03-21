//
// Created by David Burchill on 2023-09-27.
//

#pragma once

// Include SFML modules for timing and graphics functionality
#include <SFML/System/Time.hpp>                         // For sf::Time, handling delta time
#include <SFML/Graphics.hpp>                            // For SFML graphics (sprites, shapes, views, etc.)

// Include custom project headers for entity and scene management
#include "EntityManager.h"                              // Manages all game entities (creation, update, deletion)
#include "Entity.h"                                     // Defines an individual game entity and its components
#include "Scene.h"                                      // Base class for all game scenes (e.g., menu, gameplay)
#include "GameEngine.h"                                 // Core game engine class that manages the window and scene switching

// Define the Scene_Snake class, which represents the gameplay scene for the Snake game.
// It inherits from the base Scene class.
class Scene_Snake : public Scene
{
    sPtrEntt _player{ nullptr };                        // Pointer to the player entity. "sPtrEntt" is an alias for a shared pointer to an Entity.

    sf::FloatRect _worldBounds;                         // Rectangle defining the boundaries of the game world (e.g., level dimensions)

    // Game state variables:
    int _lives{ 1 };                                    // The number of lives the player has (default initialized to 1)
    int _score{ 0 };                                    // The player's current score
    int _scoreTotal{ 0 };                               // The cumulative score, which may include bonus or previous levels
    int _scoredHeights[11] = {};                        // An array storing score contributions based on the player's vertical position

    // Flags to track various states and debug options:
    bool _isFinish = false;                             // Flag indicating whether the level has been finished
    bool _isComplete = false;                           // Flag indicating if level objectives are complete
    bool _drawTextures{ true };                         // Toggle for drawing textures (useful for debugging)
    bool _drawAABB{ false };                            // Toggle for drawing axis-aligned bounding boxes (collision debugging)
    bool _drawCam{ false };                             // Toggle for drawing camera outlines or debugging camera view

    // --- Systems: Methods to update various game systems ---
    void sMovement(sf::Time dt);                        // Updates movement for entities based on the elapsed time (dt)
    void sCollisions();                                 // Processes collision detection and response between entities
    void sUpdate(sf::Time dt);                          // Updates the overall scene (combining movement, collisions, etc.)
    void onEnd() override;                              // Called when the scene is ending; overrides the base Scene onEnd()
    void drawScore();                                   // Renders the current score on the screen
    void getScore();                                    // Calculates or updates the score based on game events
    void drawGameOver();                                // Renders the "Game Over" screen when the player loses

    // --- Helper Functions: Utility functions for scene initialization and management ---
    void init(const std::string& path);                 // Initializes the scene (loads level data, sets up entities) from a file path
    void loadLevel(const std::string& path);            // Loads level configuration and entity placements from a file
    void spawnPlayer(sf::Vector2f pos);                 // Creates and positions the player entity at the given position
    void playerMovement(sf::Time dt);                   // Handles player-specific movement logic based on user input and time (dt)
    void adjustPlayerPosition();                        // Ensures the player stays within defined game boundaries
    void registerActions();                             // Sets up input actions (keyboard mappings) for this scene
	void spawnWalls();                                  // Spawns a wall entity in the game world
	void spawnApple();                                  // Spawns an apple entity in the game world
	void spawnOrange();                                 // Spawns an orange entity in the game world
	void spawnBlueberry();                              // Spawns a blueberry entity in the game world
	void checkWallCollision();                          // Checks for collisions between the player and walls
	void checkAppleCollision();                         // Checks for collisions between the player and apples
	void checkOrangeCollision();                        // Checks for collisions between the player and oranges
	void checkBlueberryCollision();                     // Checks for collisions between the player and blueberries

    int gridCount;
    float gridSize;

    bool isCellOccupied(int cellX, int cellY, float cellSize);


public:
    // Constructor: Initializes the Scene_Snake instance with a pointer to the GameEngine and the level file path.
    Scene_Snake(GameEngine* gameEngine, const std::string& levelPath);

    // Override the base Scene's virtual functions to provide specific behavior for the snake game:
    void update(sf::Time dt) override;                  // Updates the scene each frame using the elapsed time (dt)
    void sDoAction(const Command& action) override;     // Processes a user input command (e.g., key press/release)
    void sRender() override;                            // Renders all elements of the scene to the game window
};

