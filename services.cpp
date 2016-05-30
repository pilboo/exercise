#include "services.h"

services::services() {
	service_cnt = 0;
	service_arr = nullptr;
}

services::services(string &txt) {
	service_cnt = 0;
	init_service_arr(txt);
}

void services::init_service_arr(string &txt) {
	ifstream service_file;
	string temp_line;

	service_file.open(txt);

	if (!service_file) {
		cout << "\n\t=========================================="
			<< "\n\t|| Error: Cannot open the service file. ||"
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

		service_arr = new s_service[service_cnt];
		int arr_index = 0;
		int pos1, pos2;

		while (!service_file.eof()) {
			getline(service_file, temp_line);
			
			pos1 = 0;
			pos2 = temp_line.find(",", pos1);
			service_arr[arr_index].service_id = temp_line.substr(pos1, pos2 - pos1);

			pos1 = pos2 + 1;
			pos2 = temp_line.find(",", pos1);
			service_arr[arr_index].type_id = temp_line.substr(pos1, pos2 - pos1);

			pos1 = pos2 + 1;
			pos2 = temp_line.find(",", pos1);
			service_arr[arr_index].time_id = temp_line.substr(pos1, pos2 - pos1);
			
			pos1 = pos2 + 1;
			pos2 = temp_line.find(",", pos1);
			service_arr[arr_index].service_name = temp_line.substr(pos1, pos2 - pos1);

			pos1 = pos2 + 1;
			pos2 = temp_line.find(",", pos1);
			service_arr[arr_index].service_type = temp_line.substr(pos1, pos2 - pos1);
			
			pos1 = pos2 + 1;
			pos2 = temp_line.find(",", pos1);
			service_arr[arr_index].service_time = stoi(temp_line.substr(pos1, pos2 - pos1));

			pos1 = pos2 + 1;
			pos2 = temp_line.find(",", pos1);
			service_arr[arr_index].service_price = stod(temp_line.substr(pos1, pos2 - pos1));

			pos1 = pos2 + 1;
			pos2 = temp_line.find(",", pos1);
			service_arr[arr_index].service_limit = stoi(temp_line.substr(pos1, pos2 - pos1));

			arr_index++;
		}
	}
	service_file.close();

	return;
}

void services::print_services() {
	string s_id, s_name;

	for (int i = 0; i < service_cnt; i++) {

		if (s_id != service_arr[i].service_id) {
			s_id = service_arr[i].service_id;
			s_name = service_arr[i].service_name;

			cout << "\t[" << s_id << "] " << s_name << "\n";
		}
	}
	return;
}

bool services::chk_serviceid(string &inputid) {
	for (int i = 0; i < service_cnt; i++) {
		if (inputid == service_arr[i].service_id)
			return true;
	}
	return false;
}

void services::print_types(string &serviceid) {
	string t_id, t_name;

	for (int i = 0; i < service_cnt; i++) {
		if ((serviceid == service_arr[i].service_id) && (t_id != service_arr[i].type_id)) {
			t_id = service_arr[i].type_id;
			t_name = service_arr[i].service_type;

			cout << "\n\t[" << t_id << "]";
			cout << "\t" << ((t_name.length() > 0) ? (t_name) : ("No Type"));
		}
	}
	

	return;
}

int services::print_times(string &sid, string &tyid) {
	string tm_id; 
	int tm;

	for (int i = 0; i < service_cnt; i++) {
		if ((sid == service_arr[i].service_id) && (tyid == service_arr[i].type_id)) {
			tm_id = service_arr[i].time_id;
			tm = service_arr[i].service_time;

			cout << "\n\t[" << tm_id << "]";
			cout << "\t" << tm << " minutes";
		}
	}
	

	return tm;
}

string services::get_servicename(string &sid) {
	for (int i = 0; i < service_cnt; i++) {
		if (sid == service_arr[i].service_id) 
			return service_arr[i].service_name;
	}

	return "No Service";
}

string services::get_servicetype(string &s_id, string &ty_id) {
	string s_type = "";
	
	for (int i = 0; i < service_cnt; i++) {
		if (s_id == service_arr[i].service_id && ty_id == service_arr[i].type_id) {
			s_type = service_arr[i].service_type;
			break;
		}
	}
	
	return s_type;
}

int services::get_servicetime(string &s_id, string &ty_id, string &tm_id) {
	int s_time = 0;

	for (int i = 0; i < service_cnt; i++) {
		if (s_id == service_arr[i].service_id && ty_id == service_arr[i].type_id && tm_id == service_arr[i].time_id) {
			s_time = service_arr[i].service_time;
			break;
		}
	}
	
	return s_time;
}

double services::get_serviceprice(string &s_id, string &ty_id, string &tm_id) {
	double s_price = 0.0;

	for (int i = 0; i < service_cnt; i++) {
		if (s_id == service_arr[i].service_id && ty_id == service_arr[i].type_id && tm_id == service_arr[i].time_id) {
			s_price = service_arr[i].service_price;
			break;
		}
	}
	
	return s_price;
}


int services::get_servicelimit(string &s_id, string &ty_id, string &tm_id) {
	int s_limit = 0;

	for (int i = 0; i < service_cnt; i++) {
		if (s_id == service_arr[i].service_id && ty_id == service_arr[i].type_id && tm_id == service_arr[i].time_id) {
			s_limit = service_arr[i].service_limit;
			break;
		}
	}

	return s_limit;
}

services::~services() {
	delete[] service_arr;
	service_arr = nullptr;
}
