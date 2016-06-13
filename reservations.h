#ifndef RESERVATIONS_H
#define RESERVATIONS_H

#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include "customers.h"
#include "services.h"
using namespace std;

class reservations {
private:
	int reservation_cnt;
	string reservation_day;
	string s_day;
	string e_day;
	string reservation_time;
	string begin_time;
	string end_time;
	int begin_hour;
	int begin_min;
	int end_hour;
	int end_min;
	void init_reservation_arr(const string&);
	
	const string RESERVATION_FILE = "reservations.txt";
	customers customer;
	services service;
	string customer_id;
public:
	struct reservation_info {
		string customer_id;
		string service_id;
		string type_id;
		string time_id;
		string app_date;
		string app_start_time;
		string app_end_time;
		string input_date;
		string input_time;
		string iscanceled;
		string cancel_date;
		string cancel_time;
	};
	reservation_info *all_reservation;
	reservations();
	int get_reservation_cnt() const;
	string get_reservation_day() const;
	void set_customer_id(const string&);
	void set_reservation_day(string&);
	bool check_date(const string&);
	bool check_range_of_date(const string&, const string&, const string&);
	void set_begin_end(const string&);
	void print_calendar_header(const string&, const string&, const string&);
	void print_calendar_body(const string&, const string&, const string&);
	void print_service_cal(const string&, const string&, const string&);
	int set_temp_cnt();
	int set_temp_cnt(const string&);
	int set_temp_cnt(const string&, const string&);
	void set_temp_reservation(reservation_info*);
	void set_temp_reservation(const string&, reservation_info*);
	void set_temp_reservation(const string&, const string&, reservation_info*);
	void print_customer_cal(const string& );
	void set_reservation_time();
	void set_reservation_time(const string&, const string&, const string&);
	bool check_time(string&);
	bool check_range_of_time(string&, int);
	void set_range_of_time(const string&, const int&);
	bool check_service(const string&, const string&, const string&);
	//bool check_customer();
	bool check_customer(const string&, const string&);
	bool check_c_reservation(const string&, const string&);
	bool check_c_reservation(const reservation_info*, const int&, const string&, const string&);
	bool check_s_reservation(const string&, const string&);
	void print_available_services();
	void input_services();
	void print_available_time();
	void print_available_time(const string&, const string&);
	void set_period(string &, string&, string&, string&);
	string get_nextday(const string&, const int&);
	void reserve(string&, string&, string&);
	reservation_info *get_all_reservation();
	~reservations();
};

#endif
