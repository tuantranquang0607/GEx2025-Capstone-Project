//
// Created by David Burchill on 2023-09-28.
//

#include "EntityManager.h" // Include the EntityManager header
#include "Entity.h" // Include the Entity header

// Constructor to initialize the EntityManager
EntityManager::EntityManager() : _totalEntities(0) {}

// Add a new entity with a specific tag
std::shared_ptr<Entity> EntityManager::addEntity(const std::string &tag) 
{
    // Create a new Entity object
    auto e = std::shared_ptr<Entity>(new Entity(_totalEntities++, tag));

    // Store it in the entities vector
    _EntitiesToAdd.push_back(e);
    // Return a shared pointer to it
    return e;
}

// Get entities by tag
EntityVec &EntityManager::getEntities(const std::string &tag) 
{
    return _entityMap[tag];
}

// Update the entity manager
void EntityManager::update() 
{
    // Debug to remove later
    std::for_each(_entities.begin(), _entities.end(), [](auto e) 
        {
       if (!(e->isActive())) {
           std::cout << "Destroying entity " << e->getTag() << std::endl;
       }
   });
    // Debug end

    // Remove dead entities
    removeDeadEntities(_entities);
    for (auto& [_, entityVec] : _entityMap)
        removeDeadEntities(entityVec);

    // Add new entities
    for (auto e : _EntitiesToAdd)
    {
        _entities.push_back(e);
        _entityMap[e->getTag()].push_back(e);
    }
    _EntitiesToAdd.clear();
}

// Get all entities
EntityVec &EntityManager::getEntities() 
{
    return _entities;
}

// Remove inactive entities from a vector
void EntityManager::removeDeadEntities(EntityVec &v) 
{
    v.erase(std::remove_if(v.begin(), v.end(), [](auto e) { return !(e->isActive()); }), v.end());
}
