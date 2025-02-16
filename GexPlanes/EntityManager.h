//
// Created by David Burchill on 2023-09-28.
//

#ifndef BREAKOUT_ENTITYMANAGER_H
#define BREAKOUT_ENTITYMANAGER_H

#include <map>      // Include the map container from the standard library
#include <vector>   // Include the vector container from the standard library
#include <string>   // Include the string library
#include <memory>   // Include the memory library for smart pointers

// Forward declaration of the Entity class
class Entity;

// Type aliases for convenience
using sPtrEntt  = std::shared_ptr<Entity>;              // Shared pointer to an Entity
using EntityVec = std::vector<std::shared_ptr<Entity>>; // Vector of shared pointers to Entities
using EntityMap = std::map<std::string, EntityVec>;     // Map of entity vectors categorized by string tags

// Class to manage entities in the game
class EntityManager
{
private:
    EntityVec	    _entities;          // Vector to store all entities
    EntityMap	    _entityMap;         // Map to categorize entities by tags
    size_t		    _totalEntities{0};  // Total number of entities
    EntityVec	    _EntitiesToAdd;     // Vector to store entities to be added

    // Private method to remove inactive entities from a vector
    void		    removeDeadEntities(EntityVec& v);

public:
    // Constructor
    EntityManager();

    // Method to add a new entity with a specific tag
    std::shared_ptr<Entity>         addEntity(const std::string& tag);

    // Method to get all entities
    EntityVec&                      getEntities();

    // Method to get entities by tag
    EntityVec&                      getEntities(const std::string& tag);

    // Method to update the entity manager
    void                            update();
};

#endif //BREAKOUT_ENTITYMANAGER_H
