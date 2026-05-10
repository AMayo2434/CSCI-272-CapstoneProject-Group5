#ifndef GUEST_H
#define GUEST_H


#include "functions.h"
#include <ctime>
#include <map>
#include <vector>
#include <fstream>
#include <sstream>

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
//Verifies if anther exists.
        void generateCustomerID() {
            // Code to generate a unique customer ID
            srand(time(0)); //Creating a seed for the random number generator using the current time
            long long int randomNum =((long long int)rand() << 32) | rand(); //Generates a random number by combining two calls to rand()
            std::cout << std::fixed << std::setprecision(10);

            std::string convert;
            customerID = randomNum;

            convertToStringL(customerID, convert);

            //Checks to see if there is a duplicate value.
              if (isDuplicate("GuestInformation.csv", convert)) {
                    std::cout << "ID exists." << std::endl;
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




        void createAccount() {

            std::cout << "We are excited to have you join our community. Please follow the prompts to create your account." << std::endl;
            std::cout << "We are excited to have you join our community." << std::endl;
            
            std::cout << "Please enter your name: ";
            std::getline (std::cin, registrantName);

            //Makke sure input follows email conventions
            //Need to add in email conventions
            std::cout << "Please enter your email address: " << std::endl;
            std::getline (std::cin, emailAddress);

            //Singular input, no spaces. Must be 10 digits long.
            std::cout << "Please enter your phone 10-digit phone number: " << std::endl;
            std::cin >> phoneNumber;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // FIX: Clear buffer
  
            //Registers the user's country or region. 
            std::cout << "Please enter your country or region: " << std::endl;
            std::getline (std::cin, countryOrRegion);

            //Ensure this only allows for 5-digit zip codes. Singular input, no spaces.

            std::cout << "Please enter your zip code: " << std::endl;
            std::cin >> zipCode;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // FIX: Clear buffer

            std::cout << "Please create a password (at least 12 characters): " << std::endl;
            std::getline(std::cin, passWord); // Use getline for safety

            while (passWord.length() < 12) {
                std::cout << "Too short. Try again: " << std::endl;
                std::getline(std::cin, passWord);
            }

                    //Registers user password. Needs to be stored in an encrypted file.
                    std::cout << "Please create a password for your account. It cannot include spaces and it must be at least 12 characters long: " << std::endl;
                    std::cin >> passWord;
                    while (passWord.length() < 12) {
                        std::cout << "Your password must be at least 12 characters long. Please try again: " << std::endl;
                        clearError();
                        std::cin >> passWord;

                    }

            writeRowToCSV("GuestInformation.csv", registrantName, emailAddress, phoneNumber, countryOrRegion, zipCode);
            writeRowToCSV("Verification.csv", passWord);

        }

        void viewAccount() {
            // Code to display account information
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
                    std::string key; // Placeholder val
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