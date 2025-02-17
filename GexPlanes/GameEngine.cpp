#include "GameEngine.h"     // Include the GameEngine header
#include "Assets.h"         // Include the Assets header
#include "Scene_Menu.h"     // Include the Scene_Menu header
#include "Scene_Snake.h"   // Include the Scene_Snake header
#include "Command.h"        // Include the Command header
#include <fstream>          // Include the fstream library for file operations
#include <memory>           // Include the memory library for smart pointers
#include <cstdlib>          // Include the cstdlib library for standard library functions
#include <iostream>         // Include the iostream library for input and output

// Constructor to initialize the game engine with a configuration file
GameEngine::GameEngine(const std::string& path)
{
    Assets::getInstance().loadFromFile("../config.txt");    // Load assets from the configuration file
    init(path);                                             // Initialize the game engine with the provided path
}

// Initialize the game engine with a configuration file
void GameEngine::init(const std::string& path)
{
    unsigned int width;
    unsigned int height;

    loadConfigFromFile(path, width, height);                        // Load the window configuration from the file

    _window.create(sf::VideoMode(width, height), "Not Mario");      // Create the main window

    _statisticsText.setFont(Assets::getInstance().getFont("main")); // Set the font for the statistics text
    _statisticsText.setPosition(15.0f, 5.0f);                       // Set the position of the statistics text
    _statisticsText.setCharacterSize(15);                           // Set the character size of the statistics text

    changeScene("MENU", std::make_shared<Scene_Menu>(this));        // Change to the menu scene
}

// Load configuration from a file
void GameEngine::loadConfigFromFile(const std::string &path, unsigned int &width, unsigned int &height) const 
{
    std::ifstream config(path);
    if (config.fail()) {
        std::cerr << "Open file " << path << " failed\n";
        config.close();
        exit(1);
    }
    std::string token{""};
    config >> token;
    while (!config.eof()) {
        if (token == "Window") {
            config >> width >> height;
        } else if (token[0] == '#') {
            std::string tmp;
            std::getline(config, tmp);
            std::cout << tmp << "\n";
        }

        if (config.fail()) {
            config.clear(); // clear error on stream
            std::cout << "*** Error reading config file\n";
        }
        config >> token;
    }
    config.close();
}

// Update the game engine
void GameEngine::update()
{
    sUserInput();
    currentScene()->update(sf::seconds(1.0f / 60.f));
}

// Handle user input
void GameEngine::sUserInput()
{
    sf::Event event;
    while (_window.pollEvent(event))
    {
        if (event.type == sf::Event::Closed)  
            quit();  

        if (event.type == sf::Event::KeyPressed || event.type == sf::Event::KeyReleased)
        {
            if (currentScene()->getActionMap().contains(event.key.code))
            {
                const std::string actionType = (event.type == sf::Event::KeyPressed) ? "START" : "END";
                currentScene()->doAction(Command(currentScene()->getActionMap().at(event.key.code), actionType));
            }
        }
    }
}

// Get the current scene
std::shared_ptr<Scene> GameEngine::currentScene()
{
    return _sceneMap.at(_currentScene);
}

// Change the current scene
void GameEngine::changeScene(const std::string& sceneName, std::shared_ptr<Scene> scene, bool endCurrentScene)
{
    if (endCurrentScene)
        _sceneMap.erase(_currentScene);

    if (!_sceneMap.contains(sceneName))
        _sceneMap[sceneName] = scene;

    _currentScene = sceneName;
}

// Quit the game
void GameEngine::quit()
{
    _window.close();
}

// Run the game loop
void GameEngine::run()
{
    const sf::Time SPF = sf::seconds(1.0f / 60.f);  // seconds per frame for 60 fps 

    sf::Clock clock;
    sf::Time timeSinceLastUpdate = sf::Time::Zero;

    while (isRunning())
    {
        sUserInput(); // get user input

        timeSinceLastUpdate += clock.restart();		 
        while (timeSinceLastUpdate > SPF)
        {
            currentScene()->update(SPF); // update world
            timeSinceLastUpdate -= SPF;
        }

        window().clear(sf::Color::Cyan);
        currentScene()->sRender(); // render world
        window().display();
    }
}

// Quit the current level
void GameEngine::quitLevel()
{
    changeScene("MENU", nullptr, true);
}

// Go back to the previous level
void GameEngine::backLevel()
{
    changeScene("MENU", nullptr, false);
}

// Get the main window
sf::RenderWindow& GameEngine::window()
{
    return _window;
}

// Get the size of the window
sf::Vector2f GameEngine::windowSize() const {
    return sf::Vector2f{_window.getSize()};
}

// Check if the game is running
bool GameEngine::isRunning()
{
    return (_running && _window.isOpen());
}
