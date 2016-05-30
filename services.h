#ifndef SERVICES_H
#define SERVICES_H

#include <iostream>
#include <fstream>
#include <string>
using namespace std;

class services {
private:
	struct s_service {
		string service_id;
		string type_id;
		string time_id;
		string service_name;
		string service_type;
		int service_time;
		double service_price;
		int service_limit;
	};
	s_service *service_arr;			// Pointer to be stored into the service from the services.txt
	int service_cnt;				// Holds the elements numbers of the service array
	void init_service_arr(string&);	// Initialize the service array once dynamically allocated.
	
public:
	services();
	services(string&);
	void print_services();
	bool chk_serviceid(string&);
	void print_types(string&);
	string get_servicename(string&);
	int print_times(string&, string&);
	string get_servicetype(string&, string&);
	int get_servicetime(string&, string&, string&);
	double get_serviceprice(string&, string&, string&);
	int get_servicelimit(string&, string&, string&);
	~services();
};

#endif
