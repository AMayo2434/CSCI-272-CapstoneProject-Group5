#ifndef EMPLOYEE_H
#define EMPLOYEE_H

#include "functions.h"
#include <fstream>
#include <sstream>
#include <string>
#include <iostream>

class Employee {

    private:
        std::string employeeID;
        std::string firstName;
        std::string lastName;
        std::string position;
        std::string passWord;

    public:

    //Constructor
        Employee() {
            employeeID = "";
            firstName = "";
            lastName = "";
            position = "";
            passWord = "";
        }

    //Operator overloading to combine first and last names
        std::string operator+(const Employee& other) const {
            return firstName + " " + lastName;
        }

    //Alternative: create a function to get full name using operator overloading
        std::string getFullName() const {
            return firstName + " " + lastName;
        }

    //Sign in function - verifies employee ID and password from EmployeeV.csv
        bool signIn() {
            std::string enterEmployeeID, enterPassWord;
            std::string csvEmployeeID, csvPass;
            bool loggedIn = false;

            centerText(" Express Employee Sign In ", 80, '=');

            std::cout << "\t= = = Please enter your Employee ID = = =" << std::endl;
            std::cin >> enterEmployeeID;
            std::cout << "Please enter your password: ";
            std::cin >> enterPassWord;

            std::ifstream file("EmployeeV.csv");
            if (!file.is_open()) {
                std::cout << "Error. Unable to open file." << std::endl;
                return false;
            }

            std::string loginline;
            //Reads entire file
            while (std::getline(file, loginline)) {
                if (loginline.empty()) continue;

                std::stringstream ss(loginline);
                
                //Parse CSV: EmployeeID, Password
                if (std::getline(ss, csvEmployeeID, ',') && std::getline(ss, csvPass)) {
                    
                    // Trim whitespace
                    csvEmployeeID.erase(0, csvEmployeeID.find_first_not_of(" \t"));
                    csvEmployeeID.erase(csvEmployeeID.find_last_not_of(" \t") + 1);
                    
                    csvPass.erase(0, csvPass.find_first_not_of(" \t"));
                    csvPass.erase(csvPass.find_last_not_of(" \t") + 1);
                    
                    if (!csvPass.empty() && csvPass.back() == '\r') csvPass.pop_back();

                    if (csvEmployeeID == enterEmployeeID && csvPass == enterPassWord) {
                        this->employeeID = csvEmployeeID;
                        this->passWord = csvPass;
                        loggedIn = true;
                        file.close();
                        
                        // Load employee details from Employee.csv
                        loadEmployeeInfo();
                        break;
                    }
                }
            }
            file.close();

            if (loggedIn) {
                std::cout << "Sign in successful! Welcome, " << getFullName() << "!" << std::endl;
                return true;
                
            } else {
                std::cout << "Invalid Employee ID or password." << std::endl;
                return false;
            }
        }

    //Load employee information from Employee.csv
        void loadEmployeeInfo() {
            std::ifstream myFile("Employee.csv");
            
            if (!myFile.is_open()) {
                std::cerr << "Unable to open Employee.csv" << std::endl;
                return;
            }
            
            std::string line;
            bool headerSkipped = false;
            
            while (std::getline(myFile, line)) {
                if (!headerSkipped) {
                    headerSkipped = true;
                    continue;  // Skip header line
                }
                
                std::stringstream ss(line);
                std::string empID, fName, lName, pos;
                
                // Parse CSV: EmployeeID, FirstName, LastName, Position
                if (std::getline(ss, empID, ',') &&
                    std::getline(ss, fName, ',') &&
                    std::getline(ss, lName, ',') &&
                    std::getline(ss, pos, ',')) {
                    
                    // Trim whitespace from empID
                    empID.erase(0, empID.find_first_not_of(" \t"));
                    empID.erase(empID.find_last_not_of(" \t") + 1);
                    
                    // Check if this is the logged-in employee
                    if (empID == employeeID) {
                        // Trim whitespace from all fields
                        fName.erase(0, fName.find_first_not_of(" \t"));
                        fName.erase(fName.find_last_not_of(" \t") + 1);
                        lName.erase(0, lName.find_first_not_of(" \t"));
                        lName.erase(lName.find_last_not_of(" \t") + 1);
                        pos.erase(0, pos.find_first_not_of(" \t"));
                        pos.erase(pos.find_last_not_of(" \t") + 1);
                        
                        // Assign to member variables
                        this->firstName = fName;
                        this->lastName = lName;
                        this->position = pos;
                        
                        myFile.close();
                        return;
                    }
                }
            }
            
            myFile.close();
        }

    //Getter Functions

        std::string getEmployeeID() const {
            return employeeID;
        }

        std::string getFirstName() const {
            return firstName;
        }

        std::string getLastName() const {
            return lastName;
        }

        std::string getPosition() const {
            return position;
        }

        std::string getpassWord() const {
            return passWord;
        }

};

#endif
