#ifndef LANDINGPAGE_H
#define LANDINGPAGE_H

#include "functions.h"
#include "Guest.h"

class LandingPage {

    public:
    // Variables for interactive options on the landing page. 
    char choice;
    int guestOption;
    int employeeOption;


    //Default constructor for the landing page class.
    LandingPage() {
        choice = ' ';
        guestOption = 0;
        employeeOption = 0;
    }

// Beginning intro page in implement into main.cpp
    void beginProgram() {  
        centerText(" Express Guest Management System ", 50, '=');

            std::cout << std::endl;
            std::cout << "Please select an option to continue." << std::endl << std::endl;
            std::cout << "Are you a: (G)uest or (E)xpress Employee?" << std::endl;
        
            std::cin >> choice;
    //Determing if the user is a guest or an express employee and directing them to the appropriate page.
    //Guest landing page occurs within this block.
            if (choice == 'G' || choice == 'g') { // Guest Landing Page

                std::cout << "Welcome Guest! This is the portal intended to serve all your needs. \nPlease select an option to continue:" 
                << std::endl;
                std::cout << "\t1. Create An Account" << std::endl;
                std::cout << "\t2. Sign In" << std::endl;
                std::cout << "\t3. Enter Booking ID" << std::endl;
                std::cout << "\t4. Exit Program" << std::endl;
                
                std::cin >> guestOption;

                if (guestOption == 1) {
                    //Add the create an account function. Ensure the user details save to a CSV file.
                    //Redirect to the sign in option after the acocunt is created.
                    void generateCustomerID();
                    void createAccount();
                }
                else if (guestOption == 2) {
                    //Add the sign in function. Ensure the user details are verified with the CSV file w/ unordered map.
                }
                else if (guestOption == 3) {
                    //Add the enter booking ID function which references the booking ID in the CSV file. Brings up user information.
                }
        
                else if (guestOption == 4) { 
                    //Exits program. 
                    std::cout << "Thank you for visiting Express! We hope to see you again soon!" << std::endl;
                    exit(0);                    
                }
// Returns to the main menu if the input is invalid.
                else {
                    std::cout << "Invalid input. Please try again." << std::endl;
                    clearError();
                    beginProgram();
                }
            }
// Employee landing page begins in this else if block. FINISH GUEST LANDING PAGE FIRST.
            else if (choice == 'E' || choice == 'e') { // Employee Landing Page

                std::cout << "Welcome Express Employee! This is the portal intended to help you assist our customers with all their needs.\n Please select an option to continue:" 
                << std::endl;
                std::cout << "\t1. View Guest Information" << std::endl;
                std::cout << "\t2. View Bookings" << std::endl;



            }
// Returns to the main menu if the input is invalid.
            else {
                std::cout << "Invalid input. Please try again." << std::endl;
                clearError();
                beginProgram();
            }                  
    }

    
};

#endif