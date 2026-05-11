#ifndef GUEST_H
#define GUEST_H


#include "functions.h"
#include <ctime>
#include <map>
#include <vector>
#include <fstream>
#include <sstream>
#include <limits>
#include <random>
//Adding in a friend function from the booking class to link these files.
class Booking;
// All functions available to the guest/user class when they initially create an account.

class Guest {

    private:
        long long int customerID; //Remains private as this information references all data for the guest.
        std::string accountCreationDate;
        friend class Booking;

    protected:
        std::string registrantName;
        std::string emailAddress;
        std::string phoneNumber;
        std::string countryOrRegion;
        int zipCode;
        std::string ZIP;
        std::string passWord;
        std::string finalId;

    public:

    //Constuctor
        Guest() {
            customerID = 0;
            registrantName = "";
            emailAddress = "";
            phoneNumber = "";
            countryOrRegion = "";
            zipCode = 0;
            passWord = "";
            finalId = "";
        
        }

// COMPLETE 
//Verifies if another exists.
        void generateCustomerID() {
            // Code to generate a unique customer ID

            //Looked up information on random library
            //Random device creates a new set of numbers upon each run, reducing the chance of repeats
           static std::random_device ran;
           static std::mt19937_64 gen(ran()); 
            //Range from 1 Bil to 9.999 Bil 
           std::uniform_int_distribution<long long> dis (1000000000LL, 9999999999LL);

            long long randomNum = dis(gen);

            std::string convert = std::to_string(randomNum);
            std::string finalId = convert;
            
//Checks to see if there is a duplicate value.
              if (isDuplicate("GuestInformation.csv", convert)) {
                    std::cout << "ID exists. Re-running ID creation." << std::endl;
                    generateCustomerID();// INSERT WAY TO CHECK 
                } 
                else {
                    std::cout << "Your customer ID is: " << convert << std::endl;
                }

                
                std::to_string(customerID) = convert;


// ADD CSV SAVE FUNCTION BELOW.
//Adds customer ID to all relevant CSV files to track the guest's 
//Once an ID is created, the customer must create an account. 
//Customer IDs with no corresponding account information will be deleted from the CSV file after a certain amount of time. 

                writeRowToCSV("GuestInformation.csv", convert);
                writeRowToCSV("VerificationInfo.csv", convert); // Adding user P to file

            convert.clear();
//Resets random number assignment for next use. Customer ID will be saved to CSV file.
            randomNum = 0; 
        }

//Creates account and saves info to CSV file.
        void createAccount() 
        {
            std::cout << std:: setw(10) << "We are excited to have you join our community. Please follow the prompts to create your account." << std::endl;

            std::cout << "Please enter your name: ";
            std::getline(std::cin >> std::ws, registrantName);

//Make sure input follows email conventions
//Need to add in email conventions if I have time
            std::cout << "Please enter your email address: ";
            std::getline(std::cin, emailAddress);

// Phone Number
//Singular input, no spaces. Must be 10 digits long.
            std::cout << "Please enter your 10-digit phone number: ";
            std::cin >> phoneNumber;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); 

// Country
// Registers country or region
            std::cout << "Please enter your country or region: ";
            std::getline(std::cin, countryOrRegion);

// Zip Code Validation (Must be 5 digits)
std::cout << "Please enter your 5-digit zip code: ";
            std::cin >> zipCode;
            convertToStringI(zipCode, ZIP);
            
            while (ZIP.length() != 5) {
                std::cout << "Invalid zip code. Please enter exactly 5 digits: ";
                std::cin >> zipCode;
                convertToStringI(zipCode, ZIP);
            }
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

// Password Validation (Must be at least 12 chars)
            do {
                std::cout << "Please create a password (no spaces, min 12 characters): ";

                if(!(std::getline(std::cin,passWord))) {
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    continue;
                }

                if (passWord.length() < 12) {
                    std::cout << "Password too short. Please try again.";
                }

            } while (passWord.length() < 12);

            std::cout << "Password accepted.";
//Saves to designated CSV based on inline function. 
            writeRowToCSV("GuestInformation.csv", registrantName, emailAddress, phoneNumber, countryOrRegion, zipCode);
            writeToEndCSV("VerificationInfo.csv", passWord);
        
            std:: cout << "Account created successfully. Proceeding to Sign In page..." << std::endl;
            signIn();
        
        }

    bool signIn() {
        std::string enterCustomerID, enterPassWord;
        std::string csvCustomerID, csvPass;
        bool loggedIn = false;

        centerText(" Sign In To Access All Guest Services Associated With Your Account", 100, ('='));

            std::cout << "\t = = = Please enter your CustomerID to get started = = = " << std::endl;
            std::cin >> enterCustomerID;
            std::cout << "Please enter your password: ";
            std::cin >> enterPassWord;

        std::ifstream file("VerificationInfo.csv");
        if(!file.is_open()) {
            std::cout << "Error. Unable to open file." << std::endl;
        }

        std::string loginline;
        //Reads entire file
        while(std::getline(file, loginline)) {
            if(loginline.empty()) continue;

            std::stringstream ss(loginline);
            
           //Use arguments for all available values.
            if(std::getline(ss, csvCustomerID, ',') && std::getline(ss, csvPass)) {
                
                
                if (!csvPass.empty() && csvPass.back() == '\r') csvPass.pop_back();

                if (csvCustomerID == enterCustomerID && csvPass == enterPassWord){
                    this->customerID = std::stoll(csvCustomerID); 
                    this->passWord = csvPass;
                    loggedIn = true;
                    break; // Exit loop, found user
                }
            }
        }
        file.close();

        if(loggedIn) {
            this->finalId = csvCustomerID;
            loadGuestInfo();  // Load all guest info from CSV
            std::cout << "Sign in successful! Proceeding to portal." << std::endl;
            return true;
           
        } else {
            std::cout << "Invalid CustomerID or password." << std::endl;
            return false;
        }
}

        // Load guest information from GuestInformation.csv based on customer ID
        void loadGuestInfo() {
            std::ifstream myFile("GuestInformation.csv");
            
            if (!myFile.is_open()) {
                std::cerr << "Unable to open GuestInformation.csv" << std::endl;
                return;
            }
            
            std::string line;
            bool headerSkipped = false;
            
            while (std::getline(myFile, line)) {
                if (!headerSkipped) {
                    headerSkipped = true;
                    continue;  // Skip header line
                }
                
                std::stringstream ss(line);
                std::string custID, name, email, phone, country, zip;
                
                // Parse CSV: CustomerID,RegistrantName,Email,PhoneNumber,Country/Region,ZIP,...
                if (std::getline(ss, custID, ',') &&
                    std::getline(ss, name, ',') &&
                    std::getline(ss, email, ',') &&
                    std::getline(ss, phone, ',') &&
                    std::getline(ss, country, ',') &&
                    std::getline(ss, zip, ',')) {
                    
                    // Trim whitespace from custID
                    custID.erase(0, custID.find_first_not_of(" \t"));
                    custID.erase(custID.find_last_not_of(" \t") + 1);
                    
                    // Check if this is the logged-in customer
                    if (custID == finalId) {
                        // Trim whitespace from all fields
                        name.erase(0, name.find_first_not_of(" \t"));
                        name.erase(name.find_last_not_of(" \t") + 1);
                        email.erase(0, email.find_first_not_of(" \t"));
                        email.erase(email.find_last_not_of(" \t") + 1);
                        phone.erase(0, phone.find_first_not_of(" \t"));
                        phone.erase(phone.find_last_not_of(" \t") + 1);
                        country.erase(0, country.find_first_not_of(" \t"));
                        country.erase(country.find_last_not_of(" \t") + 1);
                        zip.erase(0, zip.find_first_not_of(" \t"));
                        zip.erase(zip.find_last_not_of(" \t") + 1);
                        
                        // Assign to member variables
                        this->registrantName = name;
                        this->emailAddress = email;
                        this->phoneNumber = phone;
                        this->countryOrRegion = country;
                        this->zipCode = std::stoi(zip);
                        
                        myFile.close();
                        return;
                    }
                }
            }
            
            myFile.close();
        }


//Getter Functions

        std::string getCustomerID() const {
            return finalId; }

         std::string getregistrantName() const {
            return registrantName; }
         
         std::string getemailAddress() const {
             return emailAddress; }
            
         std::string getphoneNumber() const {
             return phoneNumber; }
             
         std::string getcountryOrRegion() const {
             return countryOrRegion;}
             
         int getzipCode() const {
             return zipCode;}
             
         std::string getpassWord() const {
             return passWord;}


    };
#endif