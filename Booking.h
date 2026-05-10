#ifndef BOOKING_H
#define BOOKING_H

#include "functions.h"
#include "Guest.h"

using namespace std;

class Booking {

    public:
    //This will be declared as a friend function inside guest to give the user access to the booking class
    //The class will only affect their data in the databases.
        void guestPortal(Guest& g) {
            //Similar to login page, provides user with options to 
            centerText(" WELCOME, EXPRESS GUEST ", 50, ('-'));
            std::cout<< "Customer ID: " << g.getCustomerID() << std::endl;
            std::cout<<"Name: " << g.getregistrantName() << std::endl;
            std::cout << "Email: " << g.getemailAddress() << std::endl;

            std::cout << std::endl;


            
         
            

















        }


        void displayUserInfo(Guest &g) {
        	cout << "Name: " << g.getregistrantName() << endl;
        	cout << "Customer ID: " << g.getCustomerID() << endl;
        	cout << "Email Address: " << g.getemailAddress() << endl;
        	cout << "Phone Number: " << g.getphoneNumber() << endl;
        	cout << "Country/Region: " << g.getcountryOrRegion() << endl;
         }


        void displayCurrentBooking() {
             //cout << "Current Bookings: " << roomType << " book for " << numNights << "nights." << endl;
         }
         
        void displayUpcomingBookings() {
             cout << "TBD" << endl;
         }
         //Booking Date, Check-in Date, Check-out Date, RoomID, Room Type
        void createNewBooking() {
            centerText("NEW BOOKING", 20, '=');
            
            
            cout << "Enter the type of room wanted: ";
             //cin >> roomType;
             
             cout << "How many nights are you staying?: ";
             //cin >> numNights;
             
         }
         
        void editProfile() {
             cout << "TBD" << endl;
         }


};
#endif