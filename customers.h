// Specification file for the Customers class.

#ifndef CUSTOMERS_H
#define CUSTOMERS_H
#include <string>
using namespace std;

class customers {
private: 
	// Variables for storing the information of customer's found
	string c_id,			// Customer ID
		c_name,				// Customer name
		chin_date,			// Check-in date (mm-dd-yyyy)
		chin_time,			// Check-in time (hh:mm:ss)
		chout_date,			// Check-out date (mm-dd-yyyy) 
		chout_time;			// Check-out time (hh:mm:ss)

	// Structure for customers' information
	struct customer_info {	
		string cid;			// Customer ID#
		string cname;		// Customer name
		string in_date;		// Check-in date		
		string in_time;		// Check-in time
		string out_date;	// Check-out date
		string out_time;	// Check-out time
	};

	// Variables for keeping all of the customers' information
	customer_info *all_customer;
	int customer_cnt;

	//Variables for keeping today and current time
	string cur_day;
	string cur_hour;
	string cur_min;

	string CUSTOMER_FILE = "customers.txt";

	void init_customer_arr(const string&);	// Function for stroing all the customers' informtion into the all_customer's array

public:
	customers();					// Default constructor
	customers(customers&);
	void set_customer(string&);		// Function for keeping the customer informtion matching with customer ID# the clerk input
	bool id_check(const string&);	// Custeomr ID# input validation
	bool isfound(const string&, customer_info*);	// Attempts to find out the customer's information, and to keeping the information
	bool check_stay(const customer_info*);	// Checks if both a within stay and overnight customer
	void set_cur();					// Function for getting today and current time and keeping them into the variables
	void print_c_info() const;						// Display the cusomer's information
	void print_c_info(const customer_info*) const;	// Display the cusomer's information

	int get_days_left(const string&, const string&);

	string get_c_id() const;			// Returns the customer ID #
	string get_c_name() const;		// Returns the customer name
	string get_c_name(const string&);
	string get_chin_date() const;		// Returns the check-in date
	string get_chin_time() const;		// Returns the check-in time
	string get_chout_date() const;	// Returns the check-out date
	string get_chout_date(const string&);	// Returns the check-out date
	string get_chout_time() const;	// Returns the check-out time
	string get_chout_time(const string&);	// Returns the check-out time
	string get_cur_day() const;
	string get_cur_hour() const;
	string get_cur_min() const;

	~customers();			// Desconstructor
};

#endif