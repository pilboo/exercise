#include "services.h"

services::services() {
	s_service_id = "";
	s_type_id = "";
	s_time_id = "";
	s_service_name = "";
	s_service_type = "";
	s_service_time = 0;
	s_service_price = 0.00;
	s_service_limit = 0;

	service_cnt = 0;
	all_services = nullptr;

	init_service_arr(SERVICE_FILE);
}

services::services(services & obj) {
	obj.s_service_id = "";
	obj.s_type_id = "";
	obj.s_time_id = "";
	obj.s_service_name = "";
	obj.s_service_type = "";
	obj.s_service_time = 0;
	obj.s_service_price = 0.00;
	obj.s_service_limit = 0;

	obj.service_cnt = 0;
	obj.all_services = nullptr;

	obj.init_service_arr(obj.SERVICE_FILE);
}

void services::init_service_arr(const string &txt) {
	ifstream service_file;
	string temp_line;

	service_file.open(txt);

	if (!service_file) {
		cout << "\n\t=========================================="
			<< "\n\t|| Error: Cannot Open the Service File. ||"
			<< "\n\t==========================================" << endl;
		exit(0);
	}
	else {
		while (!service_file.eof()) {
			getline(service_file, temp_line);
			service_cnt++;
		}

		service_file.clear();
		service_file.seekg(0, ios::beg);

		all_services = new service_info[service_cnt];
		
		for (int i = 0; i < service_cnt; i++) {
			int pos1 = 0, pos2 = 0;
			string temp_arr[8];

			getline(service_file, temp_line);

			for (int j = 0; j < 8; j++) {
				pos2 = temp_line.find(",", pos1);
				temp_arr[j] = temp_line.substr(pos1, pos2 - pos1);
				pos1 = pos2 + 1;
			}

			all_services[i].service_id = temp_arr[0];
			all_services[i].type_id = temp_arr[1];
			all_services[i].time_id = temp_arr[2];
			all_services[i].service_name = temp_arr[3];
			all_services[i].service_type = temp_arr[4];
			all_services[i].service_time = stoi(temp_arr[5]);
			all_services[i].service_price = stod(temp_arr[6]);
			all_services[i].service_limit = stoi(temp_arr[7]);
		}
	}
	service_file.close();

	return;
}

void services::select_service(string &service_input) {
	bool isok = false;
	string type_input, time_input;

	service_info *selected_service;
	selected_service = nullptr;

	int selected_cnt = 0;

	do {
		cout << "\n\n\t=================================";
		cout << "\n\tSelect the Service.";
		cout << "\n\t=================================\n";

		print_services();
		
		cout << "\n\tPlease Enter the Service ID (3-digit number, D for Display Option): ";
		cin >> service_input;

		if (service_input == "D" || service_input == "d") {
			cout << "\n\t=== Returning to Display Option ===\n";
			break;
		} else if (chk_serviceid(service_input)) {
			selected_cnt = set_temp_count(service_input);
			selected_service = new service_info[selected_cnt];

			set_temp_service(service_input, selected_service);

			cout << "\n\t=================================";
			cout << "\n\tSelect the Type of Service.";
			cout << "\n\t=================================";

			print_types(service_input, selected_service, selected_cnt);

			cout << "\n\n\tPlease Enter the Type ID: ";
			cin >> type_input;

			if (chk_typeid(service_input, type_input)) {
				cout << "\n\t=================================";
				cout << "\n\tSelect the Time of Service.";
				cout << "\n\t=================================";

				print_times(type_input, selected_service, selected_cnt);
				
				cout << "\n\n\tPlease Enter the Time ID: ";
				cin >> time_input;

				if (chk_timeid(service_input, type_input, time_input)) {
					set_selected_service(type_input, time_input, selected_service, selected_cnt);
					isok = true;
				}
			}
		}
		
	} while (!isok);

	delete[] selected_service;
	selected_service = nullptr;

	return;
}

void services::print_services() {
	string s_id, s_name;

	for (int i = 0; i < service_cnt; i++) {
		if (all_services[i].service_id != s_id) {
			s_id = all_services[i].service_id;
			s_name = all_services[i].service_name;

			cout << "\t[" << s_id << "] " << s_name << "\n";
		}
	}
	return;
}

bool services::chk_serviceid(const string &inputid) {
	for (int i = 0; i < service_cnt; i++) {
		if (inputid == all_services[i].service_id)
			return true;
	}

	cout << "\n\t=== [ERROR] Please Enter Valid Service ID ===";
	return false;
}

int services::set_temp_count(const string &serviceid) {
	int cnt = 0;
	
	for (int i = 0; i < service_cnt; i++) {
		if (serviceid == all_services[i].service_id)
			cnt++;
	}

	return cnt;
}

void services::set_temp_service(const string &serviceid, service_info *temp_service) {
	for (int i = 0; i < service_cnt; i++) {
		if (serviceid == all_services[i].service_id) {
			temp_service->service_id = all_services[i].service_id;
			temp_service->type_id = all_services[i].type_id;
			temp_service->time_id = all_services[i].time_id;
			temp_service->service_name = all_services[i].service_name;
			temp_service->service_type = all_services[i].service_type;
			temp_service->service_time = all_services[i].service_time;
			temp_service->service_price = all_services[i].service_price;
			temp_service->service_limit = all_services[i].service_limit;
					
			temp_service++;
		}
	}

	return;
}

void services::print_types(const string &serviceid) const {
	string t_id, t_name;

	for (int i = 0; i < service_cnt; i++) {
		if ((serviceid == all_services[i].service_id) && (t_id != all_services[i].type_id)) {
			t_id = all_services[i].type_id;
			t_name = all_services[i].service_type;

			cout << "\n\t[" << t_id << "]";
			cout << "\t" << ((t_name.length() > 0) ? (t_name) : ("No Type"));
		}
	}
	return;
}

void services::print_types(const string &serviceid, const service_info *temp_service, const int &s_cnt) const {
	string t_id;

	for (int i = 0; i < s_cnt; i++) {
		if (t_id != temp_service[i].type_id) {
			t_id = temp_service[i].type_id;

			cout << "\n\t[" << temp_service[i].type_id << "]";
			cout << "\t" << ((temp_service[i].service_type.length() > 0) ? (temp_service[i].service_type) : "No Type");
		}
	}
	return;
}

bool services::chk_typeid(const string &sid, const string &type_id) const{
	if (type_id.length() == 2 && isdigit(type_id.at(0)) && isdigit(type_id.at(1))) {
		for (int i = 0; i < service_cnt; i++) {
			if (all_services[i].service_id == sid && all_services[i].type_id == type_id)
				return true;
		}
	}

	cout << "\n\t=== [ERROR] Please Enter Valid Type ID ===";
	return false;
}


int services::print_times(string &sid, string &tyid) {
	string tm_id; 
	int tm = 0;

	for (int i = 0; i < service_cnt; i++) {
		if ((sid == all_services[i].service_id) && (tyid == all_services[i].type_id)) {
			tm_id = all_services[i].time_id;
			tm = all_services[i].service_time;

			cout << "\n\t[" << tm_id << "]";
			cout << "\t" << tm << " minutes";
		}
	}
	
	return tm;
}

void services::print_times(const string &type_id, const service_info *temp_service, const int &s_cnt) const {
	for (int i = 0; i < s_cnt; i++) {
		if (type_id == temp_service[i].type_id) {
			cout << "\n\t[" << temp_service[i].time_id << "]";
			cout << "\t" << temp_service[i].service_time << " minutes";
		}
	}

	return;
}

bool services::chk_timeid(const string &sid, const string &type_id, const string &time_id) const {
	if (time_id.length() == 2 && isdigit(time_id.at(0)) && isdigit(time_id.at(1))) {
		for (int i = 0; i < service_cnt; i++) {
			if (all_services[i].service_id == sid && all_services[i].type_id == type_id && all_services[i].time_id == time_id)
				return true;
		}
	}

	cout << "\n\t=== [ERROR] Please Enter Valid Time ID ===";
	return false;
}

void services::set_selected_service(const string &type_id, const string &time_id, const service_info *temp_service, const int &s_cnt) {
	for (int i = 0; i < s_cnt; i++) {
		if (type_id == temp_service[i].type_id && time_id == temp_service[i].time_id) {
			s_service_id = temp_service[i].service_id;
			s_type_id = temp_service[i].type_id;
			s_time_id = temp_service[i].time_id;
			s_service_name = temp_service[i].service_name;
			s_service_type = temp_service[i].service_type;
			s_service_time = temp_service[i].service_time;
			s_service_price = temp_service[i].service_price;
			s_service_limit = temp_service[i].service_limit;

			break;
		}
	}

	return;
}

int services::get_service_cnt() const {
	return service_cnt;
}

string services::get_service_id() const {
	return s_service_id;
}

string services::get_type_id() const {
	return s_type_id;
}

string services::get_time_id() const {
	return s_time_id;
}

string services::get_service_name() const {
	return s_service_name;
}

string services::get_service_name(const string &sid) {
	string s_name;
	
	for (int i = 0; i < service_cnt; i++) {
		if (all_services[i].service_id == sid) {
			s_name = all_services[i].service_name;
			break;
		}
	}

	return s_name;
}

string services::get_service_type() const {
	return s_service_type;
}

string services::get_service_type(const string &sid, const string &type_id) {
	string s_type;
	
	for (int i = 0; i < service_cnt; i++) {
		if (all_services[i].service_id == sid && all_services[i].type_id == type_id) {
			s_type = all_services[i].service_type;
			break;
		}
	}

	return s_type;
}

int services::get_service_time() const {
	return s_service_time;
}

int services::get_service_time(const string &sid, const string &type_id, const string &time_id) {
	int s_time;
	
	for (int i = 0; i < service_cnt; i++) {
		if (all_services[i].service_id == sid && all_services[i].type_id == type_id &&
			all_services[i].time_id == time_id) {
			s_time = all_services[i].service_time;
			break;
		}
	}

	return s_time;
}

double services::get_service_price() const {
	return s_service_price;
}

double services::get_service_price(const string &sid, const string &type_id, const string &time_id) {
	double s_price;

	for (int i = 0; i < service_cnt; i++) {
		if (all_services[i].service_id == sid && all_services[i].type_id == type_id &&
			all_services[i].time_id == time_id) {
			s_price = all_services[i].service_price;
			break;
		}
	}

	return s_price;
}
int services::get_service_limit() const {
	return s_service_limit;
}

int services::get_service_limit(const string &sid, const string &type_id, const string &time_id) {
	int slimit;
	
	for (int i = 0; i < service_cnt; i++) {
		if (all_services[i].service_id == sid && all_services[i].type_id == type_id &&
			all_services[i].time_id == time_id) {
			slimit = all_services[i].service_limit;
			break;
		}
	}
	return slimit;
}

void services::print_all_serivces() {
	string sid, type_id, time_id;

	for (int i = 0; i < service_cnt; i++) {
		if (all_services[i].service_id != sid) {
			sid = all_services[i].service_id;
			type_id = "", time_id = "";
			cout << "\n\t[" << sid << "] " << all_services[i].service_name << " Service";
		}

		if (all_services[i].type_id != type_id) {
			type_id = all_services[i].type_id;
			time_id = "";
			cout << "\n\t\t[" << type_id << "] "
				<< (all_services[i].service_type.length() > 0 ? all_services[i].service_type : "No Type");
		}
		
		if (all_services[i].time_id != time_id) {
			time_id = all_services[i].time_id;
			cout << "\n\t\t\t[" << time_id << "] " << all_services[i].service_time << " minutes";
		}
	}

	return;
}

services::service_info *services::get_all_services() {
	return all_services;
}



services::~services() {
	delete[] all_services;
	all_services = nullptr;
}
