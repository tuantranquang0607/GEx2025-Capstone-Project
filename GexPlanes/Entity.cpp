//
// Created by David Burchill on 2023-09-27.
//

#include "Entity.h" // Include the Entity header

// Constructor to initialize the entity with an ID and tag
Entity::Entity(size_t id, const std::string &tag) : _tag(tag) // Initialize the tag
        , _id(id) // Initialize the ID
{
}

// Mark the entity as inactive
void Entity::destroy() {
    _active = false; // Set the active flag to false
}

// Get the entity's ID
const size_t &Entity::getId() const {
    return _id; // Return the ID
}

// Get the entity's tag
const std::string &Entity::getTag() const {
    return _tag; // Return the tag
}

// Check if the entity is active
bool Entity::isActive() const {
    return _active; // Return the active flag
}
