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
            cout << "\t2. Display Current Booking(s)" << endl;
            cout << "\t3. Create New Booking" << endl;
            cout << "\t4. Edit Profile" << endl;
            cout << "\t5. Check Out" << endl;
            cout << "\t6. Log Out" << endl;

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
                    checkOutGuest(g);
                }
                else if (guestPortalChoice == 6) {
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


        // -------------------------------------------------------
        // displayCurrentBooking()
        //
        // Reads GuestInformation.csv and displays ALL bookings
        // associated with this guest's CustomerID.
        //
        // Guests are allowed a maximum of 2 active bookings.
        // Both bookings are displayed if present.
        //
        // Each booking row in the CSV is identified by:
        // CustomerID, Name, Email, Phone, Country, ZIP,
        // BookingDate, Check-inDate, Check-out Date, RoomType, RoomID
        // -------------------------------------------------------
        void displayCurrentBooking(Guest &g) {
            
            centerText("Current Booking(s)", 40, '-');
            
            std::ifstream myFile("GuestInformation.csv");
            
            if (!myFile.is_open()) {
                std::cerr << "Unable to open GuestInformation.csv" << std::endl;
                return;
            }

            std::string line;
            bool headerSkipped = false;
            int bookingCount = 0;

            while (std::getline(myFile, line)) {
                // Skip header row
                if (!headerSkipped) { headerSkipped = true; continue; }
                if (line.empty()) continue;

                std::stringstream ss(line);
                std::string custID, name, email, phone, country, zip,
                            bookDate, checkIn, checkOut, roomT, roomId;
                
                // Parse CSV: CustomerID,RegistrantName,Email,PhoneNumber,
                //            Country/Region,ZIP,BookingDate,Check-inDate,
                //            Check-out Date,RoomType,RoomID
                if (std::getline(ss, custID,   ',') &&
                    std::getline(ss, name,     ',') &&
                    std::getline(ss, email,    ',') &&
                    std::getline(ss, phone,    ',') &&
                    std::getline(ss, country,  ',') &&
                    std::getline(ss, zip,      ',') &&
                    std::getline(ss, bookDate, ',') &&
                    std::getline(ss, checkIn,  ',') &&
                    std::getline(ss, checkOut, ',') &&
                    std::getline(ss, roomT,    ',') &&
                    std::getline(ss, roomId)) {

                    // Trim all fields before comparing
                    auto trim = [](std::string s) {
                        size_t a = s.find_first_not_of(" \t\r\n");
                        size_t b = s.find_last_not_of(" \t\r\n");
                        return (a == std::string::npos) ? "" : s.substr(a, b - a + 1);
                    };

                    custID   = trim(custID);
                    bookDate = trim(bookDate);
                    checkIn  = trim(checkIn);
                    checkOut = trim(checkOut);
                    roomT    = trim(roomT);
                    roomId   = trim(roomId);

                    // Only show rows with a real booking date (not empty)
                    if (custID == g.getCustomerID() && !bookDate.empty()) {
                        bookingCount++;
                        cout << "\n  Booking #" << bookingCount << ":\n";
                        cout << "  Room Type   : " << roomT    << endl;
                        cout << "  Room ID     : " << roomId   << endl;
                        cout << "  Booked On   : " << bookDate << endl;
                        cout << "  Check-In    : " << checkIn  << endl;
                        cout << "  Check-Out   : " << checkOut << endl;
                    }
                }
            }
            myFile.close();

            if (bookingCount == 0) {
                cout << "No active bookings found for this guest." << endl;
            }

            centerText("=", 40, '=');
            cout << endl;
        }
         
      
        // -------------------------------------------------------
        // dateToInt()
        //
        // Converts a MM/DD/YYYY date string into a single integer
        // of the form YYYYMMDD, which allows simple numeric
        // comparisons between dates using <, >, ==.
        //
        // Example: "05/10/2026" -> 20260510
        //
        // Used by createNewBooking() to validate that:
        // - Check-in date is not earlier than the booking date.
        // - Check-out date is not the same as booking or check-in.
        //
        // Returns -1 if the string is not in MM/DD/YYYY format.
        // -------------------------------------------------------
        int dateToInt(const std::string& date) {
            if (date.length() != 10 || date[2] != '/' || date[5] != '/') {
                return -1;
            }
            try {
                int month = std::stoi(date.substr(0, 2));
                int day   = std::stoi(date.substr(3, 2));
                int year  = std::stoi(date.substr(6, 4));
                return (year * 10000) + (month * 100) + day;
            } catch (...) {
                return -1;
            }
        }

        // -------------------------------------------------------
        // calculateCheckOutDate()
        //
        // Calculates the check-out date by adding numNights to
        // the provided check-in date string (MM/DD/YYYY).
        //
        // This fixes the original determineDate() which incorrectly
        // added days to today's date instead of the check-in date.
        //
        // EXCEPTION HANDLING:
        // - Throws std::invalid_argument if the date string is not
        //   in MM/DD/YYYY format or contains non-numeric values.
        // - Throws std::invalid_argument if numNights <= 0.
        //
        // Returns the check-out date as a MM/DD/YYYY string.
        // -------------------------------------------------------
        std::string calculateCheckOutDate(const std::string& checkIn, int nights) {

            // Validate nights
            if (nights <= 0) {
                throw std::invalid_argument(
                    "Number of nights must be greater than zero.");
            }

            // Validate format: must be exactly MM/DD/YYYY (10 chars, two slashes)
            if (checkIn.length() != 10 ||
                checkIn[2] != '/' || checkIn[5] != '/') {
                throw std::invalid_argument(
                    "Check-in date must be in MM/DD/YYYY format.");
            }

            // Parse month, day, year from the check-in string
            int month, day, year;
            try {
                month = std::stoi(checkIn.substr(0, 2));
                day   = std::stoi(checkIn.substr(3, 2));
                year  = std::stoi(checkIn.substr(6, 4));
            } catch (...) {
                throw std::invalid_argument(
                    "Check-in date contains non-numeric values. Use MM/DD/YYYY.");
            }

            // Validate month and day ranges
            if (month < 1 || month > 12) {
                throw std::invalid_argument("Month must be between 01 and 12.");
            }
            if (day < 1 || day > 31) {
                throw std::invalid_argument("Day must be between 01 and 31.");
            }

            // Use std::tm to let the C library handle month rollovers,
            // leap years, and year boundaries correctly.
            // tm_mon is 0-based so subtract 1 from the parsed month.
            std::tm timeStruct = {};
            timeStruct.tm_year = year - 1900; // years since 1900
            timeStruct.tm_mon  = month - 1;   // 0-based month
            timeStruct.tm_mday = day + nights; // add nights here

            // mktime normalises the struct — handles all overflow
            std::time_t result = std::mktime(&timeStruct);
            if (result == -1) {
                throw std::invalid_argument(
                    "Could not compute check-out date from the given inputs.");
            }

            // Format the normalised date back to MM/DD/YYYY
            char buffer[11];
            std::strftime(buffer, sizeof(buffer), "%m/%d/%Y", &timeStruct);
            return std::string(buffer);
        }


        // -------------------------------------------------------
        // countGuestBookings()
        //
        // Counts how many active bookings (rows with a non-empty
        // BookingDate field) exist for this guest in
        // GuestInformation.csv.
        //
        // Used by createNewBooking() to enforce the 1-booking limit.
        //
        // Returns the number of active bookings found (0 or 1).
        // -------------------------------------------------------
        int countGuestBookings(const std::string& customerID) {
            std::ifstream myFile("GuestInformation.csv");
            if (!myFile.is_open()) {
                std::cerr << "Unable to open GuestInformation.csv" << std::endl;
                return 0;
            }

            int count = 0;
            std::string line;
            bool headerSkipped = false;

            while (std::getline(myFile, line)) {
                if (!headerSkipped) { headerSkipped = true; continue; }
                if (line.empty()) continue;

                std::stringstream ss(line);
                std::string custID, name, email, phone, country, zip, bookDate;

                if (std::getline(ss, custID,   ',') &&
                    std::getline(ss, name,     ',') &&
                    std::getline(ss, email,    ',') &&
                    std::getline(ss, phone,    ',') &&
                    std::getline(ss, country,  ',') &&
                    std::getline(ss, zip,      ',') &&
                    std::getline(ss, bookDate, ',')) {

                    // Trim custID and bookDate
                    size_t sa = custID.find_first_not_of(" \t\r\n");
                    size_t ea = custID.find_last_not_of(" \t\r\n");
                    if (sa != std::string::npos)
                        custID = custID.substr(sa, ea - sa + 1);

                    size_t sb = bookDate.find_first_not_of(" \t\r\n");
                    size_t eb = bookDate.find_last_not_of(" \t\r\n");
                    if (sb != std::string::npos)
                        bookDate = bookDate.substr(sb, eb - sb + 1);

                    // Count rows that belong to this guest AND have a booking date
                    if (custID == customerID && !bookDate.empty()) {
                        count++;
                    }
                }
            }
            myFile.close();
            return count;
        }


        // -------------------------------------------------------
        // checkOutGuest()
        //
        // Allows the guest to check out of a specific booking by
        // entering their Room ID.
        //
        // On checkout:
        // 1. The matching row in GuestInformation.csv has its
        //    booking fields (BookingDate through RoomID) cleared.
        // 2. The room in Rooms.csv is reset to Available = True
        //    and Customer ID = 0, so it is available for new bookings.
        //
        // EXCEPTION HANDLING:
        // - Displays an error if either CSV cannot be opened.
        // - Confirms success or reports if the room ID was not found.
        // -------------------------------------------------------
        void checkOutGuest(Guest& g) {
            centerText("CHECK OUT", 40, '-');

            // Show current bookings so the guest knows what to enter
            displayCurrentBooking(g);

            cout << "Enter the Room ID you wish to check out of: ";
            std::string targetRoom;
            std::cin >> targetRoom;
            clearError();

            // Trim input
            size_t ts = targetRoom.find_first_not_of(" \t\r\n");
            size_t te = targetRoom.find_last_not_of(" \t\r\n");
            if (ts != std::string::npos)
                targetRoom = targetRoom.substr(ts, te - ts + 1);

            bool checkoutDone = false;

            // ---- Step 1: Clear booking fields in GuestInformation.csv ----
            {
                std::ifstream inFile("GuestInformation.csv");
                if (!inFile.is_open()) {
                    std::cerr << "Error: Cannot open GuestInformation.csv for checkout." << std::endl;
                    return;
                }

                std::vector<std::string> lines;
                std::string line;
                while (std::getline(inFile, line)) lines.push_back(line);
                inFile.close();

                std::ofstream outFile("GuestInformation.csv.tmp");
                if (!outFile.is_open()) {
                    std::cerr << "Error: Cannot create temp file for GuestInformation.csv." << std::endl;
                    return;
                }

                // Write header unchanged
                if (!lines.empty()) outFile << lines[0] << "\n";

                for (size_t i = 1; i < lines.size(); ++i) {
                    if (lines[i].empty()) { outFile << "\n"; continue; }

                    std::stringstream ss(lines[i]);
                    std::string custID, name, email, phone, country, zip,
                                bookDate, checkIn, checkOut, roomT, roomId;

                    if (std::getline(ss, custID,   ',') &&
                        std::getline(ss, name,     ',') &&
                        std::getline(ss, email,    ',') &&
                        std::getline(ss, phone,    ',') &&
                        std::getline(ss, country,  ',') &&
                        std::getline(ss, zip,      ',') &&
                        std::getline(ss, bookDate, ',') &&
                        std::getline(ss, checkIn,  ',') &&
                        std::getline(ss, checkOut, ',') &&
                        std::getline(ss, roomT,    ',') &&
                        std::getline(ss, roomId)) {

                        // Trim custID and roomId for comparison
                        auto trim = [](std::string s) {
                            size_t a = s.find_first_not_of(" \t\r\n");
                            size_t b = s.find_last_not_of(" \t\r\n");
                            return (a == std::string::npos) ? "" : s.substr(a, b - a + 1);
                        };

                        std::string tCust = trim(custID);
                        std::string tRoom = trim(roomId);

                        if (tCust == g.getCustomerID() && tRoom == targetRoom) {
                            // Clear booking fields — keep personal info
                            outFile << trim(custID)   << ","
                                    << trim(name)     << ","
                                    << trim(email)    << ","
                                    << trim(phone)    << ","
                                    << trim(country)  << ","
                                    << trim(zip)      << ","
                                    << ",,,," << "\n"; // blank booking fields
                            checkoutDone = true;
                        } else {
                            outFile << lines[i] << "\n";
                        }
                    } else {
                        outFile << lines[i] << "\n";
                    }
                }

                outFile.close();
                std::remove("GuestInformation.csv");
                std::rename("GuestInformation.csv.tmp", "GuestInformation.csv");
            }

            // ---- Step 2: Reset room in Rooms.csv to Available = True, Customer ID = 0 ----
            if (checkoutDone) {
                std::ifstream roomIn("Rooms.csv");
                if (!roomIn.is_open()) {
                    std::cerr << "Error: Cannot open Rooms.csv for room reset." << std::endl;
                    return;
                }

                std::vector<std::string> roomLines;
                std::string rline;
                while (std::getline(roomIn, rline)) roomLines.push_back(rline);
                roomIn.close();

                std::ofstream roomOut("Rooms.csv.tmp");
                if (!roomOut.is_open()) {
                    std::cerr << "Error: Cannot create temp file for Rooms.csv." << std::endl;
                    return;
                }

                // Write header unchanged
                if (!roomLines.empty()) roomOut << roomLines[0] << "\n";

                for (size_t i = 1; i < roomLines.size(); ++i) {
                    if (roomLines[i].empty()) { roomOut << "\n"; continue; }

                    std::stringstream rs(roomLines[i]);
                    std::string rID, rType, rAvail, rCust;

                    if (std::getline(rs, rID,    ',') &&
                        std::getline(rs, rType,  ',') &&
                        std::getline(rs, rAvail, ',')) {
                        std::getline(rs, rCust);

                        // Trim rID
                        size_t ra = rID.find_first_not_of(" \t\r\n");
                        size_t rb = rID.find_last_not_of(" \t\r\n");
                        std::string tRoomID = (ra != std::string::npos)
                                              ? rID.substr(ra, rb - ra + 1) : "";

                        if (tRoomID == targetRoom) {
                            // Reset: mark available, clear customer
                            roomOut << rID << "," << rType << ", True, 0\n";
                        } else {
                            roomOut << roomLines[i] << "\n";
                        }
                    } else {
                        roomOut << roomLines[i] << "\n";
                    }
                }

                roomOut.close();
                std::remove("Rooms.csv");
                std::rename("Rooms.csv.tmp", "Rooms.csv");

                cout << "\nCheckout successful! Room " << targetRoom
                     << " has been released.\n";
            } else {
                cout << "\nNo matching booking found for Room ID: "
                     << targetRoom << ". Please check and try again.\n";
            }

            centerText("=", 40, '=');
            cout << endl;
        }


        // -------------------------------------------------------
        // createNewBooking()
        //
        // Allows a guest to create a new room booking.
        //
        // BOOKING LIMIT: A guest may have a maximum of 1 active
        // booking at one time. If the limit is reached the
        // function informs the guest and returns early.
        //
        // CHECK-OUT DATE: Calculated from the entered check-in date
        // plus the number of nights using calculateCheckOutDate(),
        // which correctly handles month/year rollovers.
        //
        // DATE VALIDATION RULES:
        // - Check-in date must not be earlier than today's booking date.
        // - Check-out date must not equal the booking date.
        // - Check-out date must not equal the check-in date (min 1 night).
        //
        // EXCEPTION HANDLING:
        // - Invalid date format is caught and reported.
        // - Invalid night count is caught and reported.
        // - Unavailable room types are reported before booking.
        //
        // Booking Date, Check-in Date, Check-out Date, Room Type, RoomId
        // -------------------------------------------------------
        void createNewBooking(Guest &g) {
            centerText("NEW BOOKING", 20, '=');

            // ---- Enforce 1-booking maximum ----
            int currentBookings = countGuestBookings(g.getCustomerID());
            if (currentBookings >= 1) {
                cout << "You already have an active booking.\n";
                cout << "Please check out of your current booking before creating a new one.\n";
                return;
            }
            cout << "Active bookings: " << currentBookings << " / 1\n\n";

            std::string date = assignDate();
            cout << "Booking Date: " << date << endl;

            // ---- Check-in date input with validation ----
            // Rule: check-in date must not be earlier than the booking date.
            // dateToInt() converts MM/DD/YYYY to YYYYMMDD for numeric comparison.
            int bookingDateInt = dateToInt(date);
            bool validDate = false;
            while (!validDate) {
                cout << "Please enter a check-in date (MM/DD/YYYY): ";
                std::cin >> checkInDate;
                clearError();

                // Validate format first
                if (checkInDate.length() != 10 ||
                    checkInDate[2] != '/' || checkInDate[5] != '/') {
                    cout << "Invalid date format. Please use MM/DD/YYYY.\n";
                    continue;
                }

                // Validate check-in is not before the booking date
                int checkInInt = dateToInt(checkInDate);
                if (checkInInt == -1) {
                    cout << "Invalid date. Please use MM/DD/YYYY.\n";
                    continue;
                }
                if (checkInInt < bookingDateInt) {
                    cout << "Check-in date cannot be earlier than today's "
                            "booking date (" << date << ").\n";
                    cout << "Please enter a date on or after " << date << ".\n";
                    continue;
                }

                validDate = true;
            }

            // ---- Number of nights input with validation ----
            bool validNights = false;
            while (!validNights) {
                cout << "How many nights are you staying? ";
                if (!(std::cin >> numNights)) {
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    cout << "Invalid input. Please enter a whole number.\n";
                } else {
                    clearError();
                    if (numNights <= 0) {
                        cout << "Number of nights must be at least 1.\n";
                    } else {
                        validNights = true;
                    }
                }
            }

            // ---- Calculate check-out date from check-in + nights ----
            // This replaces the broken determineDate() which used today's date.
            // Additional rules enforced after calculation:
            // - Check-out must not equal the booking date.
            // - Check-out must not equal the check-in date (minimum 1 night).
            bool validCheckout = false;
            while (!validCheckout) {
                try {
                    checkOutDate = calculateCheckOutDate(checkInDate, numNights);
                } catch (const std::invalid_argument& e) {
                    cout << "Date error: " << e.what() << endl;
                    cout << "Booking cancelled. Please try again.\n";
                    return;
                }

                int checkOutInt  = dateToInt(checkOutDate);
                int checkInInt2  = dateToInt(checkInDate);

                // Check-out cannot equal the booking date
                if (checkOutInt == bookingDateInt) {
                    cout << "Check-out date cannot be the same as the "
                            "booking date (" << date << ").\n";
                    cout << "Please enter a higher number of nights.\n";
                    cout << "How many nights are you staying? ";
                    if (!(std::cin >> numNights) || numNights <= 0) {
                        std::cin.clear();
                        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                        numNights = 1;
                    }
                    clearError();
                    continue;
                }

                // Check-out cannot equal the check-in date (0 nights = invalid)
                if (checkOutInt == checkInInt2) {
                    cout << "Check-out date cannot be the same as the "
                            "check-in date (" << checkInDate << ").\n";
                    cout << "Please enter at least 1 night.\n";
                    cout << "How many nights are you staying? ";
                    if (!(std::cin >> numNights) || numNights <= 0) {
                        std::cin.clear();
                        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                        numNights = 1;
                    }
                    clearError();
                    continue;
                }

                validCheckout = true;
                cout << "Check-Out Date: " << checkOutDate << endl;
            }

            // ---- Room type selection ----
            cout << "Enter the room type (Standard, Deluxe, Presidential Suite): ";
            std::getline(std::cin, roomType);

            if (roomType == "Standard") {
                if (isRoomAvailable("Standard")) {
                    if (selectAvailableRoom("Standard", g.getCustomerID())) {
                        updateGuestBooking(g, date, checkInDate, checkOutDate, roomType, roomID);
                        cout << "Booking created successfully!" << endl;
                    } else {
                        cout << "Error assigning a Standard room." << endl;
                    }
                } else {
                    cout << "Sorry, no Standard rooms are available." << endl;
                }
            }
            else if (roomType == "Deluxe") {
                if (isRoomAvailable("Deluxe")) {
                    if (selectAvailableRoom("Deluxe", g.getCustomerID())) {
                        updateGuestBooking(g, date, checkInDate, checkOutDate, roomType, roomID);
                        cout << "Booking created successfully!" << endl;
                    } else {
                        cout << "Error assigning a Deluxe room." << endl;
                    }
                } else {
                    cout << "Sorry, no Deluxe rooms are available." << endl;
                }
            }
            else if (roomType == "Presidential Suite") {
                if (isRoomAvailable("Presidential Suite")) {
                    if (selectAvailableRoom("Presidential Suite", g.getCustomerID())) {
                        updateGuestBooking(g, date, checkInDate, checkOutDate, roomType, roomID);
                        cout << "Booking created successfully!" << endl;
                    } else {
                        cout << "Error assigning a Presidential Suite room." << endl;
                    }
                } else {
                    cout << "Sorry, no Presidential Suite rooms are available." << endl;
                }
            }
            else {
                cout << "Invalid room type. Please enter Standard, Deluxe, or Presidential Suite." << endl;
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
        bool selectAvailableRoom(const string& roomType, const string& customerID) {
            std::ifstream myFile("Rooms.csv");
            std::vector<std::string> lines;
            std::string line;
            bool found = false;
            
            if (!myFile.is_open()) {
                std::cerr << "Unable to open Rooms.csv" << std::endl;
                return false;
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
                            found = true;
                            cout << "Room assigned: " << roomID << " (" << roomType << ")" << endl;
                        }
                    }
                }
                lineNum++;
            }
            myFile.close();
            
            if (found) {
                updateRoomAssignment(roomID, customerID);
            }
            return found;
        }

        // Update the assigned room in Rooms.csv to unavailable and set the customer ID
        void updateRoomAssignment(const string& assignedRoomID, const string& customerID) {
            std::ifstream inFile("Rooms.csv");
            if (!inFile.is_open()) {
                std::cerr << "Unable to open Rooms.csv" << std::endl;
                return;
            }
            
            std::vector<std::string> lines;
            std::string line;
            while (std::getline(inFile, line)) {
                lines.push_back(line);
            }
            inFile.close();
            
            if (lines.empty()) {
                std::cerr << "Rooms.csv is empty or missing header." << std::endl;
                return;
            }
            
            std::ofstream tempFile("Rooms.csv.tmp");
            if (!tempFile.is_open()) {
                std::cerr << "Unable to open temporary Rooms.csv file" << std::endl;
                return;
            }
            
            // Write the header line exactly as read
            tempFile << lines[0] << std::endl;
            
            for (size_t i = 1; i < lines.size(); ++i) {
                std::stringstream ss(lines[i]);
                std::string rID, type, availability, custID;
                
                if (std::getline(ss, rID, ',') &&
                    std::getline(ss, type, ',') &&
                    std::getline(ss, availability, ',') &&
                    std::getline(ss, custID)) {
                    
                    rID.erase(0, rID.find_first_not_of(" \t"));
                    rID.erase(rID.find_last_not_of(" \t") + 1);
                    type.erase(0, type.find_first_not_of(" \t"));
                    type.erase(type.find_last_not_of(" \t") + 1);
                    availability.erase(0, availability.find_first_not_of(" \t"));
                    availability.erase(availability.find_last_not_of(" \t") + 1);
                    custID.erase(0, custID.find_first_not_of(" \t"));
                    custID.erase(custID.find_last_not_of(" \t") + 1);
                    
                    if (rID == assignedRoomID) {
                        tempFile << rID << ", " << type << ", False, " << customerID << std::endl;
                    } else {
                        tempFile << rID << ", " << type << ", " << availability << ", " << custID << std::endl;
                    }
                } else {
                    tempFile << lines[i] << std::endl;
                }
            }
            
            tempFile.close();
            
            std::remove("Rooms.csv");
            std::rename("Rooms.csv.tmp", "Rooms.csv");
        }

        // -------------------------------------------------------
        // updateGuestBooking()
        //
        // Writes a booking row to GuestInformation.csv.
        // If a row exists for the guest with no booking date
        // (empty slot from a checkout), it is updated in place.
        // Otherwise a new row is appended.
        //
        // This supports the 1-booking-per-guest model — a new
        // row is only written when the guest has no active booking.
        // -------------------------------------------------------
        void updateGuestBooking(Guest &g, const string &bookingDate, const string &checkIn,
                                 const string &checkOut, const string &type, const string &room) {
            std::ifstream inFile("GuestInformation.csv");
            std::string line;
            std::vector<std::string> lines;
            bool updated = false;
            
            if (!inFile.is_open()) {
                std::cerr << "Error opening GuestInformation.csv" << std::endl;
                return;
            }
            
            while (std::getline(inFile, line)) {
                lines.push_back(line);
            }
            inFile.close();
            
            std::ofstream outFile("GuestInformation.csv.tmp");
            if (!outFile.is_open()) {
                std::cerr << "Error opening temporary file for GuestInformation.csv" << std::endl;
                return;
            }
            
            for (size_t i = 0; i < lines.size(); ++i) {
                if (i == 0) {
                    outFile << lines[i] << std::endl;
                    continue;
                }

                std::stringstream ss(lines[i]);
                std::string custID, name, email, phone, country, zip, bDate;
                
                if (std::getline(ss, custID, ',') &&
                    std::getline(ss, name,   ',') &&
                    std::getline(ss, email,  ',') &&
                    std::getline(ss, phone,  ',') &&
                    std::getline(ss, country,',') &&
                    std::getline(ss, zip,    ',') &&
                    std::getline(ss, bDate,  ',')) {

                    // Trim custID and bDate
                    auto trim = [](std::string s) {
                        size_t a = s.find_first_not_of(" \t\r\n");
                        size_t b = s.find_last_not_of(" \t\r\n");
                        return (a == std::string::npos) ? "" : s.substr(a, b - a + 1);
                    };

                    std::string tCust  = trim(custID);
                    std::string tBDate = trim(bDate);

                    // Only update an existing row if it belongs to this guest
                    // AND has no booking date yet (empty slot from a checkout)
                    if (tCust == g.getCustomerID() && tBDate.empty() && !updated) {
                        outFile << g.getCustomerID()       << ","
                                << g.getregistrantName()   << ","
                                << g.getemailAddress()     << ","
                                << g.getphoneNumber()      << ","
                                << g.getcountryOrRegion()  << ","
                                << g.getzipCode()          << ","
                                << bookingDate             << ","
                                << checkIn                 << ","
                                << checkOut                << ","
                                << type                    << ","
                                << room                    << std::endl;
                        updated = true;
                        continue;
                    }
                }
                outFile << lines[i] << std::endl;
            }
            outFile.close();
            
            if (!updated) {
                // No empty slot found — append a new booking row.
                // This allows the guest to have a second active booking.
                writeToEndCSV("GuestInformation.csv.tmp",
                               g.getCustomerID(),
                               g.getregistrantName(),
                               g.getemailAddress(),
                               g.getphoneNumber(),
                               g.getcountryOrRegion(),
                               g.getzipCode(),
                               bookingDate,
                               checkIn,
                               checkOut,
                               type,
                               room);
            }
            
            // Replace original file with updated file
            std::remove("GuestInformation.csv");
            std::rename("GuestInformation.csv.tmp", "GuestInformation.csv");
        }

//Destructor to clear entry at the end of the program
         ~Booking() {};
};
#endif
