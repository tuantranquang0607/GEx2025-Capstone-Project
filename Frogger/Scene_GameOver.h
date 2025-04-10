#pragma once

#include "Scene.h"
#include <string>

class Scene_GameOver : public Scene
{
private:
    sf::Text _gameOverText;
    sf::Text _scoreText;
    sf::Text _inputLabel;
    sf::Text _nameInput;
    sf::Sprite _backgroundSprite;

    std::string _playerName;

    int _finalScore;
    bool _submitted = false;

    void init();
    void saveScoreToFile();
    void onEnd() override;

public:
    Scene_GameOver(GameEngine* game, int finalScore);

    void update(sf::Time dt) override;
    void sRender() override;
    void sDoAction(const Command& action) override;
};

