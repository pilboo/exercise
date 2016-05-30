// MiYE: Spa management program

#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
#include "customers.h"
#include "services.h"
#include "reservations.h"
using namespace std;

void get_today(string&, string&, string&);
void print_services();
//void print_reservation(string&, r_line*,int);

int main() {
	// Check customer validity
	// This part allows the clerks to input a cutomer ID then searches the ID from the Customers file.
	string input_id, customer_id;	// Variable for user input
	customers customer;				// Create customers class
	string SERVICE_FILE = "services.txt";
	string RESERVATION_FILE = "reservations.txt";

	bool c_valid = false;			// Flag for the customer validity
	string t_today, t_hour, t_min;	// Variables for today, hour, and minute	
	string chin, chout;				// Variables for storing the check-in and check-out date

	// Allow the user to input customer ID
	cout << "Enter customer ID#: "; 
	cin >> input_id;

	customer.get_c_info(input_id);		// Trying to find a customer data from customer.txt
	
	if (!customer.get_isfound()) {					// If not found
		cout << "Please ask the Hotel Front Desk.\n";
	}
	else {							// If the customer information found
	
		customer.set_customers();			// Store the customer information into customer class
		customer_id = customer.get_c_id();	// Get the customer ID#
		customer.print_c_info();			// Print the customer informtion

		chin = customer.get_chin_date();	// Variable for check-in date
		chout = customer.get_chout_date();	// Variable for check-out date
		
		get_today(t_today, t_hour, t_min);	// Set today as a format of mm-dd-yyyy

		// Check if a within-stay customer
		if (!(chin <= t_today && chout >= t_today)) {
			cout << "\nNo within stay customer.\n";
		} else {

			if (chin == chout) {	// If check-in data is same as check-out data (not overnight)
				cout << "\nNo overnight customer.\n";
			}
			else {					// The customer is a within-stay and overnight customer
				cout << "\nValid to reserve services\n";
				c_valid = true;
			}
		}
	}
	
	// Check service availability and customer availability
	if (!c_valid) {
		cout << "The customer #" << customer_id << " cannot reserve spa services\n";
	} else {	// If the customer is valid, check service availability

		// Display services and let the clerk make a choice
		string s_choice;	// Variable for saving the choice of service
		services service(SERVICE_FILE);	// Create services class
		
		bool iscorrect = false;
		do {
			cout << "\n\nSelect the service.\n";
			service.print_services();
			cout << "\nPlease Enter the service ID: ";
			cin >> s_choice;

			if (!(iscorrect = service.chk_serviceid(s_choice))) {
				cout << "\nPlease Enter valid service ID";
			}
		} while (!iscorrect);

		//Allow the user to select service types
		string t_choice;
		service.print_types(s_choice);
		cout << "\n\tPlease enter the type ID: ";
		cin >> t_choice;

		// Display the service calendar
		string r_date;		// Variable for the date of reservation

		cout << "\n=== Select the day of reservation ===\n";
		do {
			cout << "\nThe reservation can be made from " << t_today << " to " << chout << endl;
			cout << "Please enter the date of reservation (mm-dd-yyyy): ";
			cin >> r_date;

			if (!((r_date >= t_today) && (r_date <= chout))) {	// If the date of reseravtion is before today or after check-out date
				cout << "\n[ERROR] The date of reservation must be later than or the same as today, and \n";
				cout << "earlier than or the same as the customer's check-out date.\n\n";
			}
		} while (!((r_date >= t_today) && (r_date <= chout)));

		reservations r_class(RESERVATION_FILE);
		int b_hour = 8,	// Variables for the beginning and ending time of the calendar
			b_min = 0,
			e_hour = 20,
			e_min = 0;

		if (r_date == t_today) {
			b_hour = stoi(t_hour);
			b_min = 0;

			(stoi(t_min) > 30) ? b_hour++ : b_min = 30;
		}
		else if (r_date == chout) {
			e_hour = stoi(customer.get_chout_time().substr(0, 2));
			e_min = stoi(customer.get_chout_time().substr(3, 2));
		}
		
		string s_name = service.get_servicename(s_choice);
		string ty_name = service.get_servicetype(s_choice, t_choice);

		cout << "\n\t\t" << s_name << ((ty_name.length() > 0) ? "-" : "") << ty_name;
		cout << ", CUSTOMER: " << customer.get_c_name();
		cout << "\n\t==========================================================";
		cout << "\n\tTIME\tSERVICE AVALABLBE\tCUSTOMER AVAILABLE?";
		cout << "\n\t==========================================================";
					 
		

		for (int i = b_hour; i <= e_hour; i++) {
			string cal_time;
			cal_time = ((i < 10) ? "0" : "") + to_string(i) + ":" + ((b_min == 0) ? "0" : "") + to_string(b_min);
				
			cout << "\n\t" << cal_time;
			r_class.print_service_cal(s_choice, t_choice, r_date, cal_time);
			r_class.print_customer_cal(customer_id, r_date, cal_time);
			//cout << "\n\t----------------------------------------------------------";

			if (i != e_hour && b_min == 0) {
				b_min = 30;
				cal_time = ((i < 10) ? "0" : "") + to_string(i) + ":" + to_string(b_min);
					
				cout << "\n\t" << cal_time;
				r_class.print_service_cal(s_choice, t_choice, r_date, cal_time);
				r_class.print_customer_cal(customer_id, r_date, cal_time);
				//cout << "\n\t----------------------------------------------------------";
			}
			b_min = 0;
		}
		cout << "\n\t==========================================================";

		string app_time, tm_choice;
		int service_tm;
		bool is_timeok = false; 

		do {
			cout << "\n\n\tPlease Enter the time to make a reservation (hh:mm - e.g., 13:00): ";
			cin >> app_time;

			cout << "\n=== SERVICE DURATION TIME ===";
			service_tm = service.print_times(s_choice, t_choice);
			cout << "\n\tPlease enter the time ID: ";
			cin >> tm_choice;

			if (!(is_timeok = r_class.chk_apptime(app_time, service_tm, b_hour, b_min, e_hour, e_min))) {
				cout << "\n\t=== [ERROR] Please enter an available time slot ===";
			}
		} while (!is_timeok);

		get_today(t_today, t_hour, t_min);
		string c_time = t_hour + ":" + t_min;

		r_class.reserve(customer_id, s_choice, t_choice, tm_choice, r_date, app_time, service_tm, t_today, c_time);

	}
	return 0;
}

void get_today(string &r_today, string &r_hour, string &r_min) {
	time_t current_time;
	struct tm *local_time;
	
	time(&current_time);
	local_time = localtime(&current_time);

	int c_min = local_time->tm_min;				// Get the current minutes
	int c_hour = local_time->tm_hour;			// Get the current hours
	int c_day = local_time->tm_mday;			// Get the day of this month
	int c_month = local_time->tm_mon + 1;		// Get this month
	int c_year = local_time->tm_year + 1900;	// Get this year

	r_today = ((c_month < 10) ? "0" : "") + to_string(c_month)	// If the value of month is smaller than 10, add "0" in front of the month (ex. 3 -> 03)
		+ "-" + ((c_day < 10) ? "0" : "") + to_string(c_day)	// If the value of day is smaller that 10, add "0" in front of the day
		+ "-" + to_string(c_year);

	r_hour = ((c_hour < 10) ? "0": "" ) + to_string(c_hour);
	r_min = ((c_min < 10) ? "0" : "" ) + to_string(c_min);

	return;
}