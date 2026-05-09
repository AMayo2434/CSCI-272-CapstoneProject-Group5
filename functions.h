#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <iostream>
#include <iomanip>
#include <string>
#include <limits>

// Fuction to manually center setfill and choose a character
inline void centerText(std::string text, int strlgth, char fill) 
    {
        int symbols = strlgth - text.length();
        int leftsymbols = symbols / 2;
        int rightsymbols = symbols - leftsymbols;

        std::cout << std::setfill(fill) << std::setw(leftsymbols + text.length()) 
        << std::right << text << std::setw(rightsymbols) << " " << std::setfill(' ') << std::endl;
    
    }

// Function to align text to the left with a fill character
inline void leftAlignment(std::string text, int width, char fill)
{
    std::cout << std::setfill(fill) << std::setw(width) 
    << std::setiosflags(std::ios::left) << text << std::endl;
}

// Function to align text to the right with a fill character
inline void rightAlignment(std::string text, int width, char fill)
{
    std::cout << std::setfill(fill) << std::setw(width) 
    << std::setiosflags(std::ios::right) << text << std::endl;
}

//Function to clear incorrect inputs
inline void clearError() {
    std::cin.clear(); // Clear the error state
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Discard invalid input
}

#endif