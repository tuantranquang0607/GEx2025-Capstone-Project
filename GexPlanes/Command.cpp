#include "Command.h" // Include the Command header

// Default constructor
Command::Command()
{
}

// Parameterized constructor to initialize the command with a name and type
Command::Command(const std::string& name, const std::string& type)
    : m_name(name) // Initialize the name
    , m_type(type) // Initialize the type
{
}

// Getter for the command name
const std::string& Command::name() const
{
    return m_name; // Return the name of the command
}

// Getter for the command type
const std::string& Command::type() const
{
    return m_type; // Return the type of the command
}

// Method to get a string representation of the command
std::string Command::toString() const
{
    return m_name + ":" + m_type; // Return the name and type concatenated with a colon
}
