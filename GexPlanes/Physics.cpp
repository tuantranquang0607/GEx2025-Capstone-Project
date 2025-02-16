#include "Physics.h" // Include the Physics header
#include <cmath> // Include the cmath library for mathematical functions

// Function to get the overlap between two entities
sf::Vector2f Physics::getOverlap(std::shared_ptr<Entity> a, std::shared_ptr<Entity> b)
{
    sf::Vector2f overlap(0.f, 0.f); // Initialize overlap vector to zero
    if (!a->hasComponent<CBoundingBox>() || !b->hasComponent<CBoundingBox>())                               // Check if both entities have bounding boxes
        return overlap;                                                                                     // Return zero overlap if either entity lacks a bounding box

    auto atx = a->getComponent<CTransform>();                                                               // Get the transform component of entity a
    auto abb = a->getComponent<CBoundingBox>();                                                             // Get the bounding box component of entity a
    auto btx = b->getComponent<CTransform>();                                                               // Get the transform component of entity b
    auto bbb = b->getComponent<CBoundingBox>();                                                             // Get the bounding box component of entity b

    if (abb.has && bbb.has) // Check if both entities have valid bounding boxes
    {
        float dx = std::abs(atx.pos.x - btx.pos.x);                                                         // Calculate the absolute difference in x positions
        float dy = std::abs(atx.pos.y - btx.pos.y);                                                         // Calculate the absolute difference in y positions
        overlap = sf::Vector2f(abb.halfSize.x + bbb.halfSize.x - dx, abb.halfSize.y + bbb.halfSize.y - dy); // Calculate the overlap
    }
    return overlap; // Return the overlap vector
}

// Function to get the previous overlap between two entities
sf::Vector2f Physics::getPreviousOverlap(std::shared_ptr<Entity> a, std::shared_ptr<Entity> b)
{
    sf::Vector2f overlap(0.f, 0.f);                                             // Initialize overlap vector to zero
    if (!a->hasComponent<CBoundingBox>() || !b->hasComponent<CBoundingBox>())   // Check if both entities have bounding boxes
        return overlap;                                                         // Return zero overlap if either entity lacks a bounding box

    auto atx = a->getComponent<CTransform>();                                   // Get the transform component of entity a
    auto abb = a->getComponent<CBoundingBox>();                                 // Get the bounding box component of entity a
    auto btx = b->getComponent<CTransform>();                                   // Get the transform component of entity b
    auto bbb = b->getComponent<CBoundingBox>();                                 // Get the bounding box component of entity b

    if (abb.has && bbb.has) // Check if both entities have valid bounding boxes
    {
        float dx = std::abs(atx.prevPos.x - btx.prevPos.x);                     // Calculate the absolute difference in previous x positions
        float dy = std::abs(atx.prevPos.y - btx.prevPos.y);                     // Calculate the absolute difference in previous y positions
        overlap = sf::Vector2f(abb.halfSize.x + bbb.halfSize.x - dx, abb.halfSize.y + bbb.halfSize.y - dy); // Calculate the previous overlap
    }
    return overlap; // Return the previous overlap vector
}
