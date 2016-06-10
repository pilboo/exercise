// MiYE: Spa management program

#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
#include "customers.h"
#include "services.h"
#include "reservations.h"
using namespace std;

int main() {
	string main_choice, customer_choice, service_choice;
	
	// Main menu
	// 1. Service Reservation
	// 2. Produce Service Bill
	// 3. Maintenance

	// Check customer validity
	customers customer;		// Create customers class
	customer.set_customer(customer_choice);	// Check the customer's validity and store the customer's information
	string customer_id;

	if (toupper(customer_choice.at(0)) != 'M') {
		customer_id = customer.get_c_id();
		
		string display_opt;
		bool isok = false;
		
		
		services service;
		string service_id, type_id, time_id;
		
		reservations reservation;
		reservation.set_customer_id(customer.get_c_id());
		do {
			cout << "\n\n\t=======================================================";
			cout << "\n\t\tDISPLAY OPTION";
			cout << "\n\t-------------------------------------------------------";
			cout << "\n\t 1. Input Service ID#";
			cout << "\n\t 2. View All Avaiable Services at a Specific Time";
			cout << "\n\t 3. View available times for a particular service";
			cout << "\n\t 4. View available time slots by the customer's checkout";
			cout << "\n\t-------------------------------------------------------";
			cout << "\n\tEnter your option (1-4, 0 for main menu): ";
			cin >> display_opt;

			if (display_opt.length() == 1 && isdigit(display_opt.at(0))) {
				switch (stoi(display_opt)) {
				case 0:
					cout << "\n\t=== Returning to Main Menu ===\n";
					service_choice = "M";
					isok = true;
					break;
				case 1:
					service.select_service(service_choice);
					if (toupper(service_choice.at(0)) != 'D') {
						service_id = service.get_service_id();
						type_id = service.get_type_id();
						time_id = service.get_time_id();
						
						// Input a day for the service
						reservation.set_reservation_day();
						reservation.set_begin_end_time();
						// Print a calendar of the day
						reservation.print_calendar_header(service_id, type_id, time_id);
						reservation.print_calendar_body(service_id, type_id, time_id);
						// Input a time for the service
						reservation.set_reservation_time(service_id, type_id, time_id);
						// Make a reservation
						reservation.reserve(service_id, type_id, time_id);
					}
					break;
				case 2:
					reservation.set_reservation_day();
					reservation.set_reservation_time();
					reservation.print_available_services();
					reservation.input_services();
					break;
				case 3:
				case 4:
				default:
					cout << "\n\n\t=== Please enter a valid number (0-4) ===\n\n";
				}
			}
		} while (!isok);
	}

	return 0;
}