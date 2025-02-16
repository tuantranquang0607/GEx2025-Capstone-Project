//
// Created by David Burchill on 2023-09-27.
//

#ifndef BREAKOUT_UTILITIES_H
#define BREAKOUT_UTILITIES_H

#include <SFML/Graphics.hpp>    // Include the SFML graphics module
#include <iostream>             // Include the iostream library for input and output

// Function to normalize a vector
sf::Vector2f normalize(sf::Vector2f v);

// Function to calculate the bearing (angle) of a vector
float bearing(const sf::Vector2f& v);

// Function to create a unit vector from a bearing (angle)
sf::Vector2f uVecBearing(float b);

// Function to calculate the length of a vector
float length(const sf::Vector2f& v);

// Function to calculate the distance between two vectors
float dist(const sf::Vector2f& u, const sf::Vector2f& v);

// Function to convert radians to degrees
float radToDeg(float r);

// Function to convert degrees to radians
float degToRad(float d);

// Template function to center the origin of an SFML transformable object
template<typename T>
inline void centerOrigin(T &t) {
    auto bounds = t.getLocalBounds();
    t.setOrigin((bounds.width / 2.f) + bounds.left,
                (bounds.height / 2.f) + bounds.top);
}

// Template function to center the origin of an SFML transformable object (pointer version)
template<typename T>
inline void centerOrigin(T *t) {
    auto bounds = t->getLocalBounds();
    t->setOrigin((bounds.width / 2.f) + bounds.left,
                (bounds.height / 2.f) + bounds.top);
}

// Template function to output an SFML vector to an ostream
template <typename T>
inline std::ostream& operator<<(std::ostream& os, const sf::Vector2<T>& v) {
    os << "{" << v.x << ", " << v.y << "}";
    return os;
}

// Template function to output an SFML rectangle to an ostream
template <typename T>
inline std::ostream& operator<<(std::ostream& os, const sf::Rect<T>& r) {
    os << "{{" << r.left << ", " << r.top << "}, {"<< r.width << ", " << r.height << "}";
    return os;
}

// Template function to display the local and global bounds of an SFML transformable object
template <typename T>
inline void displayBounds(const T& t) {
    std::cout << "Local bounds : " << t.getLocalBounds() << "\n";
    std::cout << "Global bounds: " << t.getGlobalBounds() << "\n";
}

#endif //BREAKOUT_UTILITIES_H
