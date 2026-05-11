#ifndef ROOM_H
#define ROOM_H

#include <iostream>
#include <iomanip>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <stdexcept>
using namespace std;

/*
This file contains the Room base class and all derived room classes.

The purpose of this file is to model different hotel room types using :

1. Inheritance
2. Polymorphism
3. Encapsulation
4. Virtual Functions

The Room class acts as the BASE CLASS for all room types in the hotel.

Every room in the hotel shares common information such as :
- room ID
- room number
- price
- availability
- capacity
- current guest

Instead of rewriting this information multiple times for each room type,
the program stores these shared attributes inside one base class called Room.

The derived classes:
- StandardRoom
- DeluxeRoom
- SuiteRoom
- PresidentialSuiteRoom

Inheritance allows us to reuse common room data and behavior.

For example :
All room types need :
- a room number
- a room price
- availability tracking

Rather than rewriting these variables for every room type,
the derived classes inherit them from the Room base class.

This reduces repeated code and improves organization.

We use polymorphism because different room types calculate prices differently.

For example :
- StandardRoom may only charge the nightly rate
- DeluxeRoom may include extra service fees
- SuiteRoom may include luxury fees

The Room class uses virtual functions so the program can automatically
use the correct function depending on the room type.

Example :
If the room is a DeluxeRoom, the DeluxeRoom version of calculateCost() will run automatically.
This is called runtime polymorphism.
*/

// BASE CLASS : Room
class Room {

protected:
    /*
    protected means these variables cannot be directly accessed from main(),
    but derived classes like StandardRoom and DeluxeRoom can still use them.

    These are the common attributes that all room types share.
    */
    string roomID;          // Unique ID for the room, example : R101
    int roomNumber;         // Actual room number, example : 101
    double pricePerNight;   // Cost of staying in the room for one night
    bool isAvailable;       // true = room is available, false = room is occupied
    int capacity;           // Maximum number of guests allowed in the room
    string currentGuest;    // Name of the guest currently assigned to the room

    // -------------------------------------------------------
    // PROTECTED HELPER : printRow (string value)
    //
    // Formats a label/value pair using iomanip for consistent
    // two-column alignment in all displayDetails() overrides.
    // Label column is left-aligned at 18 characters wide.
    // -------------------------------------------------------
    void printRow(const string& label, const string& value) const {
        cout << "  " << left << setw(18) << label << ": " << value << "\n";
    }

    // -------------------------------------------------------
    // PROTECTED HELPER : printRow (double value)
    //
    // Overload for monetary values — always shows 2 decimal
    // places and a leading dollar sign via fixed + setprecision.
    // -------------------------------------------------------
    void printRow(const string& label, double value) const {
        cout << "  " << left << setw(18) << label
             << ": $" << fixed << setprecision(2) << value << "\n";
    }

    // -------------------------------------------------------
    // PROTECTED HELPER : printRow (int value)
    //
    // Overload for plain integer values (e.g. capacity count).
    // -------------------------------------------------------
    void printRow(const string& label, int value) const {
        cout << "  " << left << setw(18) << label << ": " << value << "\n";
    }

public:
    /*
        Default Constructor

        Creates an empty/default Room object.

        This allows us to create a Room object without immediately giving values.
    */
    Room()
        : roomID(""), roomNumber(0), pricePerNight(0.0),
          isAvailable(true), capacity(0), currentGuest("") {}

    /*
        Parameterized Constructor

        Creates a Room object with specific values immediately assigned.

        Example:
        Room("R101", 101, 150.0, true, 2);
    */
    Room(string id, int number, double price, bool available, int cap)
        : roomID(id), roomNumber(number), pricePerNight(price),
          isAvailable(available), capacity(cap), currentGuest("") {}

    /*
        Virtual Destructor

        Since Room is a base class and we use Room pointers for polymorphism,
        the destructor should be virtual.

        This ensures derived objects are destroyed correctly in memory.
    */
    virtual ~Room() {}

    // GETTER FUNCTIONS

    /*
        Getter functions safely return attribute values.

        Using getters supports encapsulation by preventing direct access
        to protected/private data.
    */

    string getRoomID()        const { return roomID; }
    int    getRoomNumber()    const { return roomNumber; }
    double getPricePerNight() const { return pricePerNight; }
    bool   getAvailability()  const { return isAvailable; }
    string getCurrentGuest()  const { return currentGuest; }


    // SETTER FUNCTIONS
    /*
        Setter functions safely modify room information.

        These functions help maintain control over how data changes.
    */

    void setAvailability(bool available)   { isAvailable  = available; }
    void setCurrentGuest(string guestName) { currentGuest = guestName; }

    // VIRTUAL FUNCTIONS (POLYMORPHISM)

    /*
    Returns the type of room.

    Since every derived class represents a different room type,
    each class overrides this function.

    Example:
    - StandardRoom returns "Standard"
    - DeluxeRoom returns "Deluxe"
    */

    virtual string getRoomType() const { return "Room"; }


    /*
    Displays room information.

    This function is virtual because different room types may display
    different details or extra services.

    Example:
    Deluxe rooms may display service fees,
    while presidential suites may display premium services.

    iomanip is used via the protected printRow() helpers to produce
    a consistent left-aligned two-column layout (label | value).
    setw, left, fixed, and setprecision are all applied here.
    */

    virtual void displayDetails() const {
        // Horizontal rule using setfill + setw
        cout << "  " << setfill('-') << setw(38) << "" << setfill(' ') << "\n";
        printRow("Room ID",      roomID);
        printRow("Room Number",  roomNumber);
        printRow("Type",         getRoomType());
        printRow("Price/Night",  pricePerNight);
        printRow("Availability", isAvailable ? "Available" : "Occupied");
        printRow("Capacity",     to_string(capacity) + " guests");
        if (!currentGuest.empty())
            printRow("Current Guest", currentGuest);
        cout << "  " << setfill('-') << setw(38) << "" << setfill(' ') << "\n";
    }


    /*
    Calculates the total cost of staying in the room.

    This function demonstrates POLYMORPHISM because every room type may
    calculate the total differently.

    Example:
    StandardRoom:
        pricePerNight * nights

    DeluxeRoom:
        (pricePerNight * nights) + service fee

    SuiteRoom:
        (pricePerNight * nights) + luxury fee

    EXCEPTION HANDLING :
    Throws std::invalid_argument if nights <= 0.
    The caller (FrontDesk or Booking) wraps this in a try/catch block
    to display a user-friendly error without crashing the program.
    */

    virtual double calculateCost(int nights) const {
        if (nights <= 0) {
            throw invalid_argument(
                "Invalid stay duration: nights must be greater than zero.");
        }
        return pricePerNight * nights;
    }

    // ROOM BOOKING FUNCTION

    /*
        bookRoom()

        Assigns a guest to the room and updates room availability.

        When a booking is successful:
        - currentGuest is updated
        - isAvailable becomes false

        This function connects directly to the Booking system.
    */

    void bookRoom(string guestName) {
        if (!isAvailable) {
            cout << "Room " << roomID << " is already occupied." << endl;
            return;
        }
        currentGuest = guestName;
        isAvailable  = false;
        cout << "Room " << roomID << " successfully booked for "
             << guestName << "." << endl;
    }

    // OPERATOR OVERLOADING

    /*
        operator==

        Compares two Room objects by roomID string.

        Supports equality checks such as:
            if (roomA == roomB) { ... }

        Returns true only when both rooms share the exact same roomID.
    */
    bool operator==(const Room& other) const {
        return roomID == other.roomID;
    }

    /*
        operator<

        Compares two Room objects by roomNumber for ascending sort order.

        Allows collections of rooms to be ordered numerically, e.g.:
            sort(roomVec.begin(), roomVec.end(), ...)

        Returns true if this room's number is less than the other's.
    */
    bool operator<(const Room& other) const {
        return roomNumber < other.roomNumber;
    }

    /*
        operator<<  (stream insertion)

        Outputs a compact one-line room summary to any ostream, e.g.:
            cout << myRoom;

        Declared as a friend so it can read protected member variables
        directly without going through getter calls.

        Output format (iomanip aligned):
          [RoomID]  [Type]              [Status]     $[Price]
    */
    friend ostream& operator<<(ostream& os, const Room& room) {
        os << left
           << setw(8)  << room.roomID
           << setw(22) << room.getRoomType()
           << setw(12) << (room.isAvailable ? "Available" : "Occupied")
           << "$" << fixed << setprecision(2) << room.pricePerNight;
        return os;
    }
};

// DERIVED CLASS : StandardRoom
class StandardRoom : public Room {

public:

    /*
        StandardRoom Constructor

        Inherits common room data from the Room base class.
    */

    StandardRoom(string id, int number, double price, bool available, int cap)
        : Room(id, number, price, available, cap) {}


    /*
        Overrides getRoomType()

        Returns:
        "Standard"
    */

    string getRoomType() const override { return "Standard"; }


    /*
        Overrides displayDetails()
        Displays details specific to a standard room.
    */

    void displayDetails() const override {
        cout << "\n  [ STANDARD ROOM ]\n";
        Room::displayDetails();
    }


    /*
        Overrides calculateCost(). Standard rooms use the simplest pricing calculation.
    */

    double calculateCost(int nights) const override {
        // EXCEPTION HANDLING: guard against invalid night count
        if (nights <= 0) {
            throw invalid_argument(
                "Invalid stay duration: nights must be greater than zero.");
        }
        return pricePerNight * nights;
    }
};

// DERIVED CLASS: DeluxeRoom

class DeluxeRoom : public Room {

private:

    /*
        Additional fee unique to deluxe rooms.

        This demonstrates how derived classes can add their own
        specialized attributes.
    */

    double serviceFee;

public:

    DeluxeRoom(string id, int number, double price, bool available, int cap, double fee)
        : Room(id, number, price, available, cap), serviceFee(fee) {}

    string getRoomType() const override { return "Deluxe"; }

    void displayDetails() const override {
        cout << "\n  [ DELUXE ROOM ]\n";
        Room::displayDetails();
        // Print the extra service fee below the shared base fields
        cout << "  " << setfill('-') << setw(38) << "" << setfill(' ') << "\n";
        printRow("Service Fee", serviceFee);
        cout << "  " << setfill('-') << setw(38) << "" << setfill(' ') << "\n";
    }

    /*
        Deluxe room pricing may include:
        - nightly cost
        - additional service fee
    */

    double calculateCost(int nights) const override {
        // EXCEPTION HANDLING: guard against invalid night count
        if (nights <= 0) {
            throw invalid_argument(
                "Invalid stay duration: nights must be greater than zero.");
        }
        return (pricePerNight * nights) + serviceFee;
    }
};

// DERIVED CLASS: SuiteRoom
class SuiteRoom : public Room {

private:

    /*
        Luxury fee specific to suite rooms.
    */

    double luxuryFee;

public:

    SuiteRoom(string id, int number, double price, bool available, int cap, double fee)
        : Room(id, number, price, available, cap), luxuryFee(fee) {}

    string getRoomType() const override { return "Suite"; }

    void displayDetails() const override {
        cout << "\n  [ SUITE ROOM ]\n";
        Room::displayDetails();
        cout << "  " << setfill('-') << setw(38) << "" << setfill(' ') << "\n";
        printRow("Luxury Fee", luxuryFee);
        cout << "  " << setfill('-') << setw(38) << "" << setfill(' ') << "\n";
    }

    /*
        Suite rooms calculate cost using :
        nightly cost + luxury fee
    */

    double calculateCost(int nights) const override {
        // EXCEPTION HANDLING: guard against invalid night count
        if (nights <= 0) {
            throw invalid_argument(
                "Invalid stay duration: nights must be greater than zero.");
        }
        return (pricePerNight * nights) + luxuryFee;
    }
};

// DERIVED CLASS: PresidentialSuiteRoom
class PresidentialSuiteRoom : public Room {

private:

    /*
        Premium fee for the highest room type in the hotel.
    */

    double premiumFee;

public:

    PresidentialSuiteRoom(string id, int number, double price, bool available, int cap, double fee)
        : Room(id, number, price, available, cap), premiumFee(fee) {}

    string getRoomType() const override { return "Presidential Suite"; }

    void displayDetails() const override {
        cout << "\n  [ PRESIDENTIAL SUITE ]\n";
        Room::displayDetails();
        cout << "  " << setfill('-') << setw(38) << "" << setfill(' ') << "\n";
        printRow("Premium Fee", premiumFee);
        cout << "  " << setfill('-') << setw(38) << "" << setfill(' ') << "\n";
    }

    /*
        Presidential suites calculate cost using:
        nightly cost + premium fee
    */

    double calculateCost(int nights) const override {
        // EXCEPTION HANDLING: guard against invalid night count
        if (nights <= 0) {
            throw invalid_argument(
                "Invalid stay duration: nights must be greater than zero.");
        }
        return (pricePerNight * nights) + premiumFee;
    }
};

#endif
