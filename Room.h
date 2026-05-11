#ifndef ROOM_H
#define ROOM_H

#include <iostream>
#include <string>
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

public:
    /*
        Default Constructor

        Creates an empty/default Room object.

        This allows us to create a Room object without immediately giving values.
    */
    Room();
    /*
        Parameterized Constructor

        Creates a Room object with specific values immediately assigned.

        Example:
        Room("R101", 101, 150.0, true, 2);
    */
    Room(string id, int number, double price, bool available, int cap);
    
    /*
        Virtual Destructor

        Since Room is a base class and we use Room pointers for polymorphism,
        the destructor should be virtual.

        This ensures derived objects are destroyed correctly in memory.
    */
    virtual ~Room();

    // GETTER FUNCTIONS

    /*
        Getter functions safely return attribute values.

        Using getters supports encapsulation by preventing direct access
        to protected/private data.
    */

    string getRoomID() const;
    int getRoomNumber() const;
    double getPricePerNight() const;
    bool getAvailability() const;
    string getCurrentGuest() const;


    // SETTER FUNCTIONS
    /*
        Setter functions safely modify room information.

        These functions help maintain control over how data changes.
    */

    void setAvailability(bool available);
    void setCurrentGuest(string guestName);

    // VIRTUAL FUNCTIONS (POLYMORPHISM)

    /*
    Returns the type of room.

    Since every derived class represents a different room type,
    each class overrides this function.

    Example:
    - StandardRoom returns "Standard Room"
    - DeluxeRoom returns "Deluxe Room"
    */

    virtual string getRoomType() const;


    /*
    Displays room information.

    This function is virtual because different room types may display
    different details or extra services.

    Example:
    Deluxe rooms may display service fees,
    while presidential suites may display premium services.
    */

    virtual void displayDetails() const;


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
    */

    virtual double calculateCost(int nights) const;

    // ROOM BOOKING FUNCTION
    
    /*
        bookRoom()

        Assigns a guest to the room and updates room availability.

        When a booking is successful:
        - currentGuest is updated
        - isAvailable becomes false

        This function connects directly to the Booking system.
    */

    void bookRoom(string guestName);
};

// DERIVED CLASS : StandardRoom
class StandardRoom : public Room {

public:

    /*
        StandardRoom Constructor

        Inherits common room data from the Room base class.
    */

    StandardRoom(string id, int number, double price, bool available, int cap);


    /*
        Overrides getRoomType()

        Returns:
        "Standard Room"
    */

    string getRoomType() const override;


    /*
        Overrides displayDetails()
        Displays details specific to a standard room.
    */

    void displayDetails() const override;


    /*
        Overrides calculateCost().Standard rooms use the simplest pricing calculation.
    */

    double calculateCost(int nights) const override;
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

    DeluxeRoom(string id, int number, double price, bool available, int cap, double fee);

    string getRoomType() const override;

    void displayDetails() const override;

    /*
        Deluxe room pricing may include:
        - nightly cost
        - additional service fee
    */

    double calculateCost(int nights) const override;
};

// DERIVED CLASS: SuiteRoom
class SuiteRoom : public Room {

private:

    /*
        Luxury fee specific to suite rooms.
    */

    double luxuryFee;

public:

    SuiteRoom(string id, int number, double price, bool available, int cap, double fee);

    string getRoomType() const override;

    void displayDetails() const override;

    /*
        Suite rooms calculate cost using :
        nightly cost + luxury fee
    */

    double calculateCost(int nights) const override;
};

// DERIVED CLASS: PresidentialSuiteRoom
class PresidentialSuiteRoom : public Room {

private:

    /*
        Premium fee for the highest room type in the hotel.
    */

    double premiumFee;

public:

    PresidentialSuiteRoom(string id, int number, double price, bool available, int cap, double fee);

    string getRoomType() const override;

    void displayDetails() const override;

    /*
        Presidential suites calculate cost using:
        nightly cost + premium fee
    */

    double calculateCost(int nights) const override;
};

#endif