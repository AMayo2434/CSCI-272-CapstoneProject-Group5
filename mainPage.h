#define MAINPAGE_H
#ifndef MAINPAGE_H

#include "functions.h"

using namespace std;

class Mainpage {

    public:
    // Variables for interactive options on the landing page. 
    char choice;
    int guestOption;
    int employeeOption;

    // Beginning intro page in implement into main.cpp
    void beginProgram() {  
        centerText(" Express Guest Management System ", 50, '=');

            std::cout << std::endl;
            std::cout << "Please select an option to continue." << std::endl << std::endl;
            std::cout << "Are you a: (G)uest or (E)xpress Employee?" << std::endl << std::endl;
        
            cin >> choice;
        //Determing if the user is a guest or an express employee and directing them to the appropriate page.
            if (choice == 'G' || choice == 'g') { // Guest Landing Page

                cout << "Welcome Guest! This is the portal intended to serve all your needs. Please select an option to continue." << endl;
                cout << "1. Create an account" << endl;

            }

            else if (choice == 'E' || choice == 'e') { // Employee Landing Page

                cout << "Welcome Express Employee! This is the portal intended to help you assist our customers with all their needs.\n Please select an option to continue." << endl;



            }

            else {
                std::cout << "Invalid input. Please try again." << std::endl;
                clearError();
                beginProgram();
            }                  
    }
};

#endif