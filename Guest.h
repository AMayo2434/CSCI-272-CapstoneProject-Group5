#ifndef GUEST_H
#define GUEST_H


#include "functions.h"
#include <ctime>
#include <map>
#include <vector>
#include <fstream>
#include <sstream>
#include <limits>
// All functions available to the guest/user class when they initially create an account.
class Guest {

    private:
        long long int customerID; //Remains private as this information references all data for the guest.
        std::string accountCreationDate;

    protected:
        std::string registrantName;
        std::string emailAddress;
        long long int phoneNumber;
        std::string countryOrRegion;
        int zipCode;
        std::string ZIP;
        std::string passWord;

    public:

        Guest() {
            customerID = 0;
            registrantName = "";
            emailAddress = "";
            phoneNumber = 0;
            countryOrRegion = "";
            zipCode = 0;
            passWord = "";
        
        }

// COMPLETE 
//Verifies if another exists.
        void generateCustomerID() {
            // Code to generate a unique customer ID
            std::cout << std::fixed << std::setprecision(10);
            long long int randomNum =((long long int)rand() << 32) | rand(); //Generates a random number by combining two calls to rand()

            std::string convert;
            customerID = randomNum;

            convertToStringL(customerID, convert);

//Checks to see if there is a duplicate value.
              if (isDuplicate("GuestInformation.csv", convert)) {
                    std::cout << "ID exists. Re-running ID creation." << std::endl;
                    generateCustomerID();// INSERT WAY TO CHECK 
                } 
                else {
                    std::cout << "Your customer ID is: " << convert << std::endl;
                }

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

        void createAccount() 
        {

            std::cout << std:: setw(10) << "We are excited to have you join our community. Please follow the prompts to create your account." << std::endl;

            std::cout << "Please enter your name: ";
            std::getline(std::cin >> std::ws, registrantName);

//Make sure input follows email conventions
//Need to add in email conventions if I have time
            std::cout << "Please enter your email address: " << std::endl;
            std::getline(std::cin, emailAddress);

// Phone Number
//Singular input, no spaces. Must be 10 digits long.
            std::cout << "Please enter your 10-digit phone number: " << std::endl;
            std::cin >> phoneNumber;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); 

// Country
// Registers country or region
            std::cout << "Please enter your country or region: " << std::endl;
            std::getline(std::cin, countryOrRegion);

// Zip Code Validation (Must be 5 digits)
std::cout << "Please enter your 5-digit zip code: " << std::endl;
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
        }

               


//Not sure if this is needed anymore, will review
        void parseGuestInfo(const std::string& fileName, std::map<std::string, std::vector<int>>& guestInfo) {
            std::cout << fileName << std::endl;
            std::ifstream myFile(fileName);
            if(myFile.is_open()) {
                std::string line;
                while(std::getline(myFile, line)) {
                    //cout << line << endl;
                    std::stringstream ss(line);
                    std::string key; // Placeholder val... I mean it works,
                    getline(ss, key, ',');
                    //cout << g << endl; // Get the first value as the key
                    guestInfo.insert({key, std::vector<int>()}); // Insert the key with an empty vector as the value})

                    std::string value;
                    while(getline(ss, value, ',')) {
                        guestInfo[key].push_back(std::stoi(value));
                    }

                    }
                    myFile.close();
                }
            
            else {
                std::cout << "Unable to open file: " << fileName << std::endl;
            }
            // Code to read guest information from a file and populate the class attributes
        }



    // Destructor to clear guest information from memory
        ~Guest() 
        {
            // Clears the guest information from memory when the object is destroyed
            //Resets all values at the end of the program. There is no need to keep the information in variables
            //because the information is stored in a file.
            customerID = 0;
            registrantName = "";
            emailAddress = "";
            phoneNumber = 0;
            countryOrRegion = "";
            zipCode = 0;
            passWord = "";
        }


    };
#endif