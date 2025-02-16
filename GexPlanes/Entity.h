//
// Created by David Burchill on 2023-09-27.
//

#ifndef BREAKOUT_ENTITY_H
#define BREAKOUT_ENTITY_H

#include <tuple>        // Include the tuple container from the standard library
#include <string>       // Include the string library

#include "Components.h" // Include the Components header

// Forward declaration of EntityManager class
class EntityManager;

// Define a tuple to hold all possible components
using ComponentTuple = std::tuple<CSprite, CMissiles, CGun, CHealth, CState, CAnimation, CTransform, CAutoPilot, CBoundingBox, CInput, CScore>;

// Class to represent an entity in the game
class Entity 
{
private:
    friend class EntityManager;                 // Allow EntityManager to access private members
    Entity(size_t id, const std::string &tag);  // Private constructor, entities can only be created by EntityManager

    const size_t            _id{0};             // Unique identifier for the entity
    const std::string       _tag{"Default"};    // Tag to categorize the entity
    bool                    _active{true};      // Flag to indicate if the entity is active
    ComponentTuple          _components;        // Tuple to hold the entity's components

public:
    void                    destroy();          // Mark the entity as inactive
    const size_t&           getId() const;      // Get the entity's ID
    const std::string&      getTag() const;     // Get the entity's tag
    bool                    isActive() const;   // Check if the entity is active

    // Component API
    template<typename T>
    inline bool hasComponent() const {
        return getComponent<T>().has;                   // Check if the entity has a specific component
    }

    template<typename T, typename... TArgs>
    inline T& addComponent(TArgs &&... mArgs) {
        auto &component = getComponent<T>();
        component = T(std::forward<TArgs>(mArgs)...);   // Add a component to the entity
        component.has = true;
        return component;
    }

    template<typename T>
    inline bool removeComponent()  {
        return getComponent<T>().has = false;           // Remove a component from the entity
    }

    template<typename T>
    inline T& getComponent() {
        return std::get<T>(_components);                // Get a specific component from the entity
    }

    template<typename T>
    inline const T& getComponent() const {
        return std::get<T>(_components);                // Get a specific component from the entity (const version)
    }
};

#endif //BREAKOUT_ENTITY_H
