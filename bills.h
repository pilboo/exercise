#ifndef BILLS_H
#define BILLS_H

#include <string>
#include <iomanip>
#include "customers.h"
#include "services.h"
#include "reservations.h"

using namespace std;

class bills {
private:
	string customer_id;
	int cr_cnt;

	customers customer;
	services service;
	reservations reservation;
	reservations::reservation_info *cr;

	struct bill_info {
		string s_name;
		string s_type;
		int s_time;
		string app_day;
		string app_time;
		string iscanceled;
		string input_day;
		string input_time;
		double s_price;
	};

public:
	
	bills();
	void set_customer_id(const string&);
	void print_bill();
	int set_cr_cnt(reservations::reservation_info*);
	void set_customer_reservation(reservations::reservation_info*, reservations::reservation_info*);
	void sort_customer_reservation(reservations::reservation_info&);
	
	void set_customer_bill_info(bill_info*, reservations::reservation_info*, services::service_info*);
	void print_customer_bill(bill_info*);
	~bills();
};

#endif