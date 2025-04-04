#include "Scene_GameOver.h"
#include "Scene_Menu.h"
#include <fstream>
#include <algorithm>
#include <iostream>
#include "Utilities.h"

Scene_GameOver::Scene_GameOver(GameEngine* game, int finalScore)
    : Scene(game), _finalScore(finalScore)
{
    init();
}

void Scene_GameOver::init()
{
    registerAction(sf::Keyboard::Return, "ENTER");
    registerAction(sf::Keyboard::BackSpace, "BACKSPACE");
    registerAction(sf::Keyboard::Escape, "ESC");

    for (char c = 'A'; c <= 'Z'; ++c)
        registerAction(static_cast<sf::Keyboard::Key>(c - 'A' + sf::Keyboard::A), std::string(1, c));

    _gameOverText.setFont(Assets::getInstance().getFont("main"));
    _gameOverText.setString("GAME OVER");
    _gameOverText.setCharacterSize(60);
    _gameOverText.setFillColor(sf::Color::Red);
    centerOrigin(_gameOverText);
    _gameOverText.setPosition(_game->windowSize().x / 2.f, 100);

    _scoreText.setFont(Assets::getInstance().getFont("main"));
    _scoreText.setString("Your Score: " + std::to_string(_finalScore));
    _scoreText.setCharacterSize(40);
    centerOrigin(_scoreText);
    _scoreText.setPosition(_game->windowSize().x / 2.f, 180);

    _inputLabel.setFont(Assets::getInstance().getFont("main"));
    _inputLabel.setString("Type your name and press ENTER to save your score,\n"
        "or just press ENTER or ESC to return to the main menu.");
    _inputLabel.setCharacterSize(30);
    centerOrigin(_inputLabel);
    _inputLabel.setPosition(_game->windowSize().x / 2.f, 260);

    _nameInput.setFont(Assets::getInstance().getFont("main"));
    _nameInput.setCharacterSize(40);
    _nameInput.setFillColor(sf::Color::White);
    _nameInput.setPosition(_game->windowSize().x / 2.f - 125, 300);
}

void Scene_GameOver::update(sf::Time dt)
{
    _entityManager.update();
}

void Scene_GameOver::sRender()
{
    _game->window().clear(sf::Color::Black);
    _game->window().draw(_gameOverText);
    _game->window().draw(_scoreText);
    _game->window().draw(_inputLabel);
    _nameInput.setString(_playerName + "_");
    _game->window().draw(_nameInput);
}

void Scene_GameOver::sDoAction(const Command& action)
{
    if (_submitted)
        return;

    if (action.type() == "START")
    {
        if (action.name() == "ENTER")
        {
            if (!_playerName.empty())
            {
                saveScoreToFile();
            }
            _submitted = true;
            _game->changeScene("MENU", std::make_shared<Scene_Menu>(_game), true);
        }
        else if (action.name() == "BACKSPACE")
        {
            if (!_playerName.empty())
                _playerName.pop_back();
        }
        else if (action.name() == "ESC")
        {
            _submitted = true;
            _game->changeScene("MENU", std::make_shared<Scene_Menu>(_game), true);
        }
        else if (action.name().size() == 1 && _playerName.size() < 10)
        {
            _playerName += action.name();
        }
    }
}

void Scene_GameOver::saveScoreToFile()
{
    std::ofstream file("../assets/highscores.txt", std::ios::app);
    if (file.is_open())
    {
        file << _playerName << " " << _finalScore << "\n";
        file.close();
    }
    else
    {
        std::cerr << "Failed to open highscores.txt for writing\n";
    }
}

void Scene_GameOver::onEnd()
{
    // Nothing special for now
}

