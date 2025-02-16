#pragma once

#include "Entity.h"             // Include the Entity class

#include <SFML/Graphics.hpp>    // Include SFML graphics module
#include <SFML/Audio.hpp>       // Include SFML audio module

#include <vector>               // Include the vector container from the standard library
#include <iostream>             // Include the iostream library for input and output
#include <memory>               // Include the memory library for smart pointers
#include <fstream>              // Include the fstream library for file operations
#include <sstream>              // Include the sstream library for string stream operations
#include <algorithm>            // Include the algorithm library for standard algorithms

namespace Physics
{
    // Function to get the overlap between two entities
    sf::Vector2f getOverlap(std::shared_ptr<Entity> a, std::shared_ptr<Entity> b);

    // Function to get the previous overlap between two entities
    sf::Vector2f getPreviousOverlap(std::shared_ptr<Entity> a, std::shared_ptr<Entity> b);
};
