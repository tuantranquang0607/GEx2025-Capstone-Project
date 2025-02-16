//
// Created by David Burchill on 2023-10-09.
//

#include "Utilities.h"  // Include the Utilities header
#include <numbers>      // Include the numbers library for mathematical constants
#include <cmath>        // Include the cmath library for mathematical functions

static const float PI = std::numbers::pi_v<float>; // Define a constant for PI

// Convert radians to degrees
float radToDeg(float r) {
    return r * 180.f / PI; // Convert radians to degrees
}

// Convert degrees to radians
float degToRad(float d) {
    return d * PI / 180.f; // Convert degrees to radians
}

// Normalize a vector
sf::Vector2f normalize(sf::Vector2f v)
{
    static const float epsi = 0.00001f; // Define a small epsilon value
    float d = length(v);                // Calculate the length of the vector
    if (d > epsi)                       // Check if the length is greater than epsilon
        v = v / d;                      // Normalize the vector
    return v;                           // Return the normalized vector
}

// Calculate the length of a vector
float length(const sf::Vector2f& v)
{
    return std::sqrtf(v.x * v.x + v.y * v.y); // Calculate the length using the Pythagorean theorem
}

// Calculate the distance between two vectors
float dist(const sf::Vector2f& u, const sf::Vector2f& v)
{
    return length(v - u); // Calculate the distance as the length of the difference vector
}

// Calculate the bearing of a vector
float bearing(const sf::Vector2f& v)
{
    return radToDeg(std::atan2(v.y, v.x)); // Calculate the bearing as the angle in degrees
}

// Calculate the unit vector from a bearing
sf::Vector2f uVecBearing(float b)
{
    return sf::Vector2f(std::cos(degToRad(b)), std::sin(degToRad(b))); // Calculate the unit vector from the bearing
}
