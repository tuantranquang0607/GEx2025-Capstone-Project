#include "Scene.h" // Include the Scene header

// Constructor to initialize the scene with a game engine
Scene::Scene(GameEngine* gameEngine) : _game(gameEngine)
{}

// Virtual destructor
Scene::~Scene()
{}

// Function to set the paused state of the scene
void Scene::setPaused(bool paused)
{
    _isPaused = paused; // Set the paused flag
}

// Function to simulate the scene (currently empty)
void Scene::simulate(int)
{}

// Function to perform an action in the scene
void Scene::doAction(Command command)
{
    this->sDoAction(command); // Call the pure virtual function sDoAction
}

// Function to get the action map
const CommandMap Scene::getActionMap() const
{
    return _commands; // Return the map of commands
}

// Function to register an action with a key
void Scene::registerAction(int inputKey, std::string command)
{
    _commands[inputKey] = command; // Add the command to the map with the input key
}
