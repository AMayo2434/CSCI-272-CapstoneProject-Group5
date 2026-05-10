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
        void generateCustomerID() {
            // Code to generate a unique customer ID
            srand(time(0)); //Creating a seed for the random number generator using the current time
            long long int randomNum =((long long int)rand() << 32) | rand(); //Generates a random number by combining two calls to rand()
            std::cout << std::fixed << std::setprecision(10);
            std::cout << "Your customer ID is: " << randomNum << std::endl;
            
            customerID = randomNum;

            // ADD CSV SAVE FUNCTION HERE.
            //Adds customer ID to all relevant CSV files to track the guest's 
            //Once an ID is created, the customer must create an account. 
            //Customer IDs with no corresponding account information will be deleted from the CSV file after a certain amount of time. 
            


            //Resets random number assignment for next use. Customer ID will be saved to CSV file.
            randomNum = 0; 
        
        }



        void createAccount() {

            std::cout << "We are excited to have you join our community. Please follow the prompts to create your account." << std::endl;



            std::cout << "Please enter your name: " << std::endl;
            std::getline (std::cin, registrantName);
            
            // Save to CSV function.

            //Makke sure input follows email conventions
            //Need to add in email conventions
            std::cout << "Please enter your email address: " << std::endl;
            std::getline (std::cin, emailAddress);
            


            //Singular input, no spaces. Must be 10 digits long.
            std::cout << "Please enter your phone 10-digit phone number: " << std::endl;
            std::cin >> phoneNumber >> std::fixed >> std::setprecision(10);



            //Registers the user's country or region. 
            std::cout << "Please enter your country or region: " << std::endl;
            std::getline (std::cin, countryOrRegion);



            //Ensure this only allows for 5-digit zip codes. Singular input, no spaces.
            std::cout << "Please enter your zip code. Only the first 5 digits of your entry will be recorded: " << std::endl;
            std::cin >> zipCode >> std::fixed >> std::setprecision(5);


            
            //Registers user password. Needs to be stored in an encrypted file.
            std::cout << "Please create a password for your account. It cannot include spaces and it must be at least 12 characters long: " << std::endl;
            std::cin >> passWord;
            while (passWord.length() < 12) {
                std::cout << "Your password must be at least 12 characters long. Please try again: " << std::endl;
                clearError();
                std::cin >> passWord;

            }

        }

        void viewAccount() {
            // Code to display account information
        }
        

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

/*
What was I doing here lmao? Come back if needed IG?
for (const auto&[key, value] : guestInfo) {
    std::cout << "Name: " << key << std::endl;

    */


        void saveGuestInformation(std::string fileName) {
            // Code to save guest information to a file
            std::map<std::string, std::vector<std::string>> guestInfo; // Map to store guest information 
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