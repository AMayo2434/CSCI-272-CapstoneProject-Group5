#ifndef ROOMMANAGEMENT_H
#define ROOMMANAGEMENT_H

#include <iostream>
#include <vector>
#include "Room.h"

using namespace std;

class RoomManagement {

private:
    int totalRooms;
    int availableRooms;

    vector<Room*> rooms;

public:

    RoomManagement();

    ~RoomManagement();

    void addRoom(Room* room);

    void displayAvailableRooms() const;

    void displayAllRooms() const;

    void updateRoomAvailability(string roomID, bool status);

    bool checkCapacity() const;

    void displayHotelCapacity() const;

    void displayRoomHistory() const;

    Room* searchRoomByID(string roomID) const;
};

#endif