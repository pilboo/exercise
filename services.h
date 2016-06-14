#ifndef SERVICES_H
#define SERVICES_H

#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
using namespace std;

class services {
private:
	// Variables for the service informtion selected
	string s_service_id,
		s_type_id,
		s_time_id,
		s_service_name,
		s_service_type;
	int s_service_time;
	double s_service_price;
	int s_service_limit;
	
	int service_cnt;				// Holds the elements numbers of the service array
	
	string SERVICE_FILE = "services.txt";

	void init_service_arr(const string&);	// Initialize the service array once dynamically allocated.

	
public:
	struct service_info {
		string service_id;
		string type_id;
		string time_id;
		string service_name;
		string service_type;
		int service_time;
		double service_price;
		int service_limit;
	};
	service_info *all_services;

	services();
	bool check_service_choice(string&);
	void print_service_choice(string&);
	void print_services();
	void select_service(string&);
	bool chk_serviceid(const string&);
	int set_temp_count(const string&);
	void set_temp_service(const string&, service_info*);
	void print_types(const string&) const;
	void print_types(const string&, const service_info*, const int&) const;
	bool chk_typeid(const string&, const string&) const;
	int print_times(string&, string&);
	void print_times(const string&, const service_info*, const int&) const;
	bool chk_timeid(const string&, const string&, const string&) const;
	void set_selected_service(const string&, const string&, const service_info*, const int&);
	int get_service_cnt() const;
	string get_service_id() const;
	string get_type_id() const;
	string get_time_id() const;
	string get_service_name() const;
	string get_service_name(const string&);
	string get_service_type() const;
	string get_service_type(const string&, const string&);
	int get_service_time() const;
	int get_service_time(const string&, const string&, const string&);
	double get_service_price() const;
	double get_service_price(const string&, const string&, const string&);
	int get_service_limit() const;
	int get_service_limit(const string&, const string&, const string&);
	service_info *get_all_services();
	void print_all_serivces();
	// For maintenance
	void add_new_service();
	void add_new_types(const int&, string*);
	void add_new_times(const int&, int*, double*, int*);
	bool check_int(const string&);
	~services();
};

#endif
