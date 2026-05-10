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
#include <filesystem>

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
inline void updateCSV(std::string filename, std::string id, int column, std::string updatedVal) {
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


//Can be used for all edits so inline above^


//USing a template to store a single or multiple arguments. 
//Using the filesystem library to use the exists function to see if there is already existing data.
//ues func
template <typename... Args>
void writeRowToCSV(const std::string& fileName, Args... args) {
    //Using the filesystem library to use the exists function to see if there is already existing data.
    bool isNewFile = !std::filesystem::exists(fileName) || std::filesystem::file_size(fileName) == 0;

    std::ofstream myFile(fileName, std::ios::app);
    
    if (myFile.is_open()) {
       
        size_t count = 0;
        ((myFile << args << (++count < sizeof...(args) ? "," : "")), ...);
        myFile << ",";

        myFile.close();
    } else {
        std::cerr << "Unable to open file: " << fileName << std::endl;
    }
}

template <typename... Args>
void writeToEndCSV(const std::string& fileName, Args... args) {
    //Using the filesystem library to use the exists function to see if there is already existing data.
    bool isNewFile = !std::filesystem::exists(fileName) || std::filesystem::file_size(fileName) == 0;

    std::ofstream myFile(fileName, std::ios::app);
    
    if (myFile.is_open()) {
       
        size_t count = 0;
        ((myFile << args << (++count < sizeof...(args) ? "," : "")), ...);
        myFile << std::endl;
        
        myFile.close();
    } else {
        std::cerr << "Unable to open file: " << fileName << std::endl;
    }
}



// FINAL FRONT DESK FUNCTION ABOVE. TESTED AND WORKING.

bool checkLogin(std::string inputUser, std::string inputPass) {
    std::ifstream file("users.csv");
    std::string line, csvUser, csvPass;

    if (file.is_open()) {
        // Read each line from the CSV
        while (std::getline(file, line)) {
            std::stringstream ss(line);
            
            // Extract the username (first column) and password (second column)
            if (std::getline(ss, csvUser, ',') && std::getline(ss, csvPass, ',')) {
                // Check if the current row matches the input
                if (csvUser == inputUser && csvPass == inputPass) {
                    file.close();
                    return true;
                }
            }
        }
        file.close();
    }
    return false;
}
// MAYBE RUNNING INTO FRONT DESK

//DELETE CUSTOMER OR ENTRY (FRONT DESK)

inline void deleteRowByID(std::string filename, std::string targetID) {
    std::ifstream fileIn(filename);
    std::ofstream fileOut("temp.csv");
    std::string line;
    bool found = false;

    if (!fileIn.is_open()) {
        std::cerr << "Error: Could not open file." << std::endl;
        return;
    }

    while (std::getline(fileIn, line)) {
        std::stringstream ss(line);
        std::string currentID;
        
        // Assumes ID is in the first column
        std::getline(ss, currentID, ',');

        if (currentID != targetID) {
            fileOut << line << "\n";
        } else {
            found = true;
        }
    }

    fileIn.close();
    fileOut.close();

    // Replace original file with temporary file
    std::remove(filename.c_str());
    std::rename("temp.csv", filename.c_str());

    if (found) std::cout << "Record deleted successfully.\n";
    else std::cout << "ID not found.\n";
}



//Checks for duplicate IDs.
bool isDuplicate(const std::string& filename, const std::string& newID) {
    std::ifstream file(filename);
    std::string line;

    if (!file.is_open()) {
        std::cerr << "Error opening file!" << std::endl;
        return false;
    }

    // Read file line by line
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string existingID;

        // Assuming ID is the first column
        std::getline(ss, existingID, ',');

        if (existingID == newID) {
            return true; // Duplicate found
        }
    }
    return false; // No duplicate
}


// Using this to convert values to string for the CSV file.
inline void convertToStringL(long long int num, std::string& str) {
    str = std::to_string(num);
}

inline void convertToStringI(int num, std::string& str) {
    str = std::to_string(num);
}

#endif