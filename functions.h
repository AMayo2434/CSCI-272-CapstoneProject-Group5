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
#include <cstdio>

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

// FRONT DESK FUNCTIONS
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

// Function that will update a specific value in a CSV file based on an ID.
//FRONT DESK FUNCTION
//WORKING DO NOT TOUCH
//Allows the user to overwrite files. 
//Refefenced from online and stack overflow.
void updateCSV(std::string filename, std::string id, int column, std::string updatedVal) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error. Could not open file: " << filename << std::endl;
        return;
    }

    std::ofstream temp("temp.csv");
    if (!temp.is_open()) {
        std::cerr << "Error: Could not create temp file" << std::endl;
        file.close();
        return;
    }

    std::string line, word;
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::vector<std::string> row;
        
        while (std::getline(ss, word, ',')) {
            row.push_back(word);
        }
        //If the row isn't empty and the row contains an ID corresponding to the deired
        if (!row.empty() && row[0] == id) {
            if (column >= 0 && column < row.size()) {
                row[column] = updatedVal;
            }
        }

        // Write the row back to the temp file
        for (size_t i = 0; i < row.size(); i++) {
            temp << row[i] << (i == row.size() - 1 ? "" : ",");
        }
        temp << "\n";
    }
    file.close(); 
    temp.close(); 

    // Replace original file with updated file
    if (std::remove(filename.c_str()) != 0) {
        std::perror("Error deleting file.");
    } else if (std::rename("temp.csv", filename.c_str()) != 0) {
        std::perror("Error renaming file.");
    }
}


//Template to save a single value to a CSV file. Use of a template requirement implemented. Going to try to get this to work
//Type traits are used to make decisions based on the data type passed into the variable.
//Sometimes the code may perform differently based on different data types. 
template<typename T>
void saveToCSV(const std::string& fileName, const T& value) {
  
    std::ifstream checkFile(fileName);
    
    // Using type traits to avoid errors in code. This type checks to see if the CSV file is empty.
    bool isEmpty = (checkFile.peek() == std::ifstream::traits_type::eof());
    checkFile.close();

    // Uses the basic
    std::ofstream myFile(fileName, std::ios::app);
    if (myFile.is_open()) {
        if (isEmpty) {
            //Creating an error message to notify the employee of a need for a manual reset.
            myFile << "Headers missing from file. Please manually edit and re-run save program.";
        }
        myFile << value << ",";
        myFile.close();
    } else {
        std::cerr << "Unable to open file: " << fileName << std::endl;
    }
}

// FINAL FRONT DESK FUNCTION ABOVE. TESTED AND WORKING.


// Using this to convert values to string for the CSV file.
void convertToStringL(long long int num, std::string& str) {
    str = std::to_string(num);
}

void convertToStringI(int num, std::string& str) {
    str = std::to_string(num);
}

#endif