#define MAINPAGE_H
#ifndef MAINPAGE_H

#include "functions.h"

using namespace std;

class MainPage {

    public:

    char choice;
    // Beginning intro page in implement into main.cpp
    void beginProgram() {  
        centerText(" Express Guest Management System ", 50, '=');

            std::cout << std::endl;
            std::cout << "Please select an option to continue." << std::endl << std::endl;
            std::cout << "Are you a: (G)uest or (E)xpress Employee?" << std::endl << std::endl;
        
            cin >> choice;
        //Determinging if the user is a guest or an express employee and directing them to the appropriate page.

        switch (choice != 'G' || choice != 'g' || choice != 'E' || choice != 'e') {
            case 'G':


            break;

            case 'g': 
                std::cout << "Welcome! Our system is designed to provide a seamless experience for our guests. Please select an option to continue." 
                << std::endl << std::endl;
    
                leftAlignment("New Guest Registration", 20, ' ');
                rightAlignment("Existing Guest Login", 23, ' ');  
                leftAlignment("Pre-Booking ID", 20, ' '); //Make option unavailable until the system has saved the pre-booking ID to the database.
                
                
            break;
            
            case 'E':

            break;

            case 'e': 
                std::cout << "You have selected Express Employee. Please proceed to the login page." << std::endl;
            
            break;

            default: 
                std::cout << "Invalid option. Please select either (G)uest or (E)xpress Employee." << std::endl;
                clearError();
            break;
        }

                

};

#endif