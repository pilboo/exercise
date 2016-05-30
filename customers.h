// Specification file for the Customers class.

#ifndef CUSTOMERS_H
#define CUSTOMERS_H
#include <string>
using namespace std;

class customers {
private: 
	string c_id,			// Customer ID
		c_name,				// Customer name
		chin_date,			// Check-in date (mm-dd-yyyy)
		chin_time,			// Check-in time (hh:mm:ss)
		chout_date,			// Check-out date (mm-dd-yyyy) 
		chout_time;			// Check-out time (hh:mm:ss)
	string c_info;			// Keep a customer's information
	bool isfound;			// Flag of whether a customer information exists in the customer data or not
public:
	customers();			// Default constructor
	void get_c_info(string&);	// Compare an input with customer data, then return customer information
	void set_customers();		// From customer information, set all member variables
	string get_c_id();			// Returns the customer ID #
	string get_c_name();		// Returns the customer name
	string get_chin_date();		// Returns the check-in date
	string get_chin_time();		// Returns the check-in time
	string get_chout_date();	// Returns the check-out date
	string get_chout_time();	// Returns the check-out time
	bool get_isfound();			// Returns the flag
	void print_c_info() const;	// Display the cusomer's information
	~customers();			// Desconstructor
};

#endif