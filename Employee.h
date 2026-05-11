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
        std::string passWord;

    public:

    //Constructor
        Employee() {
            employeeID = "";
            passWord = "";
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
                        break; // Exit loop, found user
                    }
                }
            }
            file.close();

            if (loggedIn) {
                std::cout << "Sign in successful! Proceeding to Front Desk Portal." << std::endl;
                return true;
                
            } else {
                std::cout << "Invalid Employee ID or password." << std::endl;
                return false;
            }
        }

    //Getter Functions

        std::string getEmployeeID() const {
            return employeeID;
        }

        std::string getpassWord() const {
            return passWord;
        }

};

#endif
