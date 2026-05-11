#ifndef ROOMMANAGEMENT_H
#define ROOMMANAGEMENT_H

#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <cstdio>
#include <stdexcept>
#include "Room.h"

using namespace std;

/*
    RoomManagement class

    Manages all hotel rooms by:
    - Loading room data from Rooms.csv
    - Displaying available and all rooms
    - Updating room availability in Rooms.csv
    - Linking guest assignments to GuestInformation.csv
    - Providing search and capacity tracking

    This class is used by the FrontDesk class to perform
    employee-level room operations.
*/

class RoomManagement {

private:
    int totalRooms;
    int availableRooms;

    vector<Room*> rooms;

    // -------------------------------------------------------
    // PRIVATE HELPER: trimWhitespace
    // Removes leading and trailing spaces/tabs from a string.
    // Used when reading CSV fields to ensure clean comparisons.
    // -------------------------------------------------------
    string trimWhitespace(const string& str) {
        size_t start = str.find_first_not_of(" \t\r\n");
        size_t end   = str.find_last_not_of(" \t\r\n");
        if (start == string::npos) return "";
        return str.substr(start, end - start + 1);
    }

    // -------------------------------------------------------
    // PRIVATE HELPER: roomTypeToPrice
    // Returns a default price per night based on room type.
    // Standard           : $100/night
    // Deluxe             : $200/night
    // Presidential Suite : $500/night
    // -------------------------------------------------------
    double roomTypeToPrice(const string& type) {
        if (type == "Standard")           return 100.0;
        if (type == "Deluxe")             return 200.0;
        if (type == "Presidential Suite") return 500.0;
        return 100.0;
    }

    // -------------------------------------------------------
    // PRIVATE HELPER: roomTypeToCapacity
    // Returns default guest capacity based on room type.
    // Standard           : 2 guests
    // Deluxe             : 3 guests
    // Presidential Suite : 4 guests
    // -------------------------------------------------------
    int roomTypeToCapacity(const string& type) {
        if (type == "Standard")           return 2;
        if (type == "Deluxe")             return 3;
        if (type == "Presidential Suite") return 4;
        return 2;
    }

    // -------------------------------------------------------
    // PRIVATE HELPER: printTableHeader
    // Prints the iomanip-formatted column header row used by
    // displayAllRooms() and displayAvailableRooms().
    // Keeps header and data rows in the same column widths.
    // -------------------------------------------------------
    void printTableHeader() const {
        cout << "\n  "
             << left << setw(8)  << "Room ID"
             << left << setw(22) << "Type"
             << left << setw(12) << "Status"
             << left << setw(12) << "Price/Night"
             << left << setw(10) << "Capacity"
             << "Guest ID\n";
        cout << "  " << setfill('-') << setw(72) << "" << setfill(' ') << "\n";
    }

    // -------------------------------------------------------
    // PRIVATE HELPER: printTableRow
    // Prints one iomanip-formatted room row for the table views.
    // Uses the Room operator<< for the first four columns, then
    // appends capacity and guest columns manually.
    // -------------------------------------------------------
    void printTableRow(const Room* r) const {
        // operator<< handles RoomID, Type, Status, Price columns
        cout << "  " << *r
             << "  " << left << setw(8) << r->getCurrentGuest()
             << "\n";
    }

public:

    // -------------------------------------------------------
    // Default Constructor
    // Initializes counters and loads all rooms from Rooms.csv.
    // -------------------------------------------------------
    RoomManagement() : totalRooms(0), availableRooms(0) {
        loadRoomsFromCSV();
    }

    // -------------------------------------------------------
    // Destructor
    // Frees all dynamically allocated Room objects stored
    // in the rooms vector to prevent memory leaks.
    // -------------------------------------------------------
    ~RoomManagement() {
        for (Room* r : rooms) {
            delete r;
        }
        rooms.clear();
    }

    // -------------------------------------------------------
    // loadRoomsFromCSV()
    //
    // Reads Rooms.csv and populates the rooms vector with
    // the correct derived Room type for each row.
    //
    // Expected CSV format:
    // RoomID, RoomType, Availability, Customer ID
    //
    // Availability "True" = available, anything else = occupied.
    // Customer ID "0" means no guest currently assigned.
    //
    // This function is called automatically in the constructor
    // so FrontDesk always has fresh data on creation.
    //
    // EXCEPTION HANDLING :
    // Throws runtime_error if Rooms.csv cannot be opened,
    // allowing the caller to display a message and continue.
    // -------------------------------------------------------
    void loadRoomsFromCSV() {
        // Clear any previously loaded rooms before reloading
        for (Room* r : rooms) delete r;
        rooms.clear();
        totalRooms     = 0;
        availableRooms = 0;

        ifstream myFile("Rooms.csv");

        // EXCEPTION HANDLING: throw if the file cannot be opened
        if (!myFile.is_open()) {
            throw runtime_error("Unable to open Rooms.csv — check that the file exists.");
        }

        string line;
        bool headerSkipped = false;

        while (getline(myFile, line)) {
            // Skip the header row
            if (!headerSkipped) {
                headerSkipped = true;
                continue;
            }

            if (line.empty()) continue;

            stringstream ss(line);
            string rID, rType, rAvail, rCustID;

            // Parse: RoomID, RoomType, Availability, Customer ID
            if (!getline(ss, rID,    ',')) continue;
            if (!getline(ss, rType,  ',')) continue;
            if (!getline(ss, rAvail, ',')) continue;
            getline(ss, rCustID); // Customer ID may be absent on some rows

            // Trim all parsed fields
            rID     = trimWhitespace(rID);
            rType   = trimWhitespace(rType);
            rAvail  = trimWhitespace(rAvail);
            rCustID = trimWhitespace(rCustID);

            if (rID.empty() || rType.empty()) continue;

            // Convert RoomID string to an integer room number.
            // If purely numeric (e.g. "101"), convert directly.
            // If prefixed with a letter (e.g. "R101"), strip it first.
            int roomNum = 0;
            try {
                size_t pos = rID.find_first_of("0123456789");
                roomNum = (pos != string::npos) ? stoi(rID.substr(pos)) : 0;
            } catch (...) { roomNum = 0; }

            bool   available = (rAvail == "True");
            double price     = roomTypeToPrice(rType);
            int    cap       = roomTypeToCapacity(rType);

            // Instantiate the correct derived class based on room type.
            // This uses polymorphism — FrontDesk can call virtual functions
            // on any Room pointer without knowing the exact subtype.
            Room* newRoom = nullptr;

            if (rType == "Standard") {
                newRoom = new StandardRoom(rID, roomNum, price, available, cap);
            }
            else if (rType == "Deluxe") {
                // Service fee for deluxe rooms: $50 flat
                newRoom = new DeluxeRoom(rID, roomNum, price, available, cap, 50.0);
            }
            else if (rType == "Presidential Suite") {
                // Premium fee for presidential suites: $200 flat
                newRoom = new PresidentialSuiteRoom(rID, roomNum, price, available, cap, 200.0);
            }
            else {
                // Fallback: treat unknown types as base StandardRoom
                newRoom = new StandardRoom(rID, roomNum, price, available, cap);
            }

            // Set current guest if one is assigned (Customer ID != "0")
            if (!rCustID.empty() && rCustID != "0") {
                newRoom->setCurrentGuest(rCustID);
            }

            rooms.push_back(newRoom);
            totalRooms++;
            if (available) availableRooms++;
        }

        myFile.close();
    }

    // -------------------------------------------------------
    // addRoom()
    //
    // Adds a Room pointer to the in-memory rooms vector.
    //
    // This allows FrontDesk or other employee functions to
    // dynamically insert new room objects at runtime.
    // Note: This does NOT write to Rooms.csv automatically.
    //       Call saveRoomsToCSV() after adding if persistence needed.
    // -------------------------------------------------------
    void addRoom(Room* room) {
        if (room == nullptr) {
            cerr << "Error: Cannot add a null room." << endl;
            return;
        }
        rooms.push_back(room);
        totalRooms++;
        if (room->getAvailability()) availableRooms++;
        cout << "Room " << room->getRoomID() << " added to management." << endl;
    }

    // -------------------------------------------------------
    // displayAvailableRooms()
    //
    // Iterates through the rooms vector and prints details
    // for every room where isAvailable == true.
    //
    // Uses iomanip table formatting via printTableHeader()
    // and the Room operator<< for each row.
    //
    // FrontDesk uses this to show employees what rooms are open.
    // -------------------------------------------------------
    void displayAvailableRooms() const {
        cout << "\n" << setfill('=') << setw(74) << "" << setfill(' ') << "\n";
        cout << "  AVAILABLE ROOMS\n";
        cout << setfill('=') << setw(74) << "" << setfill(' ') << "\n";

        printTableHeader();

        bool anyFound = false;
        for (const Room* r : rooms) {
            if (r->getAvailability()) {
                printTableRow(r);
                anyFound = true;
            }
        }
        if (!anyFound) {
            cout << "  No available rooms at this time.\n";
        }
        cout << "\n";
    }

    // -------------------------------------------------------
    // displayAllRooms()
    //
    // Iterates through ALL rooms in the vector regardless
    // of availability and displays them in a formatted table.
    //
    // Uses iomanip table formatting via printTableHeader()
    // and the Room operator<< for each row.
    //
    // FrontDesk uses this for a full hotel room overview.
    // -------------------------------------------------------
    void displayAllRooms() const {
        cout << "\n" << setfill('=') << setw(74) << "" << setfill(' ') << "\n";
        cout << "  ALL ROOMS\n";
        cout << setfill('=') << setw(74) << "" << setfill(' ') << "\n";

        if (rooms.empty()) {
            cout << "  No rooms loaded. Check Rooms.csv.\n";
            return;
        }

        printTableHeader();

        for (const Room* r : rooms) {
            printTableRow(r);
        }
        cout << "\n";
    }

    // -------------------------------------------------------
    // updateRoomAvailability()
    //
    // Updates a room's availability both in memory and in
    // Rooms.csv so changes persist across sessions.
    //
    // Parameters:
    //   roomID - the ID string matching the RoomID column
    //   status - true = available, false = occupied
    //
    // FrontDesk calls this during check-in and check-out.
    //
    // EXCEPTION HANDLING :
    // Throws runtime_error if either file cannot be opened.
    // -------------------------------------------------------
    void updateRoomAvailability(string roomID, bool status) {
        bool found = false;

        // 1. Update in-memory room object
        for (Room* r : rooms) {
            if (r->getRoomID() == roomID) {
                r->setAvailability(status);
                found = true;
                if (status) availableRooms++;
                else        availableRooms--;
                break;
            }
        }

        if (!found) {
            cout << "Room ID " << roomID << " not found in memory.\n";
            return;
        }

        // 2. Persist the change to Rooms.csv
        ifstream inFile("Rooms.csv");
        if (!inFile.is_open()) {
            throw runtime_error("Unable to open Rooms.csv for availability update.");
        }

        vector<string> lines;
        string line;
        while (getline(inFile, line)) lines.push_back(line);
        inFile.close();

        ofstream tempFile("Rooms.csv.tmp");
        if (!tempFile.is_open()) {
            throw runtime_error("Unable to create temp file for Rooms.csv update.");
        }

        // Write header unchanged
        if (!lines.empty()) tempFile << lines[0] << "\n";

        for (size_t i = 1; i < lines.size(); ++i) {
            stringstream ss(lines[i]);
            string rID, rType, rAvail, rCust;

            if (getline(ss, rID,    ',') &&
                getline(ss, rType,  ',') &&
                getline(ss, rAvail, ',')) {
                getline(ss, rCust); // optional 4th column

                if (trimWhitespace(rID) == roomID) {
                    // Rewrite the row with updated availability field
                    tempFile << rID << "," << rType << ","
                             << (status ? " True" : " False") << ","
                             << rCust << "\n";
                } else {
                    tempFile << lines[i] << "\n";
                }
            } else {
                tempFile << lines[i] << "\n";
            }
        }

        tempFile.close();
        remove("Rooms.csv");
        rename("Rooms.csv.tmp", "Rooms.csv");

        cout << "Room " << roomID << " availability updated to "
             << (status ? "Available" : "Occupied") << ".\n";
    }

    // -------------------------------------------------------
    // assignGuestToRoom()
    //
    // Assigns a customer ID to a specific room in Rooms.csv
    // and marks the room as unavailable (False).
    //
    // Called by FrontDesk during check-in or manual booking.
    //
    // Parameters:
    //   roomID     - the RoomID to update
    //   customerID - the guest's CustomerID string
    //
    // EXCEPTION HANDLING :
    // Throws runtime_error if Rooms.csv or temp file cannot open.
    // -------------------------------------------------------
    void assignGuestToRoom(const string& roomID, const string& customerID) {
        // Update in-memory
        for (Room* r : rooms) {
            if (r->getRoomID() == roomID) {
                r->setCurrentGuest(customerID);
                r->setAvailability(false);
                availableRooms--;
                break;
            }
        }

        // Persist to Rooms.csv
        ifstream inFile("Rooms.csv");
        if (!inFile.is_open()) {
            throw runtime_error("Cannot open Rooms.csv for guest assignment.");
        }

        vector<string> lines;
        string line;
        while (getline(inFile, line)) lines.push_back(line);
        inFile.close();

        ofstream tempFile("Rooms.csv.tmp");
        if (!tempFile.is_open()) {
            throw runtime_error("Cannot create temp file for Rooms.csv.");
        }

        if (!lines.empty()) tempFile << lines[0] << "\n";

        for (size_t i = 1; i < lines.size(); ++i) {
            stringstream ss(lines[i]);
            string rID, rType, rAvail, rCust;

            if (getline(ss, rID,    ',') &&
                getline(ss, rType,  ',') &&
                getline(ss, rAvail, ',')) {
                getline(ss, rCust);

                if (trimWhitespace(rID) == roomID) {
                    tempFile << rID << "," << rType
                             << ", False, " << customerID << "\n";
                } else {
                    tempFile << lines[i] << "\n";
                }
            } else {
                tempFile << lines[i] << "\n";
            }
        }

        tempFile.close();
        remove("Rooms.csv");
        rename("Rooms.csv.tmp", "Rooms.csv");

        cout << "Room " << roomID << " assigned to Customer "
             << customerID << ".\n";
    }

    // -------------------------------------------------------
    // releaseRoom()
    //
    // Clears the guest assignment on a room in Rooms.csv
    // and marks it as available (True) again.
    //
    // Called by FrontDesk during guest check-out.
    //
    // Parameters:
    //   roomID - the RoomID to release
    //
    // EXCEPTION HANDLING :
    // Throws runtime_error if file operations fail.
    // -------------------------------------------------------
    void releaseRoom(const string& roomID) {
        // Update in-memory
        for (Room* r : rooms) {
            if (r->getRoomID() == roomID) {
                r->setCurrentGuest("");
                r->setAvailability(true);
                availableRooms++;
                break;
            }
        }

        // Persist to Rooms.csv — reset Availability to True, Customer ID to 0
        ifstream inFile("Rooms.csv");
        if (!inFile.is_open()) {
            throw runtime_error("Cannot open Rooms.csv for room release.");
        }

        vector<string> lines;
        string line;
        while (getline(inFile, line)) lines.push_back(line);
        inFile.close();

        ofstream tempFile("Rooms.csv.tmp");
        if (!tempFile.is_open()) {
            throw runtime_error("Cannot create temp file for Rooms.csv release.");
        }

        if (!lines.empty()) tempFile << lines[0] << "\n";

        for (size_t i = 1; i < lines.size(); ++i) {
            stringstream ss(lines[i]);
            string rID, rType, rAvail, rCust;

            if (getline(ss, rID,    ',') &&
                getline(ss, rType,  ',') &&
                getline(ss, rAvail, ',')) {
                getline(ss, rCust);

                if (trimWhitespace(rID) == roomID) {
                    tempFile << rID << "," << rType << ", True, 0\n";
                } else {
                    tempFile << lines[i] << "\n";
                }
            } else {
                tempFile << lines[i] << "\n";
            }
        }

        tempFile.close();
        remove("Rooms.csv");
        rename("Rooms.csv.tmp", "Rooms.csv");

        cout << "Room " << roomID << " released — now available.\n";
    }

    // -------------------------------------------------------
    // checkCapacity()
    //
    // Returns true if there is at least one available room,
    // false if the hotel is at full capacity.
    //
    // FrontDesk can check this before attempting a new booking.
    // -------------------------------------------------------
    bool checkCapacity() const {
        return availableRooms > 0;
    }

    // -------------------------------------------------------
    // displayHotelCapacity()
    //
    // Prints a formatted summary of total vs. available rooms
    // using iomanip setw for label/value column alignment.
    // Useful for front desk situational awareness.
    // -------------------------------------------------------
    void displayHotelCapacity() const {
        cout << "\n" << setfill('=') << setw(40) << "" << setfill(' ') << "\n";
        cout << "  HOTEL CAPACITY SUMMARY\n";
        cout << setfill('=') << setw(40) << "" << setfill(' ') << "\n";
        cout << "  " << left << setw(20) << "Total Rooms"
             << ": " << totalRooms << "\n";
        cout << "  " << left << setw(20) << "Available Rooms"
             << ": " << availableRooms << "\n";
        cout << "  " << left << setw(20) << "Occupied Rooms"
             << ": " << (totalRooms - availableRooms) << "\n";
        cout << setfill('=') << setw(40) << "" << setfill(' ') << "\n\n";
    }

    // -------------------------------------------------------
    // displayRoomHistory()
    //
    // Shows all currently occupied rooms with their assigned
    // Customer IDs in a formatted iomanip table.
    //
    // FrontDesk uses this to review active guest assignments.
    // -------------------------------------------------------
    void displayRoomHistory() const {
        cout << "\n" << setfill('=') << setw(60) << "" << setfill(' ') << "\n";
        cout << "  OCCUPIED ROOMS\n";
        cout << setfill('=') << setw(60) << "" << setfill(' ') << "\n";
        cout << "  " << left << setw(10) << "Room ID"
                     << setw(22) << "Type"
                     << "Guest (Customer ID)\n";
        cout << "  " << setfill('-') << setw(58) << "" << setfill(' ') << "\n";

        bool anyOccupied = false;
        for (const Room* r : rooms) {
            if (!r->getAvailability()) {
                cout << "  " << left
                     << setw(10) << r->getRoomID()
                     << setw(22) << r->getRoomType()
                     << r->getCurrentGuest() << "\n";
                anyOccupied = true;
            }
        }
        if (!anyOccupied) {
            cout << "  No rooms are currently occupied.\n";
        }
        cout << setfill('=') << setw(60) << "" << setfill(' ') << "\n\n";
    }

    // -------------------------------------------------------
    // searchRoomByID()
    //
    // Searches the in-memory rooms vector for a room matching
    // the given roomID string.
    //
    // Returns:
    //   A pointer to the matching Room object, or nullptr if not found.
    //
    // FrontDesk uses this to look up a specific room quickly
    // without scanning the entire CSV file.
    // -------------------------------------------------------
    Room* searchRoomByID(string roomID) const {
        for (Room* r : rooms) {
            if (r->getRoomID() == roomID) {
                return r;
            }
        }
        cout << "Room ID " << roomID << " not found.\n";
        return nullptr;
    }

    // -------------------------------------------------------
    // getRoomsForType()
    //
    // Returns a vector of Room pointers filtered by room type.
    // Used by FrontDesk to list all Standard, Deluxe, or
    // Presidential Suite rooms at once.
    //
    // Parameters:
    //   type - "Standard", "Deluxe", or "Presidential Suite"
    // -------------------------------------------------------
    vector<Room*> getRoomsForType(const string& type) const {
        vector<Room*> result;
        for (Room* r : rooms) {
            if (r->getRoomType() == type) {
                result.push_back(r);
            }
        }
        return result;
    }

    // -------------------------------------------------------
    // getTotalRooms() / getAvailableRooms()
    //
    // Simple getters so FrontDesk can read counts directly
    // without accessing private member variables.
    // -------------------------------------------------------
    int getTotalRooms()     const { return totalRooms;     }
    int getAvailableRooms() const { return availableRooms; }
};

#endif
