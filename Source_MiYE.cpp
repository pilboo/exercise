// MiYE: Spa management program

#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
#include "customers.h"
#include "services.h"
#include "reservations.h"
#include "cancel_reservation.h"
#include "bills.h"
using namespace std;

int main() {
	string op_choice;
	bool flag_for_op = false;

	do {	// Operation Menu
		cout << "\n\n\t=======================================================";
		cout << "\n\n\t                    Welcome to MiYE";
		cout << "\n\n\t=======================================================";
		cout << "\n\t                    OPERATION MENU";
		cout << "\n\t-------------------------------------------------------";
		cout << "\n\n\t 1. For Spa Management\t\t2. For Maintenance";
		cout << "\n\n\n\t 0. Quit the Program";
		cout << "\n\n\t-------------------------------------------------------";
		cout << "\n\tEnter Your Choice (1-2 or 0 to quit): ";
		getline(cin, op_choice);

		if (op_choice.length() == 1 && isdigit(op_choice.at(0))) {
			customers customer;
			string customer_id;

			services service;
			string service_id, type_id, time_id;

			reservations reservation;

			bool flag_for_customer = false;
			string customer_choice;	// Keeps the clerk's input for the customer ID#

			switch (stoi(op_choice)) {	// Operation menu
			case 0:		// Operation menu #0. Quit the program
				cout << "\n\n\tQuitting...";
				cout << "\n\n\tEXIT MiYE" << endl;
				flag_for_op = true;
				break;
			case 1:		// Operation menu #1. For spa management
				do {	// For the customer ID# input
					cout << "\n\n\tEnter the Customer ID# (000 - 999 or 0 for Operation Menu): ";
					getline(cin, customer_choice);

					if (customer_choice == "0") {
						flag_for_customer = true;
						cout << "\n\t=== Returning to Operation Menu ===\n";
						break;
					}	// End of if ((customer choice == "Q") ...)
					else if (customer.set_customer(customer_choice)) {
						
						customer_id = customer.get_c_id();
						reservation.set_customer_id(customer_id);					

						string main_choice;		// For the choice of main menu
						bool flag_for_main = false;

						do {	// Main menu
							cout << "\n\n\t=======================================================";
							cout << "\n\n\t                    Welcome to MiYE";
							cout << "\n\n\t=======================================================";
							cout << "\n\t                       MAIN MENU";
							cout << "\n\t-------------------------------------------------------";
							cout << "\n\t 1. Reserve the Service";
							cout << "\n\t 2. Cancel the reservation(s)";
							cout << "\n\t 3. Produce the Service Bill";
							cout << "\n\t";
							cout << "\n\t 0. Input Customer ID#";
							cout << "\n\t-------------------------------------------------------";
							cout << "\n\tEnter Your Choice (1-3 or 0 for Customer ID# Input): ";
							getline(cin, main_choice);

							if (main_choice.length() == 1 && isdigit(main_choice.at(0))) {

								string res_opt;
								bool flag_for_resopt = false;
								cancel_reservation cancel;
								bills bill;

								switch (stoi(main_choice)) {
								case 0:	// Main menu #0. Input customer ID#
									flag_for_main = true;
									cout << "\n\t=== Returning to Customer ID# Input ===\n";
									break;
								case 1:	// Main menu #1. Reserve the service
									do {	// Reservatio option
										cout << "\n\n\t=========================================================";
										cout << "\n\t                    RESERVATION OPTION";
										cout << "\n\t---------------------------------------------------------";
										cout << "\n\t 1. Input Service ID#";
										cout << "\n\t 2. View All Avaiable Services at a Specific Time";
										cout << "\n\t 3. View Available Times for a Service during Given Time";
										cout << "\n\t 4. View Available Time Slots by the Time of Checkout";
										cout << "\n\t";
										cout << "\n\t 0. Return to Main Menu";
										cout << "\n\t---------------------------------------------------------";
										cout << "\n\tEnter One Option (1-4 or 0 for Main Menu): ";
										getline(cin, res_opt);

										if (res_opt.length() == 1 && isdigit(res_opt.at(0))) {

											string service_choice;	// Keeps the clerk's input for the service ID
											string date_input;
											bool flag_service_choice = false;

											switch (stoi(res_opt)) {
											case 0:	// Reservation option #0. Return to main menu
												cout << "\n\t=== Returning to Main Menu ===\n";
												flag_for_resopt = true;
												break;
											case 1:	// Reservation option #1. Input service ID#
												do {	// Input service ID#
													service.print_service_choice(service_choice);
													if (!(flag_service_choice = service.check_service_choice(service_choice))) {
														service.select_service(service_choice);
														service_id = service.get_service_id();
														type_id = service.get_type_id();
														time_id = service.get_time_id();

														// Input a day for the service
														reservation.set_reservation_day(date_input);
														if (!service.check_service_choice(date_input)) {
															reservation.set_begin_end(reservation.get_reservation_day());

															// Print a calendar of the day
															reservation.print_calendar_header(service_id, type_id, time_id);
															reservation.print_calendar_body(service_id, type_id, time_id);

															// Input a time for the service
															reservation.set_reservation_time(service_id, type_id, time_id);

															// Make a reservation
															reservation.reserve(service_id, type_id, time_id);
															flag_service_choice = true;
														}
														else 
															break;
													} // End of if (serivce_choice == "Q" ... )
												} while (!flag_service_choice);
												break;
											case 2:	// Reservation option #2. View all availabe services at a specific time
												do {
													reservation.set_reservation_day(date_input);
													if (!service.check_service_choice(date_input)) {
														reservation.set_reservation_time();
														reservation.print_available_services();
														flag_service_choice = true;
													}
													else 
														break;																									
												} while (!flag_service_choice);
												break;
											case 3:	// Reservation option #3. View availabe times for a service during given times
												do {
													service.print_service_choice(service_choice);
													if (!service.check_service_choice(service_choice)) {
														service.select_service(service_choice);
														service_id = service.get_service_id();
														type_id = service.get_type_id();
														time_id = service.get_time_id();
														reservation.print_available_time(service_id, type_id);
														reservation.set_reservation_day(date_input);
														if (!service.check_service_choice(date_input)) {
															reservation.set_reservation_time(service_id, type_id, time_id);
															reservation.reserve(service_id, type_id, time_id);
															flag_service_choice = true;
														}
														else
															break;
													}
													else
														break;// End of if (servoce_choice == "Q" ...)
												} while (!flag_service_choice);
												break;
											case 4:	// Reservation option #4. View avaiable time slots by the time of checkout
												do {
													reservation.print_available_time();
													reservation.set_reservation_day(date_input);
													if (!service.check_service_choice(date_input)) {
														reservation.set_reservation_time();
														reservation.print_available_services();											
														flag_service_choice = true;
													}
													else 
														break;		
												} while (!flag_service_choice);
												break;
											default:
												cout << "\n\n\t=== [ERROR] Please Enter a Number (0-4) ===\n";
											}	// End of switch (stoi(res_opt))
										}
										else {	// Else of if (res_opt.length() == 1 ...) 
											cout << "\n\n\t=== [ERROR] Please Enter a Valid Reservation Option: A Digit Number (0-4) ===\n";
										} // End of if (res_opt.length() == 1 ...) 

									} while (!flag_for_resopt);		// End of reservation option
									break;
								case 2:	// Main menu #2. Cancel the reservation(s)
									do {
										cancel.set_customer_id(customer_id);
										reservation.print_available_time();
										cancel.set_cancel_date_time(res_opt);
										if (!service.check_service_choice(res_opt)) {
											cancel.cancel();
											flag_for_resopt = true;
										}
										else
											break;
									} while (!flag_for_resopt);
									break;
								case 3:	// Produce the servcie bill
									bill.set_customer_id(customer_id);	
									bill.print_bill();
									break;
								default:
									cout << "\n\n\t=== [ERROR] Please Enter a Number (0-3) ===\n";
								} // End of switch (stoi(main_choice))
							}
							else { // Else of if (main_choice.length() == 1 ...)
								cout << "\n\n\t=== [ERROR] Please Enter a Valid Main Menu: A Digit Number (0-3) ===\n";
							} // End of if (main_choice.length() == 1 ...)

						} while (!flag_for_main);	// End of main menu
					} // End of else if (custeomr.set_customer ...)) 
				} while (!flag_for_customer);	// End of customer ID# input
				break;
			case 2:		// Operation menu #2. For maintenance
				do {
					cout << "\n\n\t=======================================================";
					cout << "\n\n\t                MAINTENANCE for MiYE";
					cout << "\n\n\t=======================================================";
					cout << "\n\t 1. Manage Service Specification";
					cout << "\n\t 2. Report Service Usage";
					cout << "\n\t";
					cout << "\n\t 0. Return to the Operation Menu";
					cout << "\n\t-------------------------------------------------------";
					cout << "\n\tEnter Your Choice (1-2, 0 for Operation Menu): ";
					getline(cin, customer_choice);

					if (customer_choice.length() == 1 && isdigit(customer_choice.at(0))) {
						string mt_choice;
						bool flag_for_mt = false;

						switch (stoi(customer_choice)) {
						case 0:
							flag_for_customer = true;
							cout << "\n\t=== Returning to Operation Menu ===\n";
							break;
						case 1:
							do {
								cout << "\n\n\t=========================================================";
								cout << "\n\t              MANAGE SERVICE SPECIFICATION";								
								cout << "\n\t---------------------------------------------------------";
								service.print_services();
								cout << "\n\t---------------------------------------------------------";
								cout << "\n\t 1. Add a New Service";
								cout << "\n\t 2. Edit a Service";
								cout << "\n\t 0. Return to the Maintenance Menu";
								cout << "\n\t---------------------------------------------------------";
								cout << "\n\tEnter Your Choice: ";
								getline(cin, mt_choice);								

								if (mt_choice.length() == 1 && isdigit(mt_choice.at(0))) {									
									string m_service_choice;
									bool flag_for_service = false;;

									switch (stoi(mt_choice)) {
									case 0:
										flag_for_mt = true;
										cout << "\n\t=== Returning to Maintenance Menu ===\n";
										break;
									case 1:
										service.add_new_service();
										break;
									case 2:
										do {
											cout << "\n\tInput the Service ID#: ";
											getline(cin, m_service_choice);

											if (service.chk_serviceid(m_service_choice)) {
												service.edit_service(m_service_choice);

												flag_for_service = true;
											}
											else {
												flag_for_service = false;
											}
										} while (!flag_for_service);

										break;
									default:
										cout << "\n\n\t=== [ERROR] Please Enter a Number (0-2) ===\n";
									} // End of switch
								}	// End of if (mt_choice.length() == 1 && isdigit(mt_choice.at(0)))
								else {
									cout << "\n\n\t=== [ERROR] Please Enter a Valid Menu: A Digit Number (0-2) ===\n";
									flag_for_mt = false;
								}	// End of if (mt_choice.length() == 1 && isdigit(mt_choice.at(0)))
							} while (!flag_for_mt);
							break;
						case 2:

							break;
						default:
							cout << "\n\n\r\t [ERROR] Please Enter a Number (0-2) ===\n";
						} // End of switch (stoi(customer_choice)) 
					} // Else of if (customer_choice.length() == 1 && isdigit(customer_choice.at(0)))
					else {
						"\n\n\t=== [ERROR] Please Enter a Valid Menu: A Digit Number (0-2) ===\n";
					} // End of if (customer_choice.length() == 1 && isdigit(customer_choice.at(0)))
				} while (!flag_for_customer);
				break;
			default:
				cout << "\n\n\t=== [ERROR] Please Enter a Number (0-2) ===\n";
			} // End of switch(stoi(op_choice))
		}
		else {
			cout << "\n\n\t=== [ERROR] Please Enter a Valid Menu: A Digit Number (0-2) ===\n";
		} // End of if (op_choice.length() ... )

	} while (!flag_for_op);

	return 0;
}