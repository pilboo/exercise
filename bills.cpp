#include "bills.h"



bills::bills() {
	customer_id = "";
	cr_cnt = 0;

	cr = nullptr;
}

void bills::set_customer_id(const string &cid) {
	customer_id = cid;
}
void bills::print_bill() {
	services::service_info *s = nullptr;
	s = service.get_all_services();
	
	reservations::reservation_info *r = nullptr;
	r = reservation.get_all_reservation();

	cr_cnt = set_cr_cnt(r);
	cr = new reservations::reservation_info[cr_cnt];
	
	set_customer_reservation(r, cr);

	bill_info *cb = nullptr;
	cb = new bill_info[cr_cnt];

	set_customer_bill_info(cb, cr, s);
	
	print_customer_bill(cb);

	delete[] cb;
	cb = nullptr;
	
	return;
}

int bills::set_cr_cnt(reservations::reservation_info *r) {
	int reservation_cnt = reservation.get_reservation_cnt();
	int count = 0;

	for (int i = 0; i < reservation_cnt; i++) {
		if (r[i].customer_id == customer_id && r[i].iscanceled != "Y" &&
			r[i].app_date >= customer.get_chin_date(customer_id) &&
			r[i].app_date <= customer.get_chout_date(customer_id))
			count++;
	}
	
	return count;
}


void bills::set_customer_reservation(reservations::reservation_info *r, reservations::reservation_info *cr) {
	int reservation_cnt = reservation.get_reservation_cnt();
	
	for (int i = 0; i < reservation_cnt; i++) {
		
		if (r[i].customer_id == customer_id && r[i].iscanceled != "Y" && 
			r[i].app_date >= customer.get_chin_date(customer_id) && 
			r[i].app_date <= customer.get_chout_date(customer_id)) {

			cr->customer_id = (r + i)->customer_id;
			cr->service_id = (r + i)->service_id;
			cr->type_id = (r + i)->type_id;
			cr->time_id = (r + i)->time_id;
			cr->app_date = (r + i)->app_date;
			cr->app_start_time = (r + i)->app_start_time;
			cr->app_end_time = (r + i)->app_end_time;
			cr->input_date = (r + i)->input_date;
			cr->input_time = (r + i)->input_time;
			cr->iscanceled = (r + i)->iscanceled;
			cr->cancel_date = (r + i)->cancel_date;
			cr->cancel_time = (r + i)->cancel_time;

			cr++;
		}
	}
	return;
}

void bills::set_customer_bill_info(bill_info *cb, reservations::reservation_info *cr, services::service_info *s) {
	int s_cnt = service.get_service_cnt();

	for (int i = 0; i < cr_cnt; i++) {
		for (int j = 0; j < s_cnt; j++) {
			if (cr[i].service_id == s[j].service_id && cr[i].type_id == s[j].type_id && cr[i].time_id == s[j].time_id) {
				cb[i].s_name = s[j].service_name;
				cb[i].s_type = s[j].service_type;
				cb[i].s_time = s[j].service_time;
				cb[i].app_day = cr[i].app_date;
				cb[i].app_time = cr[i].app_start_time;
				cb[i].iscanceled = cr[i].iscanceled;
				cb[i].input_day = cr[i].input_date;
				cb[i].input_time = cr[i].input_time;
				cb[i].s_price = s[j].service_price;

				break;
			}
		}
	}

	return;
}

void bills::print_customer_bill(bill_info *cb) {
	cout << "\n\n\t=======================================================================================";
	cout << "\n\tBILL of the SERVICES";
	cout << "\n\t----------------------------------------------------------------------------------------";
	cout << "\n\tCUSTOMER: " << customer.get_c_name(customer_id);
	cout << "\n\t----------------------------------------------------------------------------------------";
	
	int index = 1;
	
	double price = 0.0, total = 0.0;

	for (int i = 0; i < cr_cnt; i++) {
		price = cb[i].s_price * (double)cb[i].s_time;
		total += price;
		cout << "\n\t[" << index++ << "]";
		cout << "\n\tSERVICE: " << cb[i].s_name << " - " << cb[i].s_type << " (" << cb[i].s_time << " minutes)";
		cout << "\n\tRESERVATION DAY: " << cb[i].app_day << "\tTIME: " << cb[i].app_time;
		cout << "\n\tPRICE: $" << setprecision(2) << fixed << showpoint << price;
		cout << "\n\t----------------------------------------------------------------------------------------";
	}

	cout << "\n\tTotal Price: $" << setprecision(2) << fixed << showpoint << total;
	cout << "\n\n\t=======================================================================================";
	
	return;
}

bills::~bills() {
	delete[] cr;
	cr = nullptr;
}
