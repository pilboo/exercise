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

bool services::check_service_choice(string &s_input) {
	if (s_input == "0")
		return true;
	else
		return false;
}

void services::print_service_choice(string &s_input) {
	do {
		cout << "\n\n\t=================================";
		cout << "\n\tSelect the Service.";
		cout << "\n\t=================================";
		print_services();
		cout << "\n\tPlease Enter the Service ID (3-digit number or 0 for Reservation Option): ";
		getline(cin, s_input);

		if (check_service_choice(s_input))
			break;
	} while (!chk_serviceid(s_input));
	return;
}

void services::select_service(string &service_input) {
	bool isok = false;
	string type_input, time_input;

	service_info *selected_service;
	selected_service = nullptr;

	int selected_cnt = 0;

	do {
		//if (chk_serviceid(service_input)) {
			selected_cnt = set_temp_count(service_input);
			selected_service = new service_info[selected_cnt];

			set_temp_service(service_input, selected_service);
			do {
				cout << "\n\t=================================";
				cout << "\n\tSelect the Type of Service.";
				cout << "\n\t=================================";

				print_types(service_input, selected_service, selected_cnt);

				cout << "\n\n\tPlease Enter the Type ID: ";
				getline(cin, type_input);

			} while (!chk_typeid(service_input, type_input));
			
			do {
				cout << "\n\t=================================";
				cout << "\n\tSelect the Time of Service.";
				cout << "\n\t=================================";

				print_times(type_input, selected_service, selected_cnt);

				cout << "\n\n\tPlease Enter the Time ID: ";
				getline(cin, time_input);

			} while (!chk_timeid(service_input, type_input, time_input)); 

			set_selected_service(type_input, time_input, selected_service, selected_cnt);
			isok = true;
		//}
		
	} while (!isok);

	delete[] selected_service;
	selected_service = nullptr;

	return;
}

void services::print_services() {
	string s_id;
	for (int i = 0; i < service_cnt; i++) {
		if (all_services[i].service_id != s_id) {
			s_id = all_services[i].service_id;
			cout << "\n\t[" << s_id << "] " << all_services[i].service_name;
		}
	}
	return;
}

bool services::chk_serviceid(const string &inputid) {
	if (inputid.length() == 3 && isdigit(inputid.at(0)) && isdigit(inputid.at(1)) && isdigit(inputid.at(2))) {
		for (int i = 0; i < service_cnt; i++) {
			if (inputid == all_services[i].service_id)
				return true;
		}
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
	int s_time = 0;
	
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
	double s_price = 0.0;

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
	int slimit = 0;
	
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

/////////////////////////////////////////////////////////////////////////////////
//
// Member functions for maintenance
//
/////////////////////////////////////////////////////////////////////////////////
// Add a new service

void services::add_new_service() {

	string new_sname;
	int max_id = 0;
	string new_sid;

	bool isok = false;
	do {
		cout << "\n\t================================================================";
		cout << "\n\tADDING NEW SERVICE";
		cout << "\n\t----------------------------------------------------------------";
		cout << "\n\tPlease Enter new Service Name: ";
		getline(cin, new_sname);

		if (new_sname.length() <= 0) {
			cout << "\n\t=== [ERROR] Nothing Entered ===";
			isok = false;
		}
		else
			isok = true;
	} while (!isok);

	for (int i = 0; i < service_cnt; i++) {
		if (stoi(all_services[i].service_id) > max_id)
			max_id = stoi(all_services[i].service_id);
	}

	max_id++;
	new_sid = (max_id < 10 ? "00" : (max_id < 100 ? "0" : "")) + to_string(max_id);

	cout << "\n\t-----------------------------------------------------------";
	cout << "\n\tNew Service ID#: " << new_sid;
	cout << "\n\tNew Service Name: " << new_sname;
	cout << "\n\t-----------------------------------------------------------";

	int type_cnt;
	int time_cnt;
	string input;
	isok = false;
	do {
		cout << "\n\tHow Many Types Are You Going to Add? ";
		getline(cin, input);

		if (!check_int(input)) {
			cout << "\n\t=== [ERROR] Please Enter a Valid Number ===";
			isok = false;
		}
		else {
			type_cnt = stoi(input);

			if (type_cnt < 0) {
				cout << "\n\t===[ERROR] Times Must Be More Than or Equal to 0 ===";
				isok = false;
			}
			else if (type_cnt == 0) {
				type_cnt = 1;
				isok = true;
			}
			else
				isok = true;
		}
	} while (!isok);
	
	string *new_stype = nullptr;
	new_stype = new string[type_cnt];

	add_new_types(type_cnt, new_stype);

	cout << "\n\t-----------------------------------------------------------";
	for (int i = 0; i < type_cnt; i++) {
		cout << "\n\t New Type #" << i + 1 << ". " << new_stype[i];
		if (type_cnt == 1 && new_stype[i].length() == 0)
			cout << "No Type";
	}
	cout << "\n\t-----------------------------------------------------------";

	isok = false;
	do {
		cout << "\n\tHow Many Times Are You Going to Add? ";
		getline(cin, input);

		if (!check_int(input)) {
			cout << "\n\t=== [ERROR] Please Enter a Valid Number ===";
			isok = false;
		}
		else {
			time_cnt = stoi(input);

			if (time_cnt <= 0) {
				cout << "\n\t===[ERROR] Times Must Be More Than 0 ===";
				isok = false;
			}
			else
				isok = true;
		}
	} while (!isok);
	 
	int *new_stime;
	new_stime = new int[time_cnt];

	double *new_sprice;
	new_sprice = new double[time_cnt];

	int *new_slimit;
	new_slimit = new int[time_cnt];

	add_new_times(time_cnt, new_stime, new_sprice, new_slimit);
	
	cout << "\n\t-----------------------------------------------------------";
	for (int i = 0; i < time_cnt; i++) {
		cout << "\n\t New Time #" << i + 1 << ". " << new_stime[i];
		cout << "\n\t Price $: " << setprecision(2) << fixed << showpoint << new_sprice[i];
		cout << "\n\t Service Limit: " << new_slimit[i];
		cout << "\n\t";
	}
	cout << "\n\t-----------------------------------------------------------";
	
	service_info *temp = nullptr;
	temp = new service_info[type_cnt * time_cnt];

	int type_index = 0;
	int time_index = 0;

	ofstream out_sfile;
	out_sfile.open(SERVICE_FILE, ios::app);

	for (int i = 0; i < type_cnt * time_cnt; i++) {
		temp[i].service_id = new_sid;
		temp[i].service_name = new_sname;
		if (i > 0 && i % time_cnt == 0)
			type_index++;
		temp[i].type_id = (type_index < 10 ? "0": "") + to_string(type_index + 1);
		temp[i].service_type = new_stype[type_index];
		temp[i].time_id = (time_index < 10 ? "0" : "") + to_string(time_index + 1);
		temp[i].service_time = new_stime[time_index];
		temp[i].service_price = new_sprice[time_index];
		temp[i].service_limit = new_slimit[time_index];
		time_index++;
		if (time_index == time_cnt)
			time_index = 0;
		out_sfile << ((service_cnt == 0) ? "" : "\n") << temp[i].service_id << ","
			<< temp[i].type_id << ","
			<< temp[i].time_id << ","
			<< temp[i].service_name << ","
			<< temp[i].service_type << ","
			<< temp[i].service_time << ","
			<< temp[i].service_price << ","
			<< temp[i].service_limit;

		cout << "\n\n\t===[New Service Was Successfully Added]===";
	}
		
	out_sfile.close();
	init_service_arr(SERVICE_FILE);
	
	delete[] temp;
	temp = nullptr;

	delete[] new_stime, new_stype;
	new_stime = nullptr, new_stype = nullptr;

	return;
}


void services::add_new_types(const int &type_cnt, string *new_stype) {
	bool isok = false;
	string input;
	string answer;

	if (type_cnt == 1) {
		do {
			cout << "\n\tDo You Input Type Name or Keep \"No Type\"? (Y or N)";
			getline(cin, answer);

			if (answer == "Y" || answer == "y")
				break;
			else if (answer == "N" || answer == "n")
				isok = true;
			else {
				cout << "\n\t=== [ERROR] Please Input a Valid Answer (Y or N) ===";
				isok = false;
			}
		} while (!isok);
	}	// End of if (type_cnt == 1)

	if (type_cnt > 1 || isok == true) {
		for (int i = 0; i < type_cnt; i++) {
			isok = false;
			do {
				cout << "\n\tEnter the Type Name #" << (i + 1) << ": ";
				getline(cin, input);

				if (input.length() == 0) {
					cout << "\n\t=== [ERRPR] Please Input the Type Name ===";
					isok = false;
				} else {
					isok = true;
				}	// End of if (input.length() == 0)
			} while (!isok);

			new_stype[i] = input;
		}	// End of for (...)
	}	// End of if (type_cnt > 1 || isok == true)

	return;
}

void services::add_new_times(const int &time_cnt, int *new_stime, double *new_sprice, int *new_slimit) {
	bool isok = false;
	string input;

	for (int i = 0; i < time_cnt; i++) {
		do {
			// Input time
			cout << "\n\tEnter Minutes for the Service Time #" << (i + 1) << ": ";
			getline(cin, input);

			if (input.length() > 0) {
				char ch;
				for (int j = 0; j < (int)input.length(); j++) {
					ch = input.at(j);
					if (isdigit(ch))
						isok = true;
					else {
						isok = false;
						break;
					}
				}
			}
			else
				isok = false;

			if (!isok)
				cout << "\n\t===[ERROR] Please Enter Integer Numbers ===";
		} while (!isok);
		new_stime[i] = stoi(input);

		// Input Price
		isok = false;
		do {
			cout << "\n\tEnter the Service Price for the Service Time " << new_stime[i] << " Minutes: ";
			getline(cin, input);

			if (input.length() > 0) {
				char ch;
				for (int j = 0; j < (int)input.length(); j++) {
					ch = input.at(j);
					if (isdigit(ch))
						isok = true;
					else if (ch = '.') 
						isok = true;
					else {
						isok = false;
						break;
					}
				}
			}
			else
				isok = false;

			if (!isok)
				cout << "\n\t===[ERROR] Please Enter Valid Double Numbers ===";
		} while (!isok);
		new_sprice[i] = stod(input);

		// Input Service Limit
		isok = false;
		do {
			cout << "\n\tEnter the Service Limit for the Service Time " << new_stime[i] << " Minutes (0 for No Limit): ";
			getline(cin, input);

			if (input.length() > 0) {
				char ch;
				for (int j = 0; j < (int)input.length(); j++) {
					ch = input.at(j);
					if (isdigit(ch))
						isok = true;
					else {
						isok = false;
						break;
					}
				}
			}
			else
				isok = false;

			if (!isok)
				cout << "\n\t===[ERROR] Please Enter Valid Integer Numbers ===";
		} while (!isok);
		new_slimit[i] = stoi(input);
	}	// End of for(...)

	return;
}

bool services::check_int(const string &input) {
	if (input.length() > 0) {
		char ch;
		for (int i = 0; i < (int)input.length(); i++) {
			ch = input.at(i);
			if (!isdigit(ch))
				return false;
		}
	}
	return true;
}

services::~services() {
	delete[] all_services;
	all_services = nullptr;
}
