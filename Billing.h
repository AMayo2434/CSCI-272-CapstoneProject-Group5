#ifndef BILLING_H
#define BILLING_H

#include "Frontdesk.h"
#include <iostream>
#include <fstream>
#include <string>
#include <numeric>
#include <iomanip>
#include <cstdlib>
#include <cmath>


class Billing {

private:

	string guestName; // Removing bc it's going to get inherited, staying for now.
	string dueDate;//Registrant name will have a seperate string included for Billing
	int roomNumber; // References the room number for the bill
	int nights; // Number of nights the room was occupied.
	double amount;
	double roomRate; // Rate of the room stayed - will be included with RM class
	double totalAmount; // Final price of the bill. Cleared once the bill is calculated
	double totalRevenue; // Adds up all transactions unless cleared.
	std::vector<double>payments;

public:

	//Constructor to initalize base value of the class.
	Billing () : totalRevenue(0.0) {}

	//Adds the abiity to add payment info.
	void addPayment(double amount) {
		if (amount > 0) {
			payments.push_back(amount);
			totalRevenue += amount;
			std::cout << "Thank you for your payment amount of $" << amount << "." << endl;
		}
	}


	//Calculates the bill based off Employee input.
	double calculateBill(double roomRate, int nights) {
		return roomRate * nights;

	}

	//Displays payment history of the guest. Using unordered map for retrival
	void displayPaymentInfo() const {
		std::cout << "Payment History" << endl;
		for (double p : payments) {
			std::cout << "$" << p << "\n";
		}

	}

	//Displays any active bills that have a balance from the CSV file.
	//Using unordered map based on "Past due" date to display late or active bills.

	void displayActiveBills() const {
		std::cout << "Displaying bills with a non-zero balance..." << endl;


	}


	//Saves information from bill to the CSV. Will be added in at the end.
	void saveBillInfo() {

		std::ofstream hotelBills{"Billing.csv", ios::out}; //Creates required CSV file.

		if(!hotelBills) { // Checks to ensure the correct file is opened.
			std::cerr << "File unavailable. Please try again or contact the administrator.";
			std::exit(EXIT_FAILURE);
		}


		std::unordered_map<std::string, std::string> dataMap;
		std::ifstream file("Billing.csv");
		std::string line, key, value;

		if (file.is_open()) {
			while (std::getline(file, line)) {
				std::stringstream ss(line);

				// Extract key and value separated by a comma
				if (std::getline(ss, key, ',') && std::getline(ss, value)) {
					dataMap[key] = value; // Store in map
				}
			}
			file.close();
		}


	}


};

#endif