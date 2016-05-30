// Implemecation file for the Customer class.
#include <iostream>
#include <fstream>
#include "customers.h"

customers::customers() {
	c_id = "";
	c_name = "";
	chin_date = "";
	chin_time = "";
	chout_date = "";
	chout_time = "";
	c_info = "";
	isfound = false;
}

void customers::get_c_info(string &c_input) {
	
	ifstream c_file;
	string temp, temp_id;
	int pos;
	c_file.open("customers.txt");

	if (!c_file) {
		cout << "\n==========================================="
			 << "\n|| Error: Cannot open the Customer file. ||"
			 << "\n===========================================" << endl;
	}
	else {
		while (!c_file.eof()) {
			//getline(c_file, temp, '\n');
			getline(c_file, temp);
			pos = temp.find(",");

			temp_id = temp.substr(0, pos);

			if (c_input == temp_id) {
				cout << "\n\t=== Customer #" << c_input << " Found ===\n";
				c_info = temp;
				isfound = true;
				break;
			}
		}

		if (!isfound) 
			cout << "\n\t=== No customer #" << c_input << " was found ===\n";

		c_file.close();
	}
	
	return;
}

void customers::set_customers() {
	
	if (c_info.length() <= 0 || !isfound) {
		cout << "\n\t=== [ERROR] Customer information needed ===\n";
	}
	else {
		int pos1, pos2;
		
		pos1 = 0;
		pos2 = c_info.find(",", pos1);
		c_id = c_info.substr(pos1, pos2 - pos1);

		pos1 = pos2 + 1;
		pos2 = c_info.find(",", pos1);
		c_name = c_info.substr(pos1, pos2 - pos1);

		pos1 = pos2 + 1;
		pos2 = c_info.find(",", pos1);
		chin_date = c_info.substr(pos1, pos2 - pos1);

		pos1 = pos2 + 1;
		pos2 = c_info.find(",", pos1);
		chin_time = c_info.substr(pos1, pos2 - pos1);

		pos1 = pos2 + 1;
		pos2 = c_info.find(",", pos1);
		chout_date = c_info.substr(pos1, pos2 - pos1);

		pos1 = pos2 + 1;
		pos2 = c_info.find(",", pos1);
		chout_time = c_info.substr(pos1, pos2 - pos1);
	}
	return;
}

string customers::get_c_id() {
	return c_id;
}

string customers::get_c_name() {
	return c_name;
}

string customers::get_chin_date() {
	return chin_date;
}

string customers::get_chin_time() {
	return chin_time;
}

string customers::get_chout_date() {
	return chout_date;
}

string customers::get_chout_time() {
	return chout_time;
}

bool customers::get_isfound() {
	return isfound;
}

void customers::print_c_info() const {
	
	cout << "\n\tCustomer ID#: " << c_id << endl;
	cout << "\tCustomer Name: " << c_name << endl;
	cout << "\tCheck-in: " << chin_date << " " << chin_time << endl;
	cout << "\tCheck-out: " << chout_date << " " << chout_time << endl;

	return;
}

customers::~customers() {
}
