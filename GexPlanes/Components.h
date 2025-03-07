//
// Created by David Burchill on 2023-09-27.
//

#ifndef BREAKOUT_COMPONENTS_H
#define BREAKOUT_COMPONENTS_H

#include "Animation.h"          // Include the Animation class
#include <memory>               // Include the memory library for smart pointers
#include <SFML/Graphics.hpp>    // Include the SFML graphics library
#include "Utilities.h"          // Include utility functions

// Base component structure
struct Component
{
    bool has{ false };      // Flag to indicate if the component is active
    Component() = default;  // Default constructor
};

// Component for handling animations
struct CAnimation : public Component 
{
    Animation animation;                                // Animation object

    CAnimation() = default;                             // Default constructor
    CAnimation(const Animation& a) : animation(a) {}    // Parameterized constructor
};

// Component for autopilot functionality
struct CAutoPilot : public Component
{
    size_t currentLeg{0};               // Current leg of the autopilot path
    sf::Time countdown{sf::Time::Zero}; // Countdown timer for autopilot actions

    CAutoPilot() = default;             // Default constructor
};

// Component for handling missiles
struct CMissiles : public Component 
{
    size_t missileCount{15};    // Number of missiles

    CMissiles() = default;      // Default constructor
};

// Component for handling gun functionality
struct CGun : public Component {
    bool isFiring{false};                   // Flag to indicate if the gun is firing
    sf::Time countdown{sf::Time::Zero};     // Countdown timer for firing rate
    int fireRate{1};                        // Rate of fire
    int spreadLevel{1};                     // Level of bullet spread

    CGun() = default;                       // Default constructor
};

// Component for handling sprites
struct CSprite : public Component 
{
    sf::Sprite sprite;          // Sprite object

    CSprite() = default;        // Default constructor
    CSprite(const sf::Texture& t) : sprite(t) 
    {
        centerOrigin(sprite);   // Center the origin of the sprite
    }
    CSprite(const sf::Texture& t, sf::IntRect r) : sprite(t, r) 
    {
        centerOrigin(sprite);   // Center the origin of the sprite
    }
};

// Component for handling transformations
struct CTransform : public Component
{
    sf::Transformable tfm;                                                                      // Transformable object
    sf::Vector2f pos{ 0.f, 0.f };                                                               // Position
    sf::Vector2f prevPos{ 0.f, 0.f };                                                           // Previous position
    sf::Vector2f vel{ 0.f, 0.f };                                                               // Velocity
    sf::Vector2f scale{ 1.f, 1.f };                                                             // Scale

    float angVel{ 0 };                                                                          // Angular velocity
    float angle{ 0.f };                                                                         // Angle

    CTransform() = default;                                                                     // Default constructor
    CTransform(const sf::Vector2f& p) : pos(p)  {}                                              // Parameterized constructor
    CTransform(const sf::Vector2f& p, const sf::Vector2f& v) : pos(p), prevPos(p),  vel(v){}    // Parameterized constructor
};

// Component for handling collisions
struct CCollision : public Component
{
    float radius{ 0.f };                                                                        // Collision radius

    CCollision() = default;                                                                     // Default constructor
    CCollision(float r) : radius(r) {}                                                          // Parameterized constructor
};

// Component for handling bounding boxes
struct CBoundingBox : public Component
{
    sf::Vector2f size{0.f, 0.f};                                                        // Size of the bounding box
    sf::Vector2f halfSize{ 0.f, 0.f };                                                  // Half size of the bounding box

    CBoundingBox() = default;                                                           // Default constructor
    CBoundingBox(const sf::Vector2f& s) : size(s), halfSize(0.5f * s) {}                // Parameterized constructor
    CBoundingBox(float w, float h) : size(sf::Vector2f{w,h}), halfSize(0.5f * size) {}  // Parameterized constructor
};

// Component for handling input
struct CInput : public Component
{
    bool up{ false };       // Up input flag
    bool left{ false };     // Left input flag
    bool right{ false };    // Right input flag
    bool down{ false };     // Down input flag

    bool spinr{false};      // Spin right input flag
    bool spinl{false};      // Spin left input flag

    CInput() = default;     // Default constructor
};

// Component for handling scores
struct CScore : public Component
{
    int score{ 0 };                 // Score value
    CScore(int s = 0) : score(s) {} // Parameterized constructor
};

// Component for handling health
struct CHealth : public Component 
{
    int hp{ 1 };                // Health points

    CHealth() = default;        // Default constructor
    CHealth(int hp) : hp(hp) {} // Parameterized constructor
};

// Component for handling states
struct CState : public Component {
    std::string state{ "none" };                // State value

    CState() = default;                         // Default constructor
    CState(const std::string& s) : state(s) {}  // Parameterized constructor
};

// Component for handling shapes
struct CShape : public Component
{
    std::shared_ptr<sf::Shape> shape{ nullptr };

    CShape() = default;

    CShape(std::shared_ptr<sf::Shape> shape, const sf::Color& fill, const sf::Color& outline = sf::Color::Black, float thickness = 1.f)
        : shape(shape)
    {
        shape->setFillColor(fill);
        shape->setOutlineColor(outline);
        shape->setOutlineThickness(thickness);
    }
};

// Component for handling directions
struct CDirection : public Component
{
    sf::Vector2f direction{ 1.f, 1.f };

    CDirection() = default;
    CDirection(const sf::Vector2f& dir) : direction(dir) {}
};


#endif //BREAKOUT_COMPONENTS_H
