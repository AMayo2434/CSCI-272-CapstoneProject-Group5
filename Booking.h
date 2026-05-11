#ifndef BOOKING_H
#define BOOKING_H

#include "functions.h"
#include "Guest.h"

using namespace std;

class Booking {

    protected:
        string roomID;
        string roomType;
        int numNights;
        string checkInDate;
        string checkOutDate;

    public:
        int guestPortalChoice = 0;

        Booking() : numNights(0) {}

        //Function prototypes placed above to be implemented into guest portal.
        void displayUserInfo(Guest &g);
        void displayCurrentBooking();



    //This will be declared as a friend function inside guest to give the user access to the booking class
    //The class will only affect their data in the databases.
        void guestPortal(Guest& g) {
            //Similar to login page, provides user with options to 
            centerText(" WELCOME, EXPRESS GUEST ", 50, ('-'));
            std::cout<< "Customer ID: " << g.getCustomerID() << std::endl;
            viewSingleRow("GuestInformation.csv", g.getCustomerID());
            std::cout << std::endl;
        
            cout << "Please select an option to continue: " << endl;
            cout << "\t1. Display User Info" << endl;
            cout << "\t2. Display Current Booking" << endl;
            cout << "\t3. Create New Booking" << endl;
            cout << "\t4. Edit Profile" << endl;
            cout << "\t5. Log Out" << endl;

            cin >> guestPortalChoice;

            while (guestPortalChoice != 5) {
            if (guestPortalChoice == 1) {
                    displayUserInfo(g);
            }
            else if (guestPortalChoice == 2) {
                displayCurrentBooking();
            }
            else if (guestPortalChoice == 3) {
                
            }
        
            }
        
        
        }


        void displayUserInfo(Guest &g) {
            
            centerText("User Profile", 20, '-');

                cout << "Name: " << g.getregistrantName() << endl;
                cout << "Customer ID: " << g.getCustomerID() << endl;
                cout << "Email Address: " << g.getemailAddress() << endl;
                cout << "Phone Number: " << g.getphoneNumber() << endl;
                cout << "Country/Region: " << g.getcountryOrRegion() << endl;
                cout << "ZIP Code: " << g.getzipCode() << endl;

            centerText("-", 20, '-');
         }


        void displayCurrentBooking() {
            
            centerText("Current Booking", 30, '-');
                cout << "Room Type: " << roomType << endl;
                cout << "Room ID: " << roomID << endl;
                cout << "Check-in: " << checkInDate << endl;
                cout << "Check-out: " << checkOutDate << endl;
                cout << "Nights: " << numNights << endl;
            centerText("=", 30, '=');

        }
         
      
        //Booking Date, Check-in Date, Check-out Date, Room Type, RoomId
        void createNewBooking() {
            centerText("NEW BOOKING", 20, '=');
            
            std::string date = assignDate();
            cout << "Booking Date: " << date;

            cout << "Please enter a check in date: " << endl;



            cout << "How many nights are you staying?: ";
            cin >> numNights;
            clearError();

            checkOutDate = stoi(determineDate(numNights));


            cout <<  "Enter the room type (Standard, Deluxe, Presidential Suite. Please enter as given.)";
            cin >> roomType;
            
            if (roomType == "Standard") {
                viewSingleRow("Room.csv", "Deluxe Room");

            }

            else if (roomType == "Deluxe")
                viewSingleRow("Room.csv", "Deluxe Room");


            else if (roomType == "Presidential Suite");



            writeToEndCSV("GuestInformation.csv", date);
             
             
             
         }
         
        void editProfile() {
             cout << "TBD" << endl;
         }

//Destructor to clear entry at the end of the program
         ~Booking() {};
};
#endif