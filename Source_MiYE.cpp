// This program allows the clerks to input a cutomer ID
// then searches the ID from the Customers file.

#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
#include "customers.h"
using namespace std;

int main() {
	
	// Check customer validity

	string input_id, customer_id;	// Variable for user input
	customers customer;				// Create customer class
	bool c_found;					// Variable for whether a customer data exists
	bool c_valid = false;			// Flag for the customer validity

	cout << "Enter customer ID#: "; 
	cin >> input_id;

	customer.get_c_info(input_id);		// Trying to find a customer data from customer.txt
	c_found = customer.get_isfound();	// Set c_found from the result of search
	
	if (!c_found) {					// If not found
		cout << "Please ask the Hotel Front Desk.\n";
	}
	else {							// If the customer information found
		customer.set_customers();			// Store the customer information into customer class
		customer_id = customer.get_c_id();	// Get the customer ID#
		customer.print_c_info();			// Print the customer informtion

		// Overnight?
		string chin = customer.get_chin_date(),	// Variable for check-in date
			chout = customer.get_chout_date();	// Variable for check-out date
		
		if (chin == chout) {	// If check-in data is same as check-out data (not overnight)
			cout << "\nThe customer #" << customer_id << " is not overnight customer.\n";
		}
		else {					// If the customer is an overnight customer, check within stay
			time_t current_time;
			struct tm *local_time;

			time(&current_time);
			local_time = localtime(&current_time);

			int c_day = local_time->tm_mday;			// Get the day of this month
			int c_month = local_time->tm_mon + 1;		// Get this month
			int c_year = local_time->tm_year + 1900;	// Get this year

			string t_today		// Set today as a format of mm-dd-yyyy
				= ((c_month < 10) ? "0" : "") + to_string(c_month)	// If the value of month is smaller than 10, add "0" in front of the month (ex. 3 -> 03)
				+ "-" + ((c_day < 10) ? "0":"") + to_string(c_day)	// If the value of day is smaller that 10, add "0" in front of the day
				+ "-" + to_string(c_year);
			
			if (!(chin <= t_today && chout >= t_today)) {	// Check unless check-in is eariler than today and check-out is later than today
				cout << "\nThe customer #" << customer_id << " is not within stay customer.\n";
			}
			else {
				c_valid = true;
				cout << "\nThe customer #" << customer_id << " is valid to reserve service.\n";
			}
		}
	}
	
	// Check service availability

	if (c_valid) {	// If the customer is valid

	}


	return 0;
}
