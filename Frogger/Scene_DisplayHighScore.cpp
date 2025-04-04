#include "Scene_DisplayHighScore.h"
#include "Scene_Menu.h"
#include <fstream>
#include <sstream>
#include <algorithm>
#include <iostream>
#include "Utilities.h"

Scene_DisplayHighScore::Scene_DisplayHighScore(GameEngine* game)
    : Scene(game)
{
    init();
}

void Scene_DisplayHighScore::init()
{
    registerAction(sf::Keyboard::Escape, "BACK");

    _titleText.setFont(Assets::getInstance().getFont("main"));
    _titleText.setString("HIGH SCORES");
    _titleText.setCharacterSize(60);
    centerOrigin(_titleText);
    _titleText.setPosition(_game->windowSize().x / 2.f, 40);

    _footerText.setFont(Assets::getInstance().getFont("main"));
    _footerText.setCharacterSize(30);
    _footerText.setString("Press ESC to return");
    centerOrigin(_footerText);
    _footerText.setPosition(_game->windowSize().x / 2.f, _game->windowSize().y - 50);

    loadScoresFromFile();
}

void Scene_DisplayHighScore::loadScoresFromFile()
{
    std::ifstream file("../assets/highscores.txt");
    std::string name;
    int score;

    while (file >> name >> score)
    {
        _scores.emplace_back(name, score);
    }

    std::sort(_scores.begin(), _scores.end(),
        [](const auto& a, const auto& b)
        {
            return a.second > b.second; // descending
        });

    _scoreTexts.clear();
    float yStart = 120.f;

    for (size_t i = 0; i < _scores.size(); ++i)
    {
        std::stringstream ss;
        ss << i + 1 << ". " << _scores[i].first << " - " << _scores[i].second;

        sf::Text scoreText;
        scoreText.setFont(Assets::getInstance().getFont("main"));
        scoreText.setString(ss.str());
        scoreText.setCharacterSize(30);
        centerOrigin(scoreText);
        scoreText.setPosition(_game->windowSize().x / 2.f, yStart + i * 40.f);

        _scoreTexts.push_back(scoreText);
    }
}

void Scene_DisplayHighScore::update(sf::Time dt)
{
    _entityManager.update();
}

void Scene_DisplayHighScore::sRender()
{
    _game->window().clear(sf::Color(50, 50, 80));

    _game->window().draw(_titleText);
    for (const auto& text : _scoreTexts)
    {
        _game->window().draw(text);
    }
    _game->window().draw(_footerText);
}

void Scene_DisplayHighScore::sDoAction(const Command& action)
{
    if (action.type() == "START")
    {
        if (action.name() == "BACK")
        {
            _game->changeScene("MENU", std::make_shared<Scene_Menu>(_game), true);
        }
    }
}

void Scene_DisplayHighScore::onEnd()
{
    // Nothing to clean up yet
}

