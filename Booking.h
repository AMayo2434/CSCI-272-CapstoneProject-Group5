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

            while (true) {
                cin >> guestPortalChoice;
                if (guestPortalChoice == 1) {
                    displayUserInfo(g);
                }
                else if (guestPortalChoice == 2) {
                    displayCurrentBooking(g);
                }
                else if (guestPortalChoice == 3) {
                    createNewBooking(g);
                }
                else if (guestPortalChoice == 4) {
                    editProfile();
                }
                else if (guestPortalChoice == 5) {
                    centerText("Thank you for using Express Guest Portal", 50, '-');
                    break;
                }
                else {
                    cout << "Invalid choice. Please try again." << endl;
                }
            }
        
        
        }


        void displayUserInfo(Guest &g) {
            centerText("USER PROFILE", 50, '=');
            
            // Ensure guest info is loaded
            if (g.getregistrantName().empty()) {
                g.loadGuestInfo();
            }
            
            cout << "Name: " << g.getregistrantName() << endl;
            cout << "Customer ID: " << g.getCustomerID() << endl;
            cout << "Email Address: " << g.getemailAddress() << endl;
            cout << "Phone Number: " << g.getphoneNumber() << endl;
            cout << "Country/Region: " << g.getcountryOrRegion() << endl;
            cout << "ZIP Code: " << g.getzipCode() << endl;
            
            centerText("=", 50, '=');
            cout << endl;
         }


        void displayCurrentBooking(Guest &g) {
            
            centerText("Current Booking", 30, '-');
            
            // Read guest's booking info from CSV
            std::ifstream myFile("GuestInformation.csv");
            
            if (myFile.is_open()) {
                std::string line;
                bool found = false;
                
                while (std::getline(myFile, line)) {
                    std::stringstream ss(line);
                    std::string custID, name, email, phone, country, zip, bookDate, checkIn, checkOut, roomT, roomId;
                    
                    // Parse CSV: CustomerID,RegistrantName,Email,PhoneNumber,Country/Region,ZIP,BookingDate,Check-inDate,Check-out Date,RoomType,RoomID
                    if (std::getline(ss, custID, ',') &&
                        std::getline(ss, name, ',') &&
                        std::getline(ss, email, ',') &&
                        std::getline(ss, phone, ',') &&
                        std::getline(ss, country, ',') &&
                        std::getline(ss, zip, ',') &&
                        std::getline(ss, bookDate, ',') &&
                        std::getline(ss, checkIn, ',') &&
                        std::getline(ss, checkOut, ',') &&
                        std::getline(ss, roomT, ',') &&
                        std::getline(ss, roomId, ',')) {
                        
                        custID.erase(0, custID.find_first_not_of(" \t"));
                        custID.erase(custID.find_last_not_of(" \t") + 1);
                        
                        if (custID == g.getCustomerID()) {
                            roomT.erase(0, roomT.find_first_not_of(" \t"));
                            roomT.erase(roomT.find_last_not_of(" \t") + 1);
                            roomId.erase(0, roomId.find_first_not_of(" \t"));
                            roomId.erase(roomId.find_last_not_of(" \t") + 1);
                            checkIn.erase(0, checkIn.find_first_not_of(" \t"));
                            checkIn.erase(checkIn.find_last_not_of(" \t") + 1);
                            checkOut.erase(0, checkOut.find_first_not_of(" \t"));
                            checkOut.erase(checkOut.find_last_not_of(" \t") + 1);
                            
                            roomType = roomT;
                            roomID = roomId;
                            checkInDate = checkIn;
                            checkOutDate = checkOut;
                            found = true;
                            break;
                        }
                    }
                }
                myFile.close();
                
                if (found) {
                    cout << "Room Type: " << roomType << endl;
                    cout << "Room ID: " << roomID << endl;
                    cout << "Check-in: " << checkInDate << endl;
                    cout << "Check-out: " << checkOutDate << endl;
                    cout << "Nights: " << numNights << endl;
                } else {
                    cout << "No booking found for this guest." << endl;
                }
            } else {
                std::cerr << "Unable to open GuestInformation.csv" << std::endl;
            }
            centerText("=", 30, '=');

        }
         
      
        //Booking Date, Check-in Date, Check-out Date, Room Type, RoomId
        void createNewBooking(Guest &g) {
            centerText("NEW BOOKING", 20, '=');
            
            std::string date = assignDate();
            cout << "Booking Date: " << date << endl;

            cout << "Please enter a check in date (MM/DD/YYYY): ";
            cin >> checkInDate;
            clearError();

            cout << "How many nights are you staying?: ";
            cin >> numNights;
            clearError();

            checkOutDate = determineDate(numNights);

            cout << "Enter the room type (Standard, Deluxe, Presidential Suite): ";
            cin >> roomType;
            clearError();
            
            if (roomType == "Standard") {
                if (isRoomAvailable("Standard")) {
                    selectAvailableRoom("Standard");
                    // Update CSV with booking info: CustomerID, RegistrantName, Email, PhoneNumber, Country/Region, ZIP, BookingDate, Check-inDate, Check-out Date, RoomType, RoomID
                    updateGuestBooking(g, date, checkInDate, checkOutDate, roomType, roomID);
                    cout << "Booking created successfully!" << endl;
                } else {
                    cout << "Sorry, no Standard rooms are available." << endl;
                }
            }
            else if (roomType == "Deluxe") {
                if (isRoomAvailable("Deluxe")) {
                    selectAvailableRoom("Deluxe");
                    updateGuestBooking(g, date, checkInDate, checkOutDate, roomType, roomID);
                    cout << "Booking created successfully!" << endl;
                } else {
                    cout << "Sorry, no Deluxe rooms are available." << endl;
                }
            }
            else if (roomType == "Presidential Suite") {
                if (isRoomAvailable("Presidential Suite")) {
                    selectAvailableRoom("Presidential Suite");
                    updateGuestBooking(g, date, checkInDate, checkOutDate, roomType, roomID);
                    cout << "Booking created successfully!" << endl;
                } else {
                    cout << "Sorry, no Presidential Suite rooms are available." << endl;
                }
            }
            else {
                cout << "Invalid room type. Please try again." << endl;
            }
             
             
             
         }
         
        void editProfile() {
            centerText("EDIT PROFILE", 30, '=');
            cout << "Profile editing is currently under development." << endl;
            cout << "Please contact front desk for profile modifications." << endl;
            centerText("=", 30, '=');
        }

        // Check if a room type is available in the Rooms.csv file
        bool isRoomAvailable(const string& roomType) {
            std::ifstream myFile("Rooms.csv");
            
            if (!myFile.is_open()) {
                std::cerr << "Unable to open Rooms.csv" << std::endl;
                return false;
            }
            
            std::string line;
            bool headerSkipped = false;
            
            while (std::getline(myFile, line)) {
                if (!headerSkipped) {
                    headerSkipped = true;
                    continue;  // Skip header line
                }
                
                std::stringstream ss(line);
                std::string roomID, type, availability, customerID;
                
                // Parse CSV: RoomID, RoomType, Availability, Customer ID
                if (std::getline(ss, roomID, ',') &&
                    std::getline(ss, type, ',') &&
                    std::getline(ss, availability, ',')) {
                    
                    // Trim whitespace from type and availability
                    type.erase(0, type.find_first_not_of(" \t"));
                    type.erase(type.find_last_not_of(" \t") + 1);
                    availability.erase(0, availability.find_first_not_of(" \t"));
                    availability.erase(availability.find_last_not_of(" \t") + 1);
                    
                    if (type == roomType && availability == "True") {
                        myFile.close();
                        return true;
                    }
                }
            }
            
            myFile.close();
            return false;
        }

        // Find and assign an available room of the specified type
        void selectAvailableRoom(const string& roomType) {
            std::ifstream myFile("Rooms.csv");
            std::vector<std::string> lines;
            std::string line;
            bool found = false;
            
            if (!myFile.is_open()) {
                std::cerr << "Unable to open Rooms.csv" << std::endl;
                return;
            }
            
            int lineNum = 0;
            while (std::getline(myFile, line)) {
                lines.push_back(line);
                
                if (lineNum > 0) {  // Skip header
                    std::stringstream ss(line);
                    std::string rID, type, availability;
                    
                    if (std::getline(ss, rID, ',') &&
                        std::getline(ss, type, ',') &&
                        std::getline(ss, availability, ',')) {
                        
                        rID.erase(0, rID.find_first_not_of(" \t"));
                        rID.erase(rID.find_last_not_of(" \t") + 1);
                        type.erase(0, type.find_first_not_of(" \t"));
                        type.erase(type.find_last_not_of(" \t") + 1);
                        availability.erase(0, availability.find_first_not_of(" \t"));
                        availability.erase(availability.find_last_not_of(" \t") + 1);
                        
                        if (type == roomType && availability == "True" && !found) {
                            roomID = rID;
                            // Mark this room as unavailable (for future CSV update)
                            found = true;
                            cout << "Room assigned: " << roomID << " (" << roomType << ")" << endl;
                        }
                    }
                }
                lineNum++;
            }
            myFile.close();
        }

        // Helper function to update guest booking in GuestInformation.csv
        void updateGuestBooking(Guest &g, const string &bookingDate, const string &checkIn, const string &checkOut, const string &type, const string &room) {
            std::ifstream inFile("GuestInformation.csv");
            std::ofstream outFile("GuestInformation.csv.tmp");
            std::string line;
            bool updated = false;
            
            if (!inFile.is_open() || !outFile.is_open()) {
                std::cerr << "Error opening GuestInformation.csv" << std::endl;
                return;
            }
            
            while (std::getline(inFile, line)) {
                std::stringstream ss(line);
                std::string custID;
                
                if (std::getline(ss, custID, ',')) {
                    custID.erase(0, custID.find_first_not_of(" \t"));
                    custID.erase(custID.find_last_not_of(" \t") + 1);
                    
                    if (custID == g.getCustomerID()) {
                        // Update this row with booking info
                        // Format: CustomerID,RegistrantName,Email,PhoneNumber,Country/Region,ZIP,BookingDate,Check-inDate,Check-out Date,RoomType,RoomID
                        outFile << g.getCustomerID() << ","
                                << g.getregistrantName() << ","
                                << g.getemailAddress() << ","
                                << g.getphoneNumber() << ","
                                << g.getcountryOrRegion() << ","
                                << g.getzipCode() << ","
                                << bookingDate << ","
                                << checkIn << ","
                                << checkOut << ","
                                << type << ","
                                << room << std::endl;
                        updated = true;
                    } else {
                        outFile << line << std::endl;
                    }
                } else {
                    outFile << line << std::endl;
                }
            }
            
            inFile.close();
            outFile.close();
            
            // Replace original file with updated file
            std::remove("GuestInformation.csv");
            std::rename("GuestInformation.csv.tmp", "GuestInformation.csv");
        }

//Destructor to clear entry at the end of the program
         ~Booking() {};
};
#endif