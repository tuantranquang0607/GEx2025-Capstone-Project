#pragma once // Ensure the header file is included only once during compilation

#include "Scene.h" // Include the base Scene class

// Class to represent the menu scene in the game
class Scene_Menu : public Scene
{
private:
    std::vector<std::string>	m_menuStrings;  // Vector to store menu options
    sf::Text					m_menuText;     // SFML text object for rendering menu text
    std::vector<std::string>	m_levelPaths;   // Vector to store paths to level files
    int							m_menuIndex{0}; // Index of the currently selected menu option
    std::string					m_title;        // Title of the menu
    sf::Sprite m_backgroundSprite;              // Background sprite

    // Initialize the menu scene
    void init();
    // Handle the end of the menu scene
    void onEnd() override;

public:
    // Constructor to initialize the menu scene with a game engine
    Scene_Menu(GameEngine* gameEngine);

    // Update the menu scene
    void update(sf::Time dt) override;

    // Render the menu scene
    void sRender() override;

    // Handle actions in the menu scene
    void sDoAction(const Command& action) override;
};
