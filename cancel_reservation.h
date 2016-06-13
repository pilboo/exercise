// Specification file for the misc (miscellanea) class.
#ifndef CANCEL_RESERVATION_H
#define CANCEL_RESERVATION_H

#include <iostream>
#include <string>
#include "customers.h"
#include "services.h"
#include "reservations.h"
using namespace std;

class cancel_reservation {
private:
	int reservation_cnt;
	string cancel_day;
	string cancel_time;
	const string RESERVATION_FILE = "reservations.txt";
	customers customer;
	services service;
	reservations reservation;
	string customer_id;

public:
	cancel_reservation();
	void set_customer_id(const string&);
	void set_cancel_date_time(string&);
	bool check_cancel_date(const string&);
	bool check_cancel_date(const string&, const string&);
	bool check_cancel_avail(const string&, const string&, const string&, const string&);
	int get_times_left(const string&, const string&, const string&, const string&);
	void cancel();
	~cancel_reservation();
};

#endif
