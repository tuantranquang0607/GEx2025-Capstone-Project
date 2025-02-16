#pragma once // Ensure the header file is included only once during compilation

#include "EntityManager.h"  // Include the EntityManager header
#include "GameEngine.h"     // Include the GameEngine header
#include "Command.h"        // Include the Command header
#include <map>              // Include the map container from the standard library
#include <string>           // Include the string library

using CommandMap = std::map<int, std::string>; // Type alias for a map of commands

class Scene_Breakout;   // Forward declaration of Scene_Breakout class

// Base class to represent a scene in the game
class Scene
{
protected:
    GameEngine	    *_game;             // Pointer to the game engine
    EntityManager	_entityManager;     // Entity manager for the scene
    CommandMap		_commands;          // Map of commands
    bool			_isPaused{false};   // Flag to indicate if the scene is paused
    bool			_hasEnded{false};   // Flag to indicate if the scene has ended
    size_t			_currentFrame{ 0 }; // Current frame number

    // Pure virtual function to handle the end of the scene
    virtual void	onEnd() = 0;
    // Function to set the paused state of the scene
    void			setPaused(bool paused);

public:
    // Constructor to initialize the scene with a game engine
    Scene(GameEngine* gameEngine);
    // Virtual destructor
    virtual ~Scene();

    // Pure virtual function to update the scene
    virtual void		update(sf::Time dt) = 0;
    // Pure virtual function to handle actions in the scene
    virtual void		sDoAction(const Command& action) = 0;
    // Pure virtual function to render the scene
    virtual void		sRender() = 0;

    // Function to simulate the scene
    void				simulate(int);
    // Function to perform an action in the scene
    void				doAction(Command);
    // Function to register an action with a key
    void				registerAction(int, std::string);
    // Function to get the action map
    const CommandMap	getActionMap() const;
};
