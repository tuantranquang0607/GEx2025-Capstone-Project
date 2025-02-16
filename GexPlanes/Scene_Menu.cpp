#include "Scene_Menu.h" // Include the Scene_Menu header
#include "Scene_Snake.h" // Include the Scene_Snake header
#include "MusicPlayer.h" // Include the MusicPlayer header
#include <memory> // Include the memory library for smart pointers

// Handle the end of the menu scene
void Scene_Menu::onEnd()
{
    _game->window().close(); // Close the game window
}

// Constructor to initialize the menu scene with a game engine
Scene_Menu::Scene_Menu(GameEngine* gameEngine) : Scene(gameEngine) // Initialize the base Scene class with the game engine
{
    init(); // Initialize the menu scene
}

// Initialize the menu scene
void Scene_Menu::init()
{
    MusicPlayer::getInstance().play("menuTheme");   // Play the menu theme music
    MusicPlayer::getInstance().setVolume(25);        // Set the volume of the music

    // Register actions for the menu
    registerAction(sf::Keyboard::W, "UP");
    registerAction(sf::Keyboard::Up, "UP");
    registerAction(sf::Keyboard::S, "DOWN");
    registerAction(sf::Keyboard::Down, "DOWN");
    registerAction(sf::Keyboard::D, "PLAY");
    registerAction(sf::Keyboard::Escape, "QUIT");

    m_title = "Snappy Snake"; // Set the title of the menu
    m_menuStrings.push_back("Level 1"); // Add menu options
    m_menuStrings.push_back("Level 2");
    m_menuStrings.push_back("Level 3");

    m_levelPaths.push_back("../level1.txt"); // Add level paths
    m_levelPaths.push_back("../level1.txt");
    m_levelPaths.push_back("../level1.txt");

    m_menuText.setFont(Assets::getInstance().getFont("main")); // Set the font for the menu text

    const size_t CHAR_SIZE{ 64 };
    m_menuText.setCharacterSize(CHAR_SIZE); // Set the character size of the menu text

    // Initialize the background sprite
    m_backgroundSprite.setTexture(Assets::getInstance().getTexture("logo"));
    sf::Vector2u textureSize = m_backgroundSprite.getTexture()->getSize();
    sf::Vector2u windowSize = _game->window().getSize();
    m_backgroundSprite.setScale(
        static_cast<float>(windowSize.x) / textureSize.x,
        static_cast<float>(windowSize.y) / textureSize.y
    );
}

// Update the menu scene
void Scene_Menu::update(sf::Time dt)
{
    _entityManager.update(); // Update the entity manager
}

// Render the menu scene
void Scene_Menu::sRender()
{
    sf::View view = _game->window().getView();
    view.setCenter(_game->window().getSize().x / 2.f, _game->window().getSize().y / 2.f);
    _game->window().setView(view);

    // Draw the background sprite
    _game->window().draw(m_backgroundSprite);

    static const sf::Color selectedColor(255, 255, 255);
    static const sf::Color normalColor(0, 0, 0);

    static const sf::Color backgroundColor(100, 100, 255);

    sf::Text footer("UP: W    DOWN: S   PLAY:D    QUIT: ESC",
        Assets::getInstance().getFont("main"), 20);
    footer.setFillColor(normalColor);
    footer.setPosition(32, 700);

    m_menuText.setFillColor(normalColor);
    m_menuText.setString(m_title);
    m_menuText.setPosition(10, 10);
    _game->window().draw(m_menuText);

    for (size_t i{ 0 }; i < m_menuStrings.size(); ++i)
    {
        m_menuText.setFillColor((i == m_menuIndex ? selectedColor : normalColor));
        m_menuText.setPosition(32, 32 + (i + 1) * 96);
        m_menuText.setString(m_menuStrings.at(i));
        _game->window().draw(m_menuText);
    }

    _game->window().draw(footer);
}

// Handle actions in the menu scene
void Scene_Menu::sDoAction(const Command& action)
{
    if (action.type() == "START")
    {
        if (action.name() == "UP")
        {
            m_menuIndex = (m_menuIndex + m_menuStrings.size() - 1) % m_menuStrings.size(); // Move up in the menu
        }
        else if (action.name() == "DOWN")
        {
            m_menuIndex = (m_menuIndex + 1) % m_menuStrings.size(); // Move down in the menu
        }
        else if (action.name() == "PLAY")
        {
            _game->changeScene("PLAY", std::make_shared<Scene_Snake>(_game, m_levelPaths[m_menuIndex])); // Change to the play scene
        }
        else if (action.name() == "QUIT")
        {
            onEnd(); // End the menu scene
        }
    }
}
