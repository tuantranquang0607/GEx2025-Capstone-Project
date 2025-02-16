#pragma once // Ensure the header file is included only once during compilation

#include "Components.h" // Include the Components header
#include "Scene.h"      // Include the base Scene class
#include <queue>        // Include the queue container from the standard library

// Structure to represent a spawn point for enemies
struct SpawnPoint {
    std::string type;           // Type of enemy

    float y;                    // Y-coordinate for spawning

    auto operator<=>(const SpawnPoint& other) const 
    {
        return y <=> other.y;   // Compare spawn points based on Y-coordinate
    }
};

// Structure to hold level configuration settings
struct LevelConfig {
    float scrollSpeed{ 100.f };                 // Scrolling speed of the level
    float playerSpeed{ 200.f };                 // Speed of the player
    float enemySpeed{ 200.f };                  // Speed of the enemies
    float bulletSpeed{ 400.f };                 // Speed of the bullets
    float missileSpeed{ 150.f };                // Speed of the missiles
    sf::Time fireInterval{ sf::seconds(5) };    // Interval between firing bullets
};

// Class to represent the planes scene in the game
class Scene_Snake : public Scene
{
    sPtrEntt _player{ nullptr };                    // Shared pointer to the player entity
    sf::View _worldView;                            // Camera view
    sf::FloatRect _worldBounds;                     // Bounds of the game world
    LevelConfig _config;                            // Level configuration settings
    std::priority_queue<SpawnPoint> _spawnPoints;   // Priority queue of spawn points
    bool _drawTextures{ true };                     // Flag to indicate if textures should be drawn
    bool _drawAABB{ false };                        // Flag to indicate if axis-aligned bounding boxes should be drawn
    bool _drawCam{ false };                         // Flag to indicate if the camera view should be drawn

    // Systems
    void sMovement(sf::Time dt);        // Handle movement of entities
    void sSpawnEnemies();               // Handle spawning of enemies
    void sCollisions();                 // Handle collisions between entities
    void sUpdate(sf::Time dt);          // Update the scene
    void sGunUpdate(sf::Time dt);       // Update the gun system
    void sAutoPilot(sf::Time dt);       // Handle autopilot for enemies
    void sGuideMissiles(sf::Time dt);   // Guide missiles towards targets
    void sAnimation(sf::Time dt);       // Update animations

    // Helper functions
    bool isGameOver();                                      // Check if the game is over

    void dropPickup(sf::Vector2f pos);                      // Drop a pickup at a position
    void startAnimation(sPtrEntt e, std::string animation); // Start an animation for an entity
    void checkIfDead(sPtrEntt e);                           // Check if an entity is dead
    void checkMissileCollision();                           // Check for missile collisions
    void checkBulletCollision();                            // Check for bullet collisions
    void checkPlaneCollision();                             // Check for plane collisions
    void checkPickupCollision();                            // Check for pickup collisions

    /*sf::Vector2f findClosestEnemy(sf::Vector2f mPos);*/       // Find the closest enemy to a position
    /*sf::FloatRect getBattlefieldBounds() const;*/             // Get the bounds of the battlefield

    void destroyOutsideBattlefieldBounds();                 // Destroy entities outside the battlefield bounds
    void spawnEnemyPlanes(SpawnPoint sp);                   // Spawn enemy planes at a spawn point
    void fireBullet();                                      // Fire a bullet
    void spawnBullet(sf::Vector2f pos, bool isEnemy);       // Spawn a bullet at a position
    void registerActions();                                 // Register actions for the scene
    void spawnPlayer(sf::Vector2f pos);                     // Spawn the player at a position
    void playerMovement();                                  // Handle player movement
    void annimatePlayer();                                  // Animate the player
    void adjustPlayerPosition();                            // Adjust the player's position
    void init(const std::string& path);                     // Initialize the scene with a level path
    void loadLevel(const std::string& path);                // Load a level from a file
    void spawnMisille();                                    // Spawn a missile
    void onEnd() override;                                  // Handle the end of the scene
    void drawAABB(std::shared_ptr<Entity> e);               // Draw the axis-aligned bounding box of an entity
    void drawCameraView();                                  // Draw the camera view
    void drawHP(sPtrEntt e);                                // Draw the health points of an entity
    void drawAmmo(sPtrEntt e);                              // Draw the ammo count of an entity
    void drawEntt(sPtrEntt e);                              // Draw an entity

public:
    // Constructor to initialize the planes scene with a game engine and level path
    Scene_Snake(GameEngine* gameEngine, const std::string& levelPath);

    void update(sf::Time dt) override;                  // Update the scene
    void sDoAction(const Command& command) override;    // Handle actions in the scene
    void sRender() override;                            // Render the scene
};
