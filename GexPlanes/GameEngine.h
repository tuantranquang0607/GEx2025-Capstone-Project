#pragma once // Ensure the header file is included only once during compilation

#include "Assets.h" // Include the Assets header

#include <memory>   // Include the memory library for smart pointers
#include <map>      // Include the map container from the standard library

// Forward declaration of the Scene class
class Scene;

// Type alias for a map of scenes
using SceneMap = std::map<std::string, std::shared_ptr<Scene>>;

// Class to represent the game engine
class GameEngine
{
public:
    sf::RenderWindow	        _window;                                    // The main window for rendering
    std::string			        _currentScene;                              // The name of the current scene
    SceneMap			        _sceneMap;                                  // Map of scenes
    size_t				        _simulationSpeed{ 1 };                      // Simulation speed
    bool				        _running{ true };                           // Flag to indicate if the game is running

    // Stats
    sf::Text					_statisticsText;                            // Text for displaying statistics
    sf::Time					_statisticsUpdateTime{ sf::Time::Zero };    // Time for updating statistics
    unsigned int				_statisticsNumFrames{ 0 };                  // Number of frames for statistics

public:
    // Initialize the game engine with a configuration file
    void					init(const std::string& path);

    // Update the game engine
    void					update();

    // Handle user input
    void					sUserInput();

    // Get the current scene
    std::shared_ptr<Scene>	currentScene();

public:
    // Constructor to initialize the game engine with a configuration file
    GameEngine(const std::string& path);

    // Change the current scene
    void				changeScene(const std::string& sceneName, std::shared_ptr<Scene> scene, bool endCurrentScene = false);
    // Quit the game
    void				quit();
    // Run the game loop
    void				run();
    // Quit the current level
    void				quitLevel();
    // Go back to the previous level
    void				backLevel();

    // Get the main window
    sf::RenderWindow&	window();
    // Get the size of the window
    sf::Vector2f		windowSize() const;

    // Check if the game is running
    bool				isRunning();

    // Load configuration from a file
    void				loadConfigFromFile(const std::string& path, unsigned int& width, unsigned int& height) const;
};
