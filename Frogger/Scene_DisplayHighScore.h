#pragma once

#include "Scene.h"
#include <vector>
#include <string>
#include <utility>

class Scene_DisplayHighScore : public Scene
{
private:
    std::vector<std::pair<std::string, int>> _scores;
    sf::Text _titleText;
    std::vector<sf::Text> _scoreTexts;
    sf::Text _footerText;
    sf::Sprite					_backgroundSprite;

    void init();
    void loadScoresFromFile();
    void onEnd() override;

public:
    Scene_DisplayHighScore(GameEngine* game);

    void update(sf::Time dt) override;
    void sRender() override;
    void sDoAction(const Command& action) override;
};


