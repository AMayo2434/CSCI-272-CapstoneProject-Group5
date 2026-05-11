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
                        //Upon successful login, hand control to FrontDesk::runPortal().
                        //runPortal() owns a while loop that keeps the employee in the portal
                        //until they explicitly choose Sign Out (option 17).
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

    // -------------------------------------------------------
    // employeePortal()
    //
    // Called after a successful employee login.
    // Delegates entirely to FrontDesk::runPortal() which owns
    // a while loop — the employee stays in the portal until
    // they select Sign Out (option 17).
    //
    // When runPortal() returns, control comes back here and
    // then back to beginProgram() so the main menu reappears,
    // allowing another employee to log in without restarting.
    // -------------------------------------------------------
    void employeePortal() {
        // Pass employee details so the portal header displays correctly.
        // runPortal() loops internally — no recursion, no accidental exit.
        frontDeskPortal.runPortal(
            currentEmployee.getFullName(),
            currentEmployee.getEmployeeID(),
            currentEmployee.getPosition()
        );

        // Returned from runPortal() means the employee signed out.
        // Return to the main landing page.
        beginProgram();
    }
    
};

#endif
