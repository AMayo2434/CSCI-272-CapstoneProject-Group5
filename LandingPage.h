#ifndef LANDINGPAGE_H
#define LANDINGPAGE_H

#include "functions.h"
#include "Guest.h"
#include "Booking.h"
#include "Employee.h"
#include "Frontdesk.h"

class LandingPage {

    public:
    // Variables for interactive options on the landing page. 
    char choice;
    int guestOption;
    int employeeOption;
    Guest currentGuest;
    Employee currentEmployee;
    Booking portalSystem;
    FrontDesk frontDeskPortal;
    
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
                        currentGuest.generateCustomerID();
                        currentGuest.createAccount();
                }
                else if (guestOption == 2) {
                    //Add the sign in function. Ensure the user details are verified with the CSV file. 
                        if(currentGuest.signIn()){
                            portalSystem.guestPortal(currentGuest);
                        }
                        else {
                            std::cout << "Returning to main menu. Invalid login." << std::endl;
                            beginProgram();
                        }
                        
                }
                else if (guestOption == 3) {
                    //Add the enter booking ID function which references the booking ID in the CSV file. Brings up user information.
                    //This only details the basic information of the booking and redirects the user to sign in.
                    std::cout << "Please enter your BookingID: ";
                    
                    std::cout << "Thank you for entering your ID for [PLHDER]. Please proceed to the sign in page to make any adjustments to your stay.";
                    
                    currentGuest.signIn();
                    if(currentGuest.signIn()){
                            portalSystem.guestPortal(currentGuest);
                        }
                        else {
                            std::cout << "Returning to main menu. Invalid login." << std::endl;
                            beginProgram();
                        }

                }
        
                else if (guestOption == 4) { 
                    //Exits program. 
                    std::cout << "Thank you for visiting Express GP! We hope to see you again soon!" << std::endl;
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

                std::cout << "\t1. Sign In" << std::endl;
                std::cout << "\t2. Exit Program" << std::endl;
                
                std::cin >> employeeOption;

                if (employeeOption == 1) {
                    //Employee sign in function - verifies employee ID and password from EmployeeV.csv
                    if (currentEmployee.signIn()) {
                        //Upon successful login, grant access to Front Desk portal
                        employeePortal();
                    }
                    else {
                        std::cout << "Returning to main menu. Invalid login." << std::endl;
                        beginProgram();
                    }
                }
                else if (employeeOption == 2) {
                    //Exits program.
                    std::cout << "Thank you for visiting Express GP! We hope to see you again soon!" << std::endl;
                    exit(0);
                }
// Returns to the main menu if the input is invalid.
                else {
                    std::cout << "Invalid input. Please try again." << std::endl;
                    clearError();
                    beginProgram();
                }
            }
// Returns to the main menu if the input is invalid.
            else {
                std::cout << "Invalid input. Please try again." << std::endl;
                clearError();
                beginProgram();
            }                  
    }

    //Employee portal function - provides access to Front Desk features upon successful login
    void employeePortal() {
        int menuChoice = 0;
        
        centerText(" Front Desk Portal ", 80, '=');
        
        std::cout << "Welcome to the Front Desk Portal, Employee " << currentEmployee.getEmployeeID() << "!" << std::endl;
        std::cout << "\nPlease select an option:" << std::endl;
        std::cout << "\t1. View Guest Information" << std::endl;
        std::cout << "\t2. View Bookings" << std::endl;
        std::cout << "\t3. Add Guest" << std::endl;
        std::cout << "\t4. Remove Guest" << std::endl;
        std::cout << "\t5. Sign Out" << std::endl;
        
        std::cin >> menuChoice;
        
        switch(menuChoice) {
            case 1: {
                std::cout << "Guest Information feature - To be implemented." << std::endl;
                // Add guest information viewing functionality
                break;
            }
            case 2: {
                std::cout << "Bookings feature - To be implemented." << std::endl;
                // Add booking viewing functionality
                break;
            }
            case 3: {
                std::cout << "Add Guest - Enter guest name: ";
                std::string guestName;
                std::cin.ignore();
                std::getline(std::cin, guestName);
                frontDeskPortal.addGuest(guestName);
                employeePortal();
                break;
            }
            case 4: {
                std::cout << "Remove Guest - Enter guest name: ";
                std::string removeGuest;
                std::cin.ignore();
                std::getline(std::cin, removeGuest);
                frontDeskPortal.removeGuest(removeGuest);
                employeePortal();
                break;
            }
            case 5: {
                std::cout << "Signing out. Returning to main menu." << std::endl;
                beginProgram();
                break;
            }
            default: {
                std::cout << "Invalid input. Please try again." << std::endl;
                clearError();
                employeePortal();
                break;
            }
        }
    }
    
};

#endif