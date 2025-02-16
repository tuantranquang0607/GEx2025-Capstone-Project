#pragma once

#include <string> // Include the string library

// Class to represent a command with a name and type
class Command
{
private: 
    std::string m_name{ "NONE" };                               // Name of the command, default is "NONE"
    std::string m_type{ "NONE" };                               // Type of the command, default is "NONE"

public:
    Command(); // Default constructor
    Command(const std::string& name, const std::string& type);  // Parameterized constructor

    const std::string& name() const;                            // Getter for the command name
    const std::string& type() const;                            // Getter for the command type

    std::string toString() const;                               // Method to get a string representation of the command
};
