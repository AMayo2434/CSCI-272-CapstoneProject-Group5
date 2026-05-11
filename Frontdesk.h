#ifndef FRONTDESK_H
#define FRONTDESK_H

#include "Booking.h"
#include "RoomManagement.h"
#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <stdexcept>
#include <limits>

using namespace std;

/*
    FrontDesk class

    The FrontDesk class is the primary employee-facing interface
    for managing guests and rooms.

    It has access to:
    - GuestInformation.csv  (via helper functions in functions.h)
    - Rooms.csv             (via the RoomManagement class)

    All room-related operations are delegated to RoomManagement
    so that CSV data stays in sync with in-memory state.

    Guest operations (add, remove, search, check-in, check-out)
    work against GuestInformation.csv directly.
*/

class FrontDesk {
    private:
        string roomTypes;
        vector<string> guestHistory;

        // RoomManagement instance — loads Rooms.csv on construction.
        // All room reads and writes go through this object.
        RoomManagement roomManager;

        // -------------------------------------------------------
        // PRIVATE HELPER: trimWhitespace
        // Removes leading/trailing whitespace from CSV fields.
        // -------------------------------------------------------
        string trimWhitespace(const string& str) {
            size_t start = str.find_first_not_of(" \t\r\n");
            size_t end   = str.find_last_not_of(" \t\r\n");
            if (start == string::npos) return "";
            return str.substr(start, end - start + 1);
        }

        // -------------------------------------------------------
        // PRIVATE HELPER: printSectionHeader
        // Prints a formatted iomanip section header with setfill,
        // setw, and a centered title for consistent visual style.
        // -------------------------------------------------------
        void printSectionHeader(const string& title) const {
            cout << "\n" << setfill('=') << setw(60) << "" << setfill(' ') << "\n";
            // Center the title within 60 characters
            int padding = (60 - (int)title.length()) / 2;
            cout << setw(padding) << "" << title << "\n";
            cout << setfill('=') << setw(60) << "" << setfill(' ') << "\n";
        }

        // -------------------------------------------------------
        // PRIVATE HELPER: printGuestRow
        // Prints one iomanip-formatted guest row for the guest table.
        // Columns: CustomerID | Name | Email | Phone | Country | ZIP
        // -------------------------------------------------------
        void printGuestRow(const string& id,   const string& name,
                           const string& email, const string& phone,
                           const string& country, const string& zip) const {
            cout << "  " << left
                 << setw(14) << id
                 << setw(20) << name
                 << setw(26) << email
                 << setw(14) << phone
                 << setw(16) << country
                 << zip << "\n";
        }

    public:

    // -------------------------------------------------------
    // addGuest()
    //
    // Adds a guest name to the in-memory guestHistory vector
    // and appends the name to Guests.csv for persistence.
    //
    // EXCEPTION HANDLING :
    // Throws runtime_error if Guests.csv cannot be opened.
    // -------------------------------------------------------
    void addGuest(string guestName) {
        guestHistory.push_back(guestName);

        ofstream outFile("Guests.csv", ios::app);
        if (!outFile.is_open()) {
            throw runtime_error("Unable to open Guests.csv to add guest.");
        }
        outFile << guestName << "\n";
        outFile.close();

        cout << "\n  " << guestName << " added successfully.\n";
    }

    // -------------------------------------------------------
    // removeGuest()
    //
    // Removes a guest by name from the in-memory guestHistory.
    // Note: does not modify GuestInformation.csv — use
    // deleteGuestByID() for CSV-level removal.
    // -------------------------------------------------------
    void removeGuest(string guestName) {
        for (int i = 0; i < (int)guestHistory.size(); i++) {
            if (guestHistory[i] == guestName) {
                guestHistory.erase(guestHistory.begin() + i);
                cout << "\n  " << guestName << " removed successfully.\n";
                return;
            }
        }
        cout << "\n  Guest not found.\n";
    }

    // -------------------------------------------------------
    // checkIn()
    //
    // Marks a guest as checked in.
    // Also updates the room assigned to this guest in Rooms.csv
    // via RoomManagement::assignGuestToRoom().
    //
    // Parameters:
    //   guestName  - display name for confirmation message
    //   customerID - CustomerID matching GuestInformation.csv
    //   roomID     - RoomID matching Rooms.csv
    //
    // EXCEPTION HANDLING :
    // Catches and displays any runtime_error from RoomManagement
    // so a CSV failure does not crash the portal loop.
    // -------------------------------------------------------
    void checkIn(string guestName, string customerID = "", string roomID = "") {
        cout << "\n  " << guestName << " checked in successfully.\n";

        // If both customerID and roomID are provided, persist
        // the assignment to Rooms.csv via RoomManagement
        if (!customerID.empty() && !roomID.empty()) {
            try {
                roomManager.assignGuestToRoom(roomID, customerID);
            } catch (const runtime_error& e) {
                cerr << "  Check-in error: " << e.what() << "\n";
            }
        }
    }

    // -------------------------------------------------------
    // checkOut()
    //
    // Marks a guest as checked out and releases their room
    // back to available status in Rooms.csv.
    //
    // Parameters:
    //   guestName  - display name for confirmation message
    //   roomID     - RoomID to release in Rooms.csv (optional)
    //
    // EXCEPTION HANDLING :
    // Catches and displays any runtime_error from RoomManagement
    // so a CSV failure does not crash the portal loop.
    // -------------------------------------------------------
    void checkOut(string guestName, string roomID = "") {
        cout << "\n  " << guestName << " checked out successfully.\n";

        if (!roomID.empty()) {
            try {
                roomManager.releaseRoom(roomID);
            } catch (const runtime_error& e) {
                cerr << "  Check-out error: " << e.what() << "\n";
            }
        }
    }

    // -------------------------------------------------------
    // searchGuest()
    //
    // Searches the in-memory guestHistory vector by name.
    // For a full CSV search, use searchGuestByID().
    // -------------------------------------------------------
    void searchGuest(string guestName) {
        for (string guest : guestHistory) {
            if (guest == guestName) {
                cout << "\n  Guest Found: " << guestName << "\n";
                return;
            }
        }
        cout << "\n  Guest not found.\n";
    }

    // -------------------------------------------------------
    // searchGuestByID()
    //
    // Searches GuestInformation.csv for a customer by ID and
    // prints all fields in an iomanip-formatted guest card.
    //
    // Parameters:
    //   customerID - the CustomerID string to look up
    //
    // EXCEPTION HANDLING :
    // Throws runtime_error if GuestInformation.csv cannot open.
    // -------------------------------------------------------
    void searchGuestByID(const string& customerID) {
        ifstream myFile("GuestInformation.csv");
        if (!myFile.is_open()) {
            throw runtime_error("Cannot open GuestInformation.csv for guest search.");
        }

        string line;
        bool headerSkipped = false;
        bool found = false;

        while (getline(myFile, line)) {
            if (!headerSkipped) { headerSkipped = true; continue; }
            if (line.empty()) continue;

            stringstream ss(line);
            string custID, name, email, phone, country, zip,
                   bookDate, checkIn, checkOut, roomType, roomID;

            if (getline(ss, custID,   ',') &&
                getline(ss, name,     ',') &&
                getline(ss, email,    ',') &&
                getline(ss, phone,    ',') &&
                getline(ss, country,  ',') &&
                getline(ss, zip,      ',')) {

                // Read optional booking fields — they may be empty
                getline(ss, bookDate, ',');
                getline(ss, checkIn,  ',');
                getline(ss, checkOut, ',');
                getline(ss, roomType, ',');
                getline(ss, roomID);

                if (trimWhitespace(custID) == customerID) {
                    // iomanip formatted guest card
                    printSectionHeader("GUEST PROFILE");
                    cout << "  " << left << setw(20) << "Customer ID"
                         << ": " << trimWhitespace(custID)   << "\n";
                    cout << "  " << left << setw(20) << "Name"
                         << ": " << trimWhitespace(name)     << "\n";
                    cout << "  " << left << setw(20) << "Email"
                         << ": " << trimWhitespace(email)    << "\n";
                    cout << "  " << left << setw(20) << "Phone"
                         << ": " << trimWhitespace(phone)    << "\n";
                    cout << "  " << left << setw(20) << "Country/Region"
                         << ": " << trimWhitespace(country)  << "\n";
                    cout << "  " << left << setw(20) << "ZIP Code"
                         << ": " << trimWhitespace(zip)      << "\n";

                    // Only print booking fields if they are not empty
                    if (!trimWhitespace(bookDate).empty()) {
                        cout << "\n  --- Booking Info ---\n";
                        cout << "  " << left << setw(20) << "Booking Date"
                             << ": " << trimWhitespace(bookDate)  << "\n";
                        cout << "  " << left << setw(20) << "Check-In"
                             << ": " << trimWhitespace(checkIn)   << "\n";
                        cout << "  " << left << setw(20) << "Check-Out"
                             << ": " << trimWhitespace(checkOut)  << "\n";
                        cout << "  " << left << setw(20) << "Room Type"
                             << ": " << trimWhitespace(roomType)  << "\n";
                        cout << "  " << left << setw(20) << "Room ID"
                             << ": " << trimWhitespace(roomID)    << "\n";
                    }
                    cout << setfill('=') << setw(60) << "" << setfill(' ') << "\n";
                    found = true;
                    break;
                }
            }
        }
        myFile.close();

        if (!found) {
            cout << "\n  No guest found with Customer ID: " << customerID << "\n";
        }
    }

    // -------------------------------------------------------
    // displayGuestHistory()
    //
    // Prints all names currently in the in-memory guestHistory
    // in a numbered iomanip-formatted list.
    // -------------------------------------------------------
    void displayGuestHistory() {
        printSectionHeader("GUEST HISTORY (Session)");
        if (guestHistory.empty()) {
            cout << "  No guests added this session.\n";
        } else {
            for (int i = 0; i < (int)guestHistory.size(); i++) {
                cout << "  " << right << setw(3) << (i + 1) << ".  "
                     << left  << guestHistory[i] << "\n";
            }
        }
        cout << "\n";
    }

    // -------------------------------------------------------
    // viewAllGuests()
    //
    // Reads GuestInformation.csv and prints every guest in a
    // formatted iomanip table with column headers.
    //
    // EXCEPTION HANDLING :
    // Throws runtime_error if the file cannot be opened.
    // -------------------------------------------------------
    void viewAllGuests() {
        ifstream myFile("GuestInformation.csv");
        if (!myFile.is_open()) {
            throw runtime_error("Cannot open GuestInformation.csv to view guests.");
        }

        printSectionHeader("ALL GUESTS");

        // Column header row
        cout << "  " << left
             << setw(14) << "Customer ID"
             << setw(20) << "Name"
             << setw(26) << "Email"
             << setw(14) << "Phone"
             << setw(16) << "Country"
             << "ZIP\n";
        cout << "  " << setfill('-') << setw(90) << "" << setfill(' ') << "\n";

        string line;
        bool headerSkipped = false;

        while (getline(myFile, line)) {
            // Skip the CSV header row — we already printed our own
            if (!headerSkipped) { headerSkipped = true; continue; }
            if (line.empty()) continue;

            stringstream ss(line);
            string custID, name, email, phone, country, zip;

            if (getline(ss, custID,  ',') &&
                getline(ss, name,    ',') &&
                getline(ss, email,   ',') &&
                getline(ss, phone,   ',') &&
                getline(ss, country, ',') &&
                getline(ss, zip,     ',')) {

                printGuestRow(
                    trimWhitespace(custID),
                    trimWhitespace(name),
                    trimWhitespace(email),
                    trimWhitespace(phone),
                    trimWhitespace(country),
                    trimWhitespace(zip)
                );
            }
        }
        myFile.close();
        cout << "\n";
    }

    // -------------------------------------------------------
    // accessBilling()
    //
    // Placeholder for the Billing subsystem.
    // -------------------------------------------------------
    void accessBilling() {
        printSectionHeader("BILLING SYSTEM");
        cout << "  Billing system is currently under development.\n\n";
    }

    // -------------------------------------------------------
    // accessRoomManagement()
    //
    // Delegates to RoomManagement to display all rooms.
    // This connects FrontDesk to the full room data layer.
    // -------------------------------------------------------
    void accessRoomManagement() {
        roomManager.displayAllRooms();
    }

    // -------------------------------------------------------
    // viewAvailableRooms()
    //
    // Delegates to RoomManagement to show only open rooms.
    // FrontDesk uses this when helping a guest pick a room.
    // -------------------------------------------------------
    void viewAvailableRooms() {
        roomManager.displayAvailableRooms();
    }

    // -------------------------------------------------------
    // viewAvailableRoomsByType()
    //
    // Filters available rooms by type and displays them using
    // iomanip-formatted table output via Room operator<<.
    //
    // Parameters:
    //   type - "Standard", "Deluxe", or "Presidential Suite"
    // -------------------------------------------------------
    void viewAvailableRoomsByType(const string& type) {
        printSectionHeader("AVAILABLE " + type + " ROOMS");

        cout << "  " << left
             << setw(8)  << "Room ID"
             << setw(22) << "Type"
             << setw(12) << "Status"
             << "Price/Night\n";
        cout << "  " << setfill('-') << setw(54) << "" << setfill(' ') << "\n";

        vector<Room*> filtered = roomManager.getRoomsForType(type);
        bool anyAvail = false;

        for (Room* r : filtered) {
            if (r->getAvailability()) {
                // Uses Room operator<< for the core columns
                cout << "  " << *r << "\n";
                anyAvail = true;
            }
        }
        if (!anyAvail) {
            cout << "  No available " << type << " rooms at this time.\n";
        }
        cout << "\n";
    }

    // -------------------------------------------------------
    // viewHotelCapacity()
    //
    // Displays total vs. available room counts.
    // Delegates to RoomManagement::displayHotelCapacity().
    // -------------------------------------------------------
    void viewHotelCapacity() {
        roomManager.displayHotelCapacity();
    }

    // -------------------------------------------------------
    // viewOccupiedRooms()
    //
    // Displays all currently occupied rooms with guest IDs.
    // Delegates to RoomManagement::displayRoomHistory().
    // -------------------------------------------------------
    void viewOccupiedRooms() {
        roomManager.displayRoomHistory();
    }

    // -------------------------------------------------------
    // lookupRoomByID()
    //
    // Finds and displays a single room by its RoomID using
    // the virtual displayDetails() for full iomanip output.
    // Delegates to RoomManagement::searchRoomByID().
    //
    // Parameters:
    //   roomID - the RoomID string to look up (e.g. "101")
    // -------------------------------------------------------
    void lookupRoomByID(const string& roomID) {
        Room* r = roomManager.searchRoomByID(roomID);
        if (r != nullptr) {
            // Calls the correct derived displayDetails() via polymorphism
            r->displayDetails();
        }
    }

    // -------------------------------------------------------
    // updateRoomStatus()
    //
    // Manually sets a room's availability from FrontDesk.
    // Delegates to RoomManagement::updateRoomAvailability().
    //
    // Parameters:
    //   roomID - RoomID to update
    //   status - true = available, false = occupied
    //
    // EXCEPTION HANDLING :
    // Catches runtime_error from RoomManagement and displays
    // it so the portal loop is not interrupted by a file error.
    // -------------------------------------------------------
    void updateRoomStatus(const string& roomID, bool status) {
        try {
            roomManager.updateRoomAvailability(roomID, status);
        } catch (const runtime_error& e) {
            cerr << "  Room status update error: " << e.what() << "\n";
        }
    }

    // -------------------------------------------------------
    // reloadRooms()
    //
    // Re-reads Rooms.csv and refreshes the in-memory room list.
    // Call this if Rooms.csv was modified externally (e.g. by
    // the Booking class) and FrontDesk needs fresh data.
    //
    // EXCEPTION HANDLING :
    // Catches runtime_error thrown by loadRoomsFromCSV() if the
    // file is missing, and prints a descriptive message.
    // -------------------------------------------------------
    void reloadRooms() {
        try {
            roomManager.loadRoomsFromCSV();
            cout << "\n  Room data reloaded from Rooms.csv.\n";
        } catch (const runtime_error& e) {
            cerr << "  Reload error: " << e.what() << "\n";
        }
    }

    // -------------------------------------------------------
    // runPortal()
    //
    // Self-contained Front Desk menu loop.
    // Keeps the employee in the portal until they choose
    // Sign Out (option 17). No recursion — uses a while loop
    // so the stack never grows and the session never drops
    // accidentally on an invalid input.
    //
    // Called from LandingPage::employeePortal() after login.
    //
    // Parameters:
    //   employeeName - displayed in the welcome header
    //   employeeID   - displayed in the welcome header
    //   position     - displayed in the welcome header
    //
    // EXCEPTION HANDLING :
    // All CSV-touching operations inside the switch are wrapped
    // in try/catch so a file error never breaks the loop.
    // -------------------------------------------------------
    void runPortal(const string& employeeName,
                   const string& employeeID,
                   const string& position) {

        int menuChoice = 0;

        // Loop continues until the employee selects Sign Out
        while (true) {

            // Print the menu fresh on every iteration so the
            // employee always sees their options after each action
            cout << "\n" << setfill('=') << setw(80) << "" << setfill(' ') << "\n";
            cout << "  FRONT DESK PORTAL\n";
            cout << setfill('=') << setw(80) << "" << setfill(' ') << "\n";
            cout << "  " << left << setw(12) << "Employee"  << ": " << employeeName << "\n";
            cout << "  " << left << setw(12) << "ID"        << ": " << employeeID   << "\n";
            cout << "  " << left << setw(12) << "Position"  << ": " << position     << "\n";
            cout << setfill('-') << setw(80) << "" << setfill(' ') << "\n";
            cout << "\n  -- GUEST OPERATIONS --\n";
            cout << "    1.  View All Guests\n";
            cout << "    2.  Search Guest by ID\n";
            cout << "    3.  Add Guest\n";
            cout << "    4.  Remove Guest\n";
            cout << "    5.  Check In Guest\n";
            cout << "    6.  Check Out Guest\n";
            cout << "    16. Display Session Guest History\n";
            cout << "\n  -- ROOM OPERATIONS --\n";
            cout << "    7.  View All Rooms\n";
            cout << "    8.  View Available Rooms\n";
            cout << "    9.  View Available Rooms by Type\n";
            cout << "    10. View Occupied Rooms\n";
            cout << "    11. View Hotel Capacity\n";
            cout << "    12. Look Up Room by ID\n";
            cout << "    13. Update Room Status\n";
            cout << "    14. Reload Room Data from CSV\n";
            cout << "\n  -- OTHER --\n";
            cout << "    15. Access Billing\n";
            cout << "    17. Sign Out\n";
            cout << "\n  Choice: ";

            // Read input; if extraction fails (e.g. letters typed)
            // clear the error and loop back to the menu prompt
            if (!(cin >> menuChoice)) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "\n  Invalid input. Please enter a number.\n";
                continue;
            }
            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            switch (menuChoice) {

                // ---- GUEST OPERATIONS ----

                case 1: {
                    // View every guest row from GuestInformation.csv
                    // Wrapped in try/catch — viewAllGuests() throws if file missing
                    try {
                        viewAllGuests();
                    } catch (const runtime_error& e) {
                        cerr << "  Error: " << e.what() << "\n";
                    }
                    break;
                }

                case 2: {
                    // Search GuestInformation.csv by CustomerID
                    // Wrapped in try/catch — searchGuestByID() throws if file missing
                    cout << "\n  Enter Customer ID to search: ";
                    string searchID;
                    getline(cin, searchID);
                    try {
                        searchGuestByID(searchID);
                    } catch (const runtime_error& e) {
                        cerr << "  Error: " << e.what() << "\n";
                    }
                    break;
                }

                case 3: {
                    // Add a guest name to guestHistory and Guests.csv
                    // Wrapped in try/catch — addGuest() throws if file missing
                    cout << "\n  Enter guest name to add: ";
                    string guestName;
                    getline(cin, guestName);
                    try {
                        addGuest(guestName);
                    } catch (const runtime_error& e) {
                        cerr << "  Error: " << e.what() << "\n";
                    }
                    break;
                }

                case 4: {
                    // Remove a guest from the in-memory guestHistory
                    cout << "\n  Enter guest name to remove: ";
                    string removeName;
                    getline(cin, removeName);
                    removeGuest(removeName);
                    break;
                }

                case 5: {
                    // Check in a guest and optionally assign their room
                    // in Rooms.csv via RoomManagement::assignGuestToRoom()
                    cout << "\n  Enter guest name: ";
                    string ciName;
                    getline(cin, ciName);
                    cout << "  Enter Customer ID (or press Enter to skip): ";
                    string ciID;
                    getline(cin, ciID);
                    cout << "  Enter Room ID to assign (or press Enter to skip): ";
                    string ciRoom;
                    getline(cin, ciRoom);
                    // checkIn() internally catches runtime_error from RoomManagement
                    checkIn(ciName, ciID, ciRoom);
                    break;
                }

                case 6: {
                    // Check out a guest and release their room in Rooms.csv
                    // via RoomManagement::releaseRoom()
                    cout << "\n  Enter guest name: ";
                    string coName;
                    getline(cin, coName);
                    cout << "  Enter Room ID to release (or press Enter to skip): ";
                    string coRoom;
                    getline(cin, coRoom);
                    // checkOut() internally catches runtime_error from RoomManagement
                    checkOut(coName, coRoom);
                    break;
                }

                // ---- ROOM OPERATIONS ----

                case 7: {
                    // Display every room via RoomManagement::displayAllRooms()
                    // Room operator<< is used inside the table rows
                    accessRoomManagement();
                    break;
                }

                case 8: {
                    // Display only available rooms via
                    // RoomManagement::displayAvailableRooms()
                    viewAvailableRooms();
                    break;
                }

                case 9: {
                    // Filter by room type via RoomManagement::getRoomsForType()
                    // Room operator<< is used for each row output
                    cout << "\n  Enter room type (Standard / Deluxe / Presidential Suite): ";
                    string rType;
                    getline(cin, rType);
                    viewAvailableRoomsByType(rType);
                    break;
                }

                case 10: {
                    // Display occupied rooms via
                    // RoomManagement::displayRoomHistory()
                    viewOccupiedRooms();
                    break;
                }

                case 11: {
                    // Display total vs. available count via
                    // RoomManagement::displayHotelCapacity()
                    viewHotelCapacity();
                    break;
                }

                case 12: {
                    // Find and display a single room via
                    // RoomManagement::searchRoomByID(), then calls
                    // the virtual displayDetails() (polymorphism)
                    cout << "\n  Enter Room ID (e.g. 101): ";
                    string rID;
                    getline(cin, rID);
                    lookupRoomByID(rID);
                    break;
                }

                case 13: {
                    // Manually toggle a room's availability via
                    // RoomManagement::updateRoomAvailability()
                    // updateRoomStatus() catches runtime_error internally
                    cout << "\n  Enter Room ID to update: ";
                    string uRoom;
                    getline(cin, uRoom);
                    cout << "  Set to available? (1 = Yes / 0 = No): ";
                    int statusIn;
                    if (!(cin >> statusIn)) {
                        cin.clear();
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                        cout << "  Invalid input.\n";
                        break;
                    }
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    updateRoomStatus(uRoom, statusIn == 1);
                    break;
                }

                case 14: {
                    // Re-read Rooms.csv into memory via
                    // RoomManagement::loadRoomsFromCSV()
                    // reloadRooms() catches runtime_error internally
                    reloadRooms();
                    break;
                }

                case 15: {
                    // Billing placeholder
                    accessBilling();
                    break;
                }

                case 16: {
                    // Print numbered in-memory guestHistory list
                    displayGuestHistory();
                    break;
                }

                case 17: {
                    // Sign Out — ONLY exit point from the portal loop
                    cout << "\n  Signing out. Returning to main menu.\n";
                    return; // exits runPortal(), returns to LandingPage
                }

                default: {
                    cout << "\n  Invalid option. Please choose 1-17.\n";
                    break;
                }
            }
        }
    }

    // ADMIN ACCOUNT ONLY (IF WE HAVE TIME TO IMPLEMENT)

    // -------------------------------------------------------
    // addEmployee()
    //
    // Placeholder for adding an employee record.
    // -------------------------------------------------------
    void addEmployee(string employeeName) {
        cout << "\n  " << employeeName << " added as employee.\n";
    }

    // -------------------------------------------------------
    // removeEmployee()
    //
    // Placeholder for removing an employee record.
    // -------------------------------------------------------
    void removeEmployee(string employeeName) {
        cout << "\n  " << employeeName << " removed from employees.\n";
    }
};
#endif
