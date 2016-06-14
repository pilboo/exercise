// Implemecation file for the Customer class.
#include <iostream>
#include <fstream>
#include <ctime>
#include "customers.h"

customers::customers() {
	c_id = "";
	c_name = "";
	chin_date = "";
	chin_time = "";
	chout_date = "";
	chout_time = "";
		
	cur_day = "";
	cur_hour = "";
	cur_min = "";

	all_customer = nullptr;
	customer_cnt = 0;

	init_customer_arr(CUSTOMER_FILE);
}

customers::customers(customers &obj) {
	obj.c_id = "";
	obj.c_name = "";
	obj.chin_date = "";
	obj.chin_time = "";
	obj.chout_date = "";
	obj.chout_time = "";

	obj.cur_day = "";
	obj.cur_hour = "";
	obj.cur_min = "";

	obj.all_customer = nullptr;
	obj.customer_cnt = 0;

	obj.init_customer_arr(obj.CUSTOMER_FILE);
}
void customers::init_customer_arr(const string &txt_file) {
	ifstream customer_file;
	string customer_record;

	customer_file.open(txt_file);

	if (!customer_file) {
		cout << "\n==========================================="
			 << "\n|| Error: Cannot Open the Customer File. ||"
			 << "\n===========================================" << endl;
		exit(0);
	}
	else {
		while (!customer_file.eof()) {	// Calculates the number of customer's information in the customer.txt
			getline(customer_file, customer_record);
			customer_cnt++;
		}	// End of while

		customer_file.clear();
		customer_file.seekg(0, ios::beg);	// Goes back to the beginning of the file

		all_customer = new customer_info[customer_cnt];
		
		for (int i = 0; i < customer_cnt; i++) {
			int pos1 = 0, pos2 = 0;
			string temp_arr[6];

			getline(customer_file, customer_record);

			for (int j = 0; j < 6; j++) {
				pos2 = customer_record.find(",", pos1);
				temp_arr[j] = customer_record.substr(pos1, pos2 - pos1);
				pos1 = pos2 + 1;
			}	// End of for statement to store each customer information to the temp_arr.  
			
			all_customer[i].cid = temp_arr[0];
			all_customer[i].cname = temp_arr[1];
			all_customer[i].in_date = temp_arr[2];
			all_customer[i].in_time = temp_arr[3];
			all_customer[i].out_date = temp_arr[4];
			all_customer[i].out_time = temp_arr[5];
		}	// End of for statement to store each customer record into the all_customer structure array.
	}

	customer_file.close();

	return;
}
/*
The member function of set_customer accepts the customer ID#,
and stores the customer information into the member variables.
*/
bool customers::set_customer(string &input_id) {
	bool isok = false;
	customer_info *found_customer = nullptr;
	found_customer = new customer_info;

	if (id_check(input_id) && isfound(input_id, found_customer) && check_stay(found_customer)) {

		c_id = found_customer->cid;
		c_name = found_customer->cname;
		chin_date = found_customer->in_date;
		chin_time = found_customer->in_time;
		chout_date = found_customer->out_date;
		chout_time = found_customer->out_time;

		isok = true;
	}

	delete found_customer;
	found_customer = nullptr;

	return isok;
}

bool customers::id_check(const string &input_str) {
	bool isok = true;
	
	// Check the lenght of the input
	if (input_str.length() != 3) {
		isok = false;
	}
	else {
		for (int i = 0; i < 3; i++) {
			if (!isdigit(input_str.at(i)))
				isok = false;
		}
	}

	if (!isok)
		cout << "\n\t=== [ERROR] Please Enter a 3-Digit Number [000 - 999] ===" << endl;

	return isok;
}

bool customers::isfound(const string &input_str, customer_info *c_info) {
	
	for (int i = 0; i < customer_cnt; i++) {
		if (all_customer[i].cid == input_str) {

			c_info->cid = all_customer[i].cid;
			c_info->cname = all_customer[i].cname;
			c_info->in_date = all_customer[i].in_date;
			c_info->in_time = all_customer[i].in_time;
			c_info->out_date = all_customer[i].out_date;
			c_info->out_time = all_customer[i].out_time;

			cout << "\n\t=== Customer ID #" << input_str << " Is Found ===\n";
			print_c_info(c_info);

			return true;
		}
	}

	cout << "\n\t=== [No Customer ID # " << input_str << " Was Found] ===\n";
	return false;
}

bool customers::check_stay(const customer_info *c_info) {
	string chin = c_info->in_date, 
		chout = c_info->out_date;

	set_cur();
	
	if ((get_days_left(chin, cur_day) <= 0 && get_days_left(cur_day, chout) <= 0) && (chin != chout)) {
		cout << "\n\n\t=== [No Within Stay Customer] ===\n";
		return false;
	}
	else if (chin == chout) {	// If check-in data is same as check-out data (not overnight)
		cout << "\n\n\t=== [No Overnight Customer] ===\n";
		return false;
	}	

	// The customer is a within-stay and overnight customer
	cout << "\n\n\t=== Valid to Reserve the Spa Service(s) ===\n";
	return true;
}

void customers::set_cur() {
	time_t current_time;
	struct tm local_time;

	time(&current_time);
	localtime_s(&local_time, &current_time);

	int c_min = local_time.tm_min;				// Get the current minutes
	int c_hour = local_time.tm_hour;			// Get the current hours
	int c_day = local_time.tm_mday;			// Get the day of this month
	int c_month = local_time.tm_mon + 1;		// Get this month
	int c_year = local_time.tm_year + 1900;	// Get this year

	cur_day = ((c_month < 10) ? "0" : "") + to_string(c_month)	// If the value of month is smaller than 10, add "0" in front of the month (ex. 3 -> 03)
		+ "-" + ((c_day < 10) ? "0" : "") + to_string(c_day)	// If the value of day is smaller that 10, add "0" in front of the day
		+ "-" + to_string(c_year);

	cur_hour = ((c_hour < 10) ? "0" : "") + to_string(c_hour);
	cur_min = ((c_min < 10) ? "0" : "") + to_string(c_min);

	return;
}

void customers::print_c_info() const {
	cout << "\n\t=================================================";
	cout << "\n\tCustomer ID#: " << c_id;
	cout << "\n\tCustomer Name: " << c_name;
	cout << "\n\tCheck-in: " << chin_date << " " << chin_time;
	cout << "\n\tCheck-out: " << chout_date << " " << chout_time;
	cout << "\n\t=================================================";

	return;
}

void customers::print_c_info(const customer_info *c_info) const {

	cout << "\n\t=================================================";
	cout << "\n\tCustomer ID#: " << c_info->cid;
	cout << "\n\tCustomer Name: " << c_info->cname;
	cout << "\n\tCheck-in: " << c_info->in_date << " " << c_info->in_time;
	cout << "\n\tCheck-out: " << c_info->out_date << " " << c_info->out_time;
	cout << "\n\t=================================================";

	return;
}

int customers::get_days_left(const string &b_date, const string &e_date) {
	time_t bdate, edate;
	struct tm tm_bdate, tm_edate;
	
	time(&bdate);
	time(&edate);
	localtime_s(&tm_bdate, &bdate);
	localtime_s(&tm_edate, &edate);

	tm_bdate.tm_mon = stoi(b_date.substr(0, 2)) - 1;
	tm_bdate.tm_mday = stoi(b_date.substr(3, 2));
	tm_bdate.tm_year = stoi(b_date.substr(6, 4)) - 1900;
	
	tm_edate.tm_mon = stoi(e_date.substr(0, 2)) - 1;
	tm_edate.tm_mday = stoi(e_date.substr(3, 2));
	tm_edate.tm_year = stoi(e_date.substr(6, 4)) - 1900;
	

	bdate = mktime(&tm_bdate);
	edate = mktime(&tm_edate);

	int days_left = (int)difftime(edate, bdate) / (60*60*24);
	return days_left;
}

string customers::get_c_id() const {
	return c_id;
}

string customers::get_c_name() const {
	return c_name;
}

string customers::get_c_name(const string &cid) {
	string cname;
	for (int i = 0; i < customer_cnt; i++) {
		if (all_customer[i].cid == cid) {
			cname = all_customer[i].cname;
			break;
		}
	}
	return cname;
}

string customers::get_chin_date() const {
	return chin_date;
}

string customers::get_chin_date(const string &cid) const {
	string str;
	for (int i = 0; i < customer_cnt; i++) {
		if (all_customer[i].cid == cid)
			str = all_customer[i].in_date;
	}

	return str;
}

string customers::get_chin_time() const {
	return chin_time;
}

string customers::get_chin_time(const string &cid) const {
	string str;
	for (int i = 0; i < customer_cnt; i++) {
		if (all_customer[i].cid == cid)
			str = all_customer[i].in_time;
	}

	return str;
}
string customers::get_chout_date() const {
	return chout_date;
}

string customers::get_chout_date(const string &cid) {
	string str;
	for (int i = 0; i < customer_cnt; i++) {
		if (all_customer[i].cid == cid) 
			str = all_customer[i].out_date;
	}
	return str;
}

string customers::get_chout_time() const {
	return chout_time;
}

string customers::get_chout_time(const string &cid) {
	string str;
	for (int i = 0; i < customer_cnt; i++) {
		if (all_customer[i].cid == cid) 
			str = all_customer[i].out_time;
	}
	return str;
}

string customers::get_cur_day() const {
	return cur_day;
}

string customers::get_cur_hour() const {
	return cur_hour;
}

string customers::get_cur_min() const {
	return cur_min;
}

customers::~customers() {
	delete[] all_customer;
	all_customer = nullptr;
}