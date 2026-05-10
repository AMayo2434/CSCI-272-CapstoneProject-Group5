#ifndef FUNCTIONS_H
#define FUNCTIONS_H

//Inline functions to be used to avoid multiple declaration errors.

#include <iostream>
#include <iomanip>
#include <string>
#include <limits>
#include <cmath>
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <vector>


//Variables for functions. To be moved to the appropriate classes after testing.



// Fuction to manually center setfill and choose a character
inline void centerText(std::string text, int strlgth, char fill) 
    {
        int symbols = strlgth - text.length();
        int leftsymbols = symbols / 2;
        int rightsymbols = symbols - leftsymbols;

        std::cout << std::setfill(fill) << std::setw(leftsymbols + text.length()) 
        << std::right << text << std::setw(rightsymbols) << " " << std::setfill(' ') 
        << std::endl;
    
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

//All of the above are functional. 
// Going to use a template to calculate totals in Billing. To be moved.


//This will be added to front desk class. All employees will have access to this function to view the CSV file. To be moved after tests
inline void viewCSVFile(std::string fileName) {
    std::ifstream myFile(fileName);
    
    if (myFile.is_open()) {
        std::string line;
        // This loop reads the file line by line from the beginning
        while (std::getline(myFile, line)) {
            std::cout << line << std::endl;
        }
        myFile.close();
    } else {
        std::cerr << "Unable to open file: " << fileName << std::endl;
    }
}

// WORKING



//Template to save a single value to a CSV file. Use of a template requirement implemented. Going to try to get this to work

template<typename T>
void saveToCSV(const std::string& fileName, const T& value) {
    
    std::ofstream myFile(fileName, std::ios::app);

    if (!myFile.is_open()) {
        std::cerr << "Unable to open file: " << fileName << std::endl;
        myFile.close();
    } 
    else {
        std::cerr << "Error. Unable to open file." << std::endl;
    }   
        
}


// Using this to convert values to string for the CSV file.
void convertToStringL(long long int num, std::string& str) {
    str = std::to_string(num);
}

void convertToStringI(int num, std::string& str) {
    str = std::to_string(num);
}

#endif