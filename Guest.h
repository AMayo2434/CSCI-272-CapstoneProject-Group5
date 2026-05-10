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

    //Constuctor
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

            //Looked up information on random library
            //Random device creates a new set of numbers upon each run, reducing the chance of repeats
           static std::random_device ran;
           static std::mt19937_64 gen(ran()); 
            //Range from 1 Bil to 9.999 Bil 
           std::uniform_int_distribution<long long> dis (1000000000LL, 9999999999LL);

            long long randomNum = dis(gen);

            std::string convert = std::to_string(randomNum);

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
        
        }

    bool signIn() {
        std::string enterCustomerID, enterPassWord;
        std::string csvCustomerID, csvPass;
        bool loggedIn = false;

        std::setw(30);
        centerText(" Sign In To Access All Guest Services Associated With Your Account", 100, ('='));

        std::cout << "Please enter your CustomerID to get started: ";
        std::cin >> enterCustomerID;
        std::cout << std::endl;

        std::cout << "Please enter your password: ";
        std::cin >> enterPassWord;
        std::cout << std::endl;

        std::ifstream file("VerificationInfo.csv");
        if(!file.is_open()) {
            std::cout << "Error. Unable to open file." << std::endl;
            return false;
        }

        std::string loginline;
        while(std::getline(file,loginline)) {
            std::stringstream ss(loginline);
            std::string temp;

            std::getline(ss, csvCustomerID, ',');
            std::getline(ss, csvPass, ',');

            if (csvCustomerID == enterCustomerID && csvPass == enterPassWord){
                loggedIn = true;
                this->customerID = std::stoll(csvCustomerID); //Converts strin into a signed long long int.
                this->passWord = csvPass;
                break;
            }
        }
        file.close();

        if(loggedIn) {

           std::cout << "Sign in successful! Proceeding to portal.";
        }

        else {
            std::cout << "Invalid email or password. Please try again." << std:: endl;
            return false;
        }

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