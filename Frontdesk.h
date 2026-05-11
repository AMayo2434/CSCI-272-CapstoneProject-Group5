#ifndef FRONTDESK_H
#define FRONTDESK_H

#include "Booking.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>

using namespace std;

class FrontDesk {
    private:
    string roomTypes;
    vector<string> guestHistory;
    
    public:
    
    // Add guest
    void addGuest(string guestName){
        guestHistory.push_back(guestName);
        ofstream outFile("Guests.csv", ios::app);
        if (outFile.is_open()){
            outFile << guestName << endl;

            outFile.close();
        }
    cout << guestName
             << " added successfully.\n";  
    }
    
    // Remove Guest 
    void removeGuest(string guestName) {
        for (int i = 0; i < guestHistory.size(); i++) {

            if (guestHistory[i] == guestName) {

                guestHistory.erase(
                    guestHistory.begin() + i
                );

                cout << guestName
                     << " removed successfully.\n";

                return;
            }
        }

        cout << "Guest not found.\n";
    }

    // Check In
    void checkIn(string guestName) {

        cout << guestName
             << " checked in successfully.\n";
    }

    // Check Out
    void checkOut(string guestName) {

        cout << guestName
             << " checked out successfully.\n";
    }

    // Search Guest
    void searchGuest(string guestName) {

        for (string guest : guestHistory) {

            if (guest == guestName) {

                cout << "Guest Found: "
                     << guestName << endl;

                return;
            }
        }

        cout << "Guest not found.\n";
    }

    // Display Guest History
    void displayGuestHistory() {

        cout << "\n--- Guest History ---\n";

        for (string guest : guestHistory) {

            cout << guest << endl;
        }
    }

    // Access Billing
    void accessBilling() {

        cout << "Opening Billing System...\n";
    }

    // Access Room Management
    void accessRoomManagement() {

        cout << "Opening Room Management...\n";
    }

// ADMIN ACCOUNT ONLY (IF WE HAVE TIME TO IMPLEMENT)

    // Add Employee
    void addEmployee(string employeeName) {

        cout << employeeName
             << " added as employee.\n";
    }

    // Remove Employee
    void removeEmployee(string employeeName) {

        cout << employeeName
             << " removed from employees.\n";
    }
};
#endif