////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Assignment:
//  Instructor:     David Burchill
//  Year / Term:    Fall 2024
//  File name:      Source.cpp
//
//  Student name:   Tuan Q. Tran
//  Student email:  tranquangtuan060703@gmail.com
//
//     I certify that this work is my work only, any work copied from Stack Overflow, textbooks,
//     or elsewhere is properly cited.
//
// ////////////////////////////////////////////////////////////////////////////////////////////////////////////

//  BUG
//  list any and all bugs in your code
//  1. I didnt get abc to work because of xyz, I tried ijk...
//

#include <iostream>     // Include the iostream library for input and output
#include "GameEngine.h" // Include the GameEngine header

// Main function
int main()
{
    GameEngine game("../config.txt");   // Create a GameEngine object with the configuration file path
    game.run();                         // Run the game engine
    return 0;                           // Return 0 to indicate successful execution
}