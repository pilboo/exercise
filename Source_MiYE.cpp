// MiYE: Spa management program

#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
#include "customers.h"
#include "services.h"
#include "reservations.h"
#include "misc.h"
using namespace std;

int main() {
	string main_choice;		// For the choice of main menu
	string customer_choice;	// Keeps the clerk's input for the customer ID#
	string service_choice;	// Keeps the clerk's input for the service ID
	
	misc misc_obj;

	bool flag_for_main = true;
	// Main menu
	do {
		system("cls");
		cout << "\n\n\t=======================================================";
		cout << "\n\n\t                    Welcome to MiYE";
		cout << "\n\n\t=======================================================";
		cout << "\n\t                       MAIN MENU";
		cout << "\n\t-------------------------------------------------------";
		cout << "\n\t 1. Service Reservation";
		cout << "\n\t 2. Produce Service Bill";
		cout << "\n\t 3. Maintenance";
		cout << "\n\t Q. Quit the Program";
		cout << "\n\t-------------------------------------------------------";
		cout << "\n\tEnter Your Choice (1-3, Q to quit): ";
		cin >> main_choice;

		if (!misc_obj.check_main_choice(main_choice)) {
			cout << "\n\n\t=== [ERROR] Please Enter a Valid Menu ===\n";
			flag_for_main = false;
		}
		else {
			customers customer;
			string customer_id;

			switch (main_choice.at(0)) {
			case '1':
				// Check customer validity
				// Create customers class
				customer.set_customer(customer_choice);	// Check the customer's validity and store the customer's information
				
				if (toupper(customer_choice.at(0)) != 'M') {
					customer_id = customer.get_c_id();

					string display_opt;
					bool isok = false;

					services service;
					string service_id, type_id, time_id;

					reservations reservation;
					reservation.set_customer_id(customer_id);
					do {
						cout << "\n\n\t=======================================================";
						cout << "\n\t                    DISPLAY OPTION";
						cout << "\n\t-------------------------------------------------------";
						cout << "\n\t 1. Input Service ID#";
						cout << "\n\t 2. View All Avaiable Services at a Specific Time";
						cout << "\n\t 3. View available times for a particular service";
						cout << "\n\t 4. View available time slots by the customer's checkout";
						cout << "\n\t-------------------------------------------------------";
						cout << "\n\tEnter One Option (1-4, 0 for main menu): ";
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
									reservation.set_begin_end();
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
								break;
							case 4:
								reservation.print_available_time();
								reservation.set_reservation_day();
								reservation.set_reservation_time();
								reservation.print_available_services();
								reservation.input_services();
								break;
							default:
								cout << "\n\n\t=== Please enter a valid number (0-4) ===\n\n";
							}
						}
					} while (!isok);
				}
				else {
					flag_for_main = false;
					//break;
				}
				break;
			case '2': //
				break;
			case '3': //
				break;
			case 'Q':
			case 'q':
				system("cls");
				cout << "\n\n\tQuitting...";
				cout << "\n\n\tEXIT MiYE" << endl;
				flag_for_main = true;
				break;
			default:
				cout << "\n\n\t=== [ERROR] Please Enter a Valid Menu ===\n";
			} // End of switch(main_choice)
		} // End of if (check_main_choice ... )
	} while (!flag_for_main); // End of main menu

	return 0;
}