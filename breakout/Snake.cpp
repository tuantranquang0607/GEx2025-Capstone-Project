#include "Snake.h"

Snake::Snake() : m_body(std::list<sf::Sprite>(1))
{
    m_head = --m_body.end();
    m_tail = m_body.begin();
}

Snake::~Snake()
{
}

void Snake::Init(const sf::Texture& texture)
{
    float x = 24.f;

    for (auto& piece : m_body)
    {
        piece.setTexture(texture);
        piece.setPosition({ x, 24.f });
        x += 24.f;
    }
}

void Snake::Move(const sf::Vector2f& direction)
{
    m_tail->setPosition(m_head->getPosition() + direction);

    m_head = m_tail;
    ++m_tail;

    if (m_tail == m_body.end())
    {
        m_tail = m_body.begin();
    }
}

bool Snake::IsOn(const sf::Sprite& other) const
{
    return other.getGlobalBounds().intersects(m_head->getGlobalBounds());
}

void Snake::Grow(const sf::Vector2f& direction)
{
    sf::Sprite newPiece;

    newPiece.setTexture(*(m_body.begin()->getTexture()));
    newPiece.setPosition(m_head->getPosition() + direction);

    m_head = m_body.insert(++m_head, newPiece);
}

bool Snake::IsSelfIntersecting() const
{
    return false;

    /*bool flag = false;

    for (auto piece = m_body.begin(); piece != m_body.end(); ++piece)
    {
        if (m_head != piece)
        {
            flag = IsOn(*piece);

            if (flag)
            {
                break;
            }
        }
    }

    return flag;*/
}

void Snake::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    for (auto& piece : m_body)
    {
        target.draw(piece);
    }
}
