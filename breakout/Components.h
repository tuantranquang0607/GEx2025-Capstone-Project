//
// Created by David Burchill on 2023-09-27.
//

#ifndef BREAKOUT_COMPONENTS_H
#define BREAKOUT_COMPONENTS_H


#include <memory>
#include <SFML/Graphics.hpp>
#include "Utilities.h"


struct Component
{
    bool		has{ false };
    Component() = default;
};


struct CShape : public Component
{

    std::shared_ptr<sf::Shape> shape{nullptr};
    CShape() = default;

    CShape(std::shared_ptr<sf::Shape> shape,  const sf::Color& fill, const sf::Color& outline=sf::Color::Black, float thickness = 1.f)
            : shape(shape)
    {
        shape->setFillColor(fill);
        shape->setOutlineColor(outline);
        shape->setOutlineThickness(thickness);

        centerOrigin(shape.get());
    }
};


struct CTransform : public Component
{

    sf::Transformable  tfm;
    sf::Vector2f	pos			{ 0.f, 0.f };
    sf::Vector2f	prevPos		{ 0.f, 0.f };
    sf::Vector2f	vel			{ 0.f, 0.f };
    sf::Vector2f	scale		{ 1.f, 1.f };

    float   angVel{ 0 };
    float	angle{ 0.f };

    CTransform() = default;
    CTransform(const sf::Vector2f& p) : pos(p)  {}
    CTransform(const sf::Vector2f& p, const sf::Vector2f& v)
            : pos(p), prevPos(p),  vel(v){}

};


struct CCollision : public Component
{
    float radius{ 0.f };

    CCollision() = default;
    CCollision(float r)
            : radius(r) {}
};


struct CBoundingBox : public Component
{
    sf::Vector2f size{0.f, 0.f};
    sf::Vector2f halfSize{ 0.f, 0.f };

    CBoundingBox() = default;
    CBoundingBox(const sf::Vector2f& s) : size(s), halfSize(0.5f * s)
    {}
};



struct CInput : public Component
{
    bool up{ false };
    bool left{ false };
    bool right{ false };
    bool down{ false };

    bool spinr{false};
    bool spinl{false};

    CInput() = default;
};


struct CScore : public Component
{
    int score{ 0 };
    CScore(int s = 0) : score(s) {}
};

#endif //BREAKOUT_COMPONENTS_H
