#include "Scene_Snake.h"
#include "Scene_Menu.h"
#include <SFML/Graphics.hpp>

Scene_Snake::Scene_Snake(GameEngine* gameEngine, const std::string& levelPath)
    : Scene(gameEngine)
{
    // Initialize the Snake game here.
    // This is where we could load levels, initialize game objects, etc.
}

void Scene_Snake::onEnd()
{
    // This method is called when the game ends.
    // You could return to the menu or perform other actions.
    _game->changeScene("MENU", std::make_shared<Scene_Menu>(_game));
}

void Scene_Snake::update(sf::Time dt)
{
    // Update game objects (like snake movement, collision checks, etc.)
    _entityManager.update();  // Update entities
}

void Scene_Snake::sDoAction(const Command& action)
{
    // Handle input actions for the Snake game, like moving the snake, pausing, etc.
    if (action.name() == "UP")
    {
        // Move the snake up
    }
    else if (action.name() == "DOWN")
    {
        // Move the snake down
    }
    // Handle other actions (left, right, pause, quit)
}

void Scene_Snake::sRender()
{
    // Render the game window
    _game->window().clear(sf::Color::Black);

    // Draw snake, food, and other elements
    // Example: _game->window().draw(snake);

    _game->window().display();
}

void Scene_Snake::launchSnakeGameWindow()
{
    // Create a new window specifically for the Snake game
    sf::RenderWindow snakeWindow(sf::VideoMode(800, 800), "Snake Game");

    while (snakeWindow.isOpen())
    {
        sf::Event event;
        while (snakeWindow.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                snakeWindow.close();
        }

        // Here, you would handle updating and rendering for the snake game.
        // For example:
        update(sf::Time::Zero); // Or pass actual deltaTime for better control
        sRender();

        snakeWindow.display();
    }
}
