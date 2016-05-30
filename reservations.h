#ifndef RESERVATIONS_H
#define RESERVATIONS_H

#include <iostream>
#include <fstream>
#include <string>
using namespace std;

class reservations {
private:
	struct s_reservation {
		string customer_id;
		string service_id;
		string type_id;
		string time_id;
		string app_date;
		string app_start_time;
		string app_end_time;
		string input_date;
		string input_time;
		string iscancle;
		string cancel_date;
		string cancel_time;
	};
	s_reservation *rsrv_arr;
	string s_name;
	string t_name;
	int rsrv_cnt;
	int srvc_tm;
	int c_srvc_tm;
	void init_rsrv_arr(string&);
public:
	reservations();
	reservations(string&);
	void print_service_cal(string&, string&, string&, string&);
	void print_customer_cal(string&, string&, string&);
	bool chk_apptime(string&, int, int, int, int, int);
	bool chk_service(string&, string&, string&, string&, string&, int);
	bool chk_customer(string&, string&, string&, string&);
	bool reserve(string&, string&, string&, string&, string&, string&, int, string&, string&);
	~reservations();
};

#endif
