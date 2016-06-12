#include "reservations.h"

// Default constructor
reservations::reservations() {
	reservation_cnt = 0;
	reservation_day = "";
	reservation_time = "";
	begin_time = "";
	end_time = "";
	begin_hour = 8;
	begin_min = 0;
	end_hour = 20;
	end_min = 0;

	all_reservation = nullptr;

	init_reservation_arr(RESERVATION_FILE);
}

reservations::reservations(customers &c_obj, services &s_obj) {
	reservation_cnt = 0;
	reservation_day = "";
	reservation_time = "";
	begin_time = "";
	end_time = "";
	begin_hour = 8;
	begin_min = 0;
	end_hour = 20;
	end_min = 0;

	all_reservation = nullptr;

	init_reservation_arr(RESERVATION_FILE);

	customer = c_obj;
	service = s_obj;
}
// Accepts the name of the reservation text file, then sets the all_reservation array.
void reservations::init_reservation_arr(const string &txt) {
	ifstream reservation_file;
	string temp_line;

	reservation_file.open(txt);

	if (!reservation_file) {
		cout << "\n=============================================="
			<< "\n|| Error: Cannot open the reservation file. ||"
			<< "\n==============================================" << endl;
		exit(0);
	}
	else {
		while (!reservation_file.eof()) {
			getline(reservation_file, temp_line);
			reservation_cnt++;
		}

		reservation_file.clear();
		reservation_file.seekg(0, ios::beg);

		all_reservation = new reservation_info[reservation_cnt];
		
		for (int i = 0; i < reservation_cnt; i++) {
			int pos1 = 0, pos2 = 0;
			string temp_arr[12];

			getline(reservation_file, temp_line);

			for (int j = 0; j < 12; j++) {
				pos2 = temp_line.find(",", pos1);
				temp_arr[j] = temp_line.substr(pos1, pos2 - pos1);
				pos1 = pos2 + 1;
			}

			all_reservation[i].customer_id = temp_arr[0];
			all_reservation[i].service_id = temp_arr[1];
			all_reservation[i].type_id = temp_arr[2];
			all_reservation[i].time_id = temp_arr[3];
			all_reservation[i].app_date = temp_arr[4];
			all_reservation[i].app_start_time = temp_arr[5];
			all_reservation[i].app_end_time = temp_arr[6];
			all_reservation[i].input_date = temp_arr[7];
			all_reservation[i].input_time = temp_arr[8];
			all_reservation[i].iscancled = temp_arr[9];
			all_reservation[i].cancel_date = temp_arr[10];
			all_reservation[i].cancel_time = temp_arr[11];
		}
	}
	reservation_file.close();

	return;
}

void reservations::set_customer_id(const string &cid) {
	customer_id = cid;
}

// Check the day of reservation input, then store the day into the reservation_day
void reservations::set_reservation_day() {
	bool isok = false;
	string r_date;
	customer.set_cur();
	string today = customer.get_cur_day();
	string chout_day = customer.get_chout_date(customer_id);

	do {
		cout << "\n\n\t=================================";
		cout << "\n\tSelect the Day of Reservation";
		cout << "\n\t=================================";
		cout << "\n\tThe Reservation Can Be Made between " << today << " and " << chout_day;
		cout << "\n\tPlease Enter the Date of Reservation (mm-dd-yyyy): ";
		cin >> r_date;

		if (check_date(r_date) && check_range_of_date(r_date, today, chout_day)) {
			reservation_day = r_date;
			isok = true;
		}
	} while (!isok);

	return;
}

// Check whether the day input is valid format of date
bool reservations::check_date(const string &date_input) {
	if (date_input.length() == 10) {
		bool isok = false;
		char ch;

		for (int i = 0; i < 10; i++) {
			ch = date_input.at(i);

			if (i == 2 || i == 5) 
				isok = (ch == '-') ? true : false;
			else 
				isok = isdigit(ch) ? true : false;

			if (!isok)
				break;
		}
		
		if (isok) {
			int temp_month = stoi(date_input.substr(0, 2));
			int temp_day = stoi(date_input.substr(3, 2));
			int temp_year = stoi(date_input.substr(6, 4));
			
			int day_of_month[12] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

			if ((temp_year % 4 == 0) && (temp_year % 100 != 0) && (temp_year % 400 == 0))
				day_of_month[1] = 29;
			
			if ((temp_month >= 1 && temp_month <= 12) && (temp_day >= 1 && temp_day <= day_of_month[temp_month - 1])) {
				return true;
			}
		}
	}

	cout << "\n\t=== [ERROR] Please Enter Valid Date (mm-dd-yyyy) ===";
	return false;
	
}

bool reservations::check_range_of_date(const string &input_date, const string &begin_day, const string &end_day) {
	if ((input_date >= begin_day) && (input_date <= end_day)) 
		return true;

	cout << "\n\t=== [ERROR] Please Enter the Day between " << begin_day << " and " << end_day << " ===";
	return false;
}

// Set up the beginning and ending time of the calendar
void reservations::set_begin_end() {
	customer.set_cur();
	
	if (reservation_day == customer.get_cur_day()) {
		begin_hour = stoi(customer.get_cur_hour());
		begin_min = 0;

		(stoi(customer.get_cur_min()) > 30) ? begin_hour++ : begin_min = 30;
	}
	else if (reservation_day == customer.get_chout_date(customer_id)) {
		end_hour = stoi((customer.get_chout_time(customer_id)).substr(0, 2));
		end_min = stoi((customer.get_chout_time(customer_id)).substr(3, 2));
	}

	return;
}


void reservations::print_calendar_header(const string &sid, const string &type_id, const string &time_id) {
	init_reservation_arr(RESERVATION_FILE);
	string stype = service.get_service_type(sid, type_id);
	int stime = service.get_service_time(sid, type_id, time_id);
		
	cout << "\n\t===============================================================";
	cout << "\n\tSERVICE: " << service.get_service_name(sid) << ((stype.length() > 0) ? " - " : "") << stype << " (" << stime << " minutes)";
	cout << setprecision(2) << fixed << showpoint;
	cout << "\tPRICE: $" << (double)stime * service.get_service_price(sid, type_id, time_id);
	cout << "\n\tCUSTOMER: " << customer.get_c_name(customer_id);
	cout << "\n\tDATE of RESERVATION: " << reservation_day;
	cout << "\n\t---------------------------------------------------------------";
	cout << "\n\tTIME\tSERVICE AVALABLBE\tCUSTOMER AVAILABLE?";
	cout << "\n\t===============================================================";

	return;
}
void reservations::print_calendar_body(const string &sid, const string &type_id, const string &time_id) {
	for (int i = begin_hour; i <= end_hour; i++) {
		string cal_time;
		cal_time = ((i < 10) ? "0" : "") + to_string(i) + ":" + ((begin_min == 0) ? "0" : "") + to_string(begin_min);

		cout << "\n\t" << cal_time;
		print_service_cal(sid, type_id, cal_time);
		print_customer_cal(sid, type_id, time_id, cal_time);
		//cout << "\n\t----------------------------------------------------------";

		if (i != end_hour && begin_min == 0) {
			begin_min = 30;
			cal_time = ((i < 10) ? "0" : "") + to_string(i) + ":" + to_string(begin_min);

			cout << "\n\t" << cal_time;
			print_service_cal(sid, type_id, cal_time);
			print_customer_cal(sid, type_id, time_id, cal_time);
			//cout << "\n\t----------------------------------------------------------";
		}
		begin_min = 0;
	}
	cout << "\n\t===============================================================";

	return;
}

void reservations::print_service_cal(const string &sid, const string &tid, const string &rtime) {
	int one_day_cnt = set_temp_cnt(sid, tid);
	
	if (one_day_cnt > 0) {
		reservation_info *one_day_reservation = nullptr;
		one_day_reservation = new reservation_info[one_day_cnt];

		set_temp_reservation(sid, tid, one_day_reservation);

		for (int i = 0; i < one_day_cnt; i++) {
			if (one_day_reservation[i].app_date == reservation_day &&
				one_day_reservation[i].app_start_time <= rtime &&
				one_day_reservation[i].app_end_time >= rtime) {

				cout << "\tRESERVED";
				break;
			}
			else if (i == one_day_cnt - 1) 
				cout << "\t        ";
		}  
		delete[] one_day_reservation;
		one_day_reservation = nullptr;
	}
	else 
		cout << "\t        ";
	return;
}

// Count the customer's reservation on the reservation day
int reservations::set_temp_cnt() {
	int cnt = 0;

	for (int i = 0; i < reservation_cnt; i++) {
		if (all_reservation[i].customer_id == customer_id && all_reservation[i].app_date == reservation_day 
			&& all_reservation[i].iscancled != "Y")
			cnt++;
	}

	return cnt;
}

// Count the reservation on a specific day 
int reservations::set_temp_cnt(const string &r_day) {
	int cnt = 0;
	for (int i = 0; i < reservation_cnt; i++) {
		if (all_reservation[i].app_date == r_day && all_reservation[i].iscancled != "Y")
			cnt++;
	}
	
	return cnt;
}
// Count the reservation of a specific service and type on the reservation day
int reservations::set_temp_cnt(const string &sid, const string &tid) {
	int cnt = 0;

	for (int i = 0; i < reservation_cnt; i++) {

		if (all_reservation[i].service_id == sid && all_reservation[i].type_id == tid &&
			all_reservation[i].app_date == reservation_day && all_reservation[i].iscancled != "Y")
			cnt++;
	}

	return cnt;
}


//Keep the reservation information of a specific customer on the reservation day
void reservations::set_temp_reservation(reservation_info *temp_reservation) {
	for (int i = 0; i < reservation_cnt; i++) {
		if (all_reservation[i].customer_id == customer_id && all_reservation[i].app_date == reservation_day && 
			all_reservation[i].iscancled != "Y") {

			temp_reservation->customer_id = all_reservation[i].customer_id;
			temp_reservation->service_id = all_reservation[i].service_id;
			temp_reservation->type_id = all_reservation[i].type_id;
			temp_reservation->time_id = all_reservation[i].time_id;
			temp_reservation->app_date = all_reservation[i].app_date;
			temp_reservation->app_start_time = all_reservation[i].app_start_time;
			temp_reservation->app_end_time = all_reservation[i].app_end_time;
			temp_reservation->input_date = all_reservation[i].input_date;
			temp_reservation->input_time = all_reservation[i].input_time;
			temp_reservation->iscancled = all_reservation[i].iscancled;
			temp_reservation->cancel_date = all_reservation[i].cancel_date;
			temp_reservation->cancel_time = all_reservation[i].cancel_time;

			temp_reservation++;
		}
	}
	return;
}

// Keep the reservation information on the specific day
void reservations::set_temp_reservation(const string &r_day, reservation_info *temp_reservation) {
	for (int i = 0; i < reservation_cnt; i++) {
		if (all_reservation[i].app_date == r_day && all_reservation[i].iscancled != "Y") {
			temp_reservation->customer_id = all_reservation[i].customer_id;
			temp_reservation->service_id = all_reservation[i].service_id;
			temp_reservation->type_id = all_reservation[i].type_id;
			temp_reservation->time_id = all_reservation[i].time_id;
			temp_reservation->app_date = all_reservation[i].app_date;
			temp_reservation->app_start_time = all_reservation[i].app_start_time;
			temp_reservation->app_end_time = all_reservation[i].app_end_time;
			temp_reservation->input_date = all_reservation[i].input_date;
			temp_reservation->input_time = all_reservation[i].input_time;
			temp_reservation->iscancled = all_reservation[i].iscancled;
			temp_reservation->cancel_date = all_reservation[i].cancel_date;
			temp_reservation->cancel_time = all_reservation[i].cancel_time;
			
			temp_reservation++;
		}
	}
	return;
}

// Keep the reservation information of a specific service on the reservation day
void reservations::set_temp_reservation(const string &sid, const string &tid, reservation_info *temp_reservation) {
	for (int i = 0; i < reservation_cnt; i++) {
		if (all_reservation[i].service_id == sid && all_reservation[i].type_id == tid &&
			all_reservation[i].app_date == reservation_day && all_reservation[i].iscancled != "Y") {

			temp_reservation->customer_id = all_reservation[i].customer_id;
			temp_reservation->service_id = all_reservation[i].service_id;
			temp_reservation->type_id = all_reservation[i].type_id;
			temp_reservation->time_id = all_reservation[i].time_id;
			temp_reservation->app_date = all_reservation[i].app_date;
			temp_reservation->app_start_time = all_reservation[i].app_start_time;
			temp_reservation->app_end_time = all_reservation[i].app_end_time;
			temp_reservation->input_date = all_reservation[i].input_date;
			temp_reservation->input_time = all_reservation[i].input_time;
			temp_reservation->iscancled = all_reservation[i].iscancled;
			temp_reservation->cancel_date = all_reservation[i].cancel_date;
			temp_reservation->cancel_time = all_reservation[i].cancel_time;

			temp_reservation++;

		}
	}
	return;
}

void reservations::print_customer_cal(const string &sid, const string &type_id, const string &time_id, const string &rtime) {
	int cr_cnt = set_temp_cnt();

	if (cr_cnt > 0) {
		reservation_info *customer_reservation = nullptr;
		customer_reservation = new reservation_info[cr_cnt];

		set_temp_reservation(customer_reservation);

		for (int i = 0; i < cr_cnt; i++) {
			if (customer_reservation[i].app_start_time <= rtime && customer_reservation[i].app_end_time >= rtime) {
				string sname = service.get_service_name(customer_reservation[i].service_id);
				string stype = service.get_service_type(customer_reservation[i].service_id, customer_reservation[i].type_id);

				cout << "\t\t" << sname << ((stype.length() > 0) ? "-" : "") << stype;
				break;
			}
		}
		delete[] customer_reservation;
		customer_reservation = nullptr;
	}
	return;
}

void reservations::set_reservation_time() {
	set_begin_end();
	string app_time;
	bool isok = false;

	do {
		cout << "\n\n\t==========================================================";
		cout << "\n\tSelect the Time of Reservation on " << reservation_day;
		cout << "\n\t==========================================================";
		cout << "\n\tThe Reservation Can Be Made between " << ((begin_hour < 10) ? "0" : "") << begin_hour;
		cout << ":" << ((begin_min < 10) ? "0" : "") << begin_min << " and ";
		cout << ((end_hour < 10) ? "0" : "") << end_hour << ":" << ((end_min < 10) ? "0" : "") << end_min;
		cout << "\n\tPlease Enter the Reservation Time (hh:mm - e.g., 13:00): ";
		cin >> app_time;

		if (check_time(app_time) && check_range_of_time(app_time, 0)) {
			reservation_time = app_time;
			isok = true;
		}

	} while (!isok);

	return;
}

void reservations::set_reservation_time(const string &sid, const string &type_id, const string &time_id) {
	string app_time;
	int stime = service.get_service_time(sid, type_id, time_id);
	bool isok = false;

	do {
		cout << "\n\n\tPlease Enter the Reservation Time (hh:mm - e.g., 13:00): ";
		cin >> app_time;

		if (check_time(app_time) && check_range_of_time(app_time, stime) 
			&& check_service(sid, type_id, time_id) && check_customer(begin_time, end_time)) {
			reservation_time = app_time;
			isok = true;
		}
	} while (!isok);

	return;
}

bool reservations::check_time(string &input_time) {
	if (input_time.length() == 5) {
		if (isdigit(input_time.at(0)) && isdigit(input_time.at(1)) && input_time.at(2) == ':' &&
			isdigit(input_time.at(3)) && isdigit(input_time.at(4)))
			return true;
	}
	
	cout << "\n\t=== [ERROR] Please Enter Valid Time (hh:mm) ===";
	return false;
}

void reservations::set_range_of_time(const string &apptime, const int &stime) {
	begin_time = apptime;

	int temp_hour = stoi(begin_time.substr(0, 2));
	int temp_min = stoi(begin_time.substr(3, 2));

	temp_min += stime;
	temp_hour = temp_hour + temp_min / 60;
	temp_min = temp_min % 60;

	end_time = (temp_hour < 10 ? "0" : "") + to_string(temp_hour) + ":" + (temp_min < 10 ? "0" : "") + to_string(temp_min);

	return;
}

bool reservations::check_range_of_time(string &apptime, int stime) {
	set_range_of_time(apptime, stime);

	string btime = (begin_hour < 10 ? "0" : "") + to_string(begin_hour) + ":" + (begin_min < 10 ? "0" : "") + to_string(begin_min);
	string etime = (end_hour < 10 ? "0" : "") + to_string(end_hour) + ":" + (end_min < 10 ? "0" : "") + to_string(end_min);

	if (begin_time < btime) {
		cout << "\n\t=== [ERROR] It Was Earlier than the Beginning of the Availabe Times ===";
		return false;
	}
	else if (end_time > etime) {
		cout << "\n\t=== [ERROR] The Time Entered Was Later than the End of the Availabe Times ===";
		return false;
	}
	else {
		return true;
	}	
}

bool reservations::check_service(const string &sid, const string &tid, const string &time_id) {
	int daily_sr_cnt = set_temp_cnt(sid, tid);

	if (daily_sr_cnt > 0) {
		reservation_info *daily_sr = nullptr;
		daily_sr = new reservation_info[daily_sr_cnt];
		set_temp_reservation(sid, tid, daily_sr);
		
		int r_count = 0;

		for (int i = 0; i < daily_sr_cnt; i++) {
			if (daily_sr[i].service_id == sid && daily_sr[i].type_id == tid) {
				if ((daily_sr[i].app_start_time <= begin_time && daily_sr[i].app_end_time > begin_time) ||
					(daily_sr[i].app_start_time <= end_time && daily_sr[i].app_end_time > end_time)) {
					
					r_count++;

					if (service.get_service_limit(sid, tid, time_id) - r_count <= 0) {
						cout << "\n\n\t=== [No More Reservation Allowed] ===\n";
						return false;
					}
				}
			}
		}

		delete[] daily_sr;
		daily_sr = nullptr;
	}

	return true;
}

// Check if there is any reservation of the customer between the b_time and e_time
bool reservations::check_customer(const string &b_time, const string &e_time) {
	bool isok = true;
	int daily_cr_cnt = set_temp_cnt();

	if (daily_cr_cnt > 0) {
		reservation_info *daily_cr = nullptr;
		daily_cr = new reservation_info[daily_cr_cnt];

		set_temp_reservation(daily_cr);

		isok = check_c_reservation(daily_cr, daily_cr_cnt, b_time, e_time);
		
		delete[] daily_cr;
		daily_cr = nullptr;
	}

	return isok;
}

bool reservations::check_c_reservation(const string &r_day, const string &r_time) {
	for (int i = 0; i < reservation_cnt; i++) {
		if (all_reservation[i].customer_id == customer_id && all_reservation[i].app_date == r_day &&
			all_reservation[i].app_start_time <= r_time && all_reservation[i].app_end_time >= r_time
			&& all_reservation[i].iscancled != "Y") {
			return true;
		}
	}
	return false;
}
bool reservations::check_c_reservation(const reservation_info *temp_rinfo, const int &temp_rcnt, const string &b_time, const string &e_time) {
	
	if (temp_rcnt > 0) {
		for (int i = 0; i < temp_rcnt; i++) {
			if ((temp_rinfo[i].app_start_time <= b_time && temp_rinfo[i].app_end_time > b_time) ||
				(temp_rinfo[i].app_start_time <= e_time && temp_rinfo[i].app_end_time > e_time)) {
				cout << "\n\n\t=== [The Customer Already Reserved Another Service at the Time] ===\n";
				return false;
			}
		}
	}
	
	return true;
}

void reservations::print_available_services() {
	init_reservation_arr(RESERVATION_FILE);
	// Print the header
	cout << "\n\t===============================================================";
	cout << "\n\tCUSTOMER: " << customer.get_c_name(customer_id);
	cout << "\n\tDATE of RESERVATION: " << reservation_day;
	cout << "\n\tTIME of RESERVATION: " << reservation_time;
	cout << "\n\t---------------------------------------------------------------";
	cout << "\n\t\t\tList of All Available Services";
	cout << "\n\t===============================================================";
	
	int daily_cnt = set_temp_cnt(reservation_day);	//Calculate the number of reservation of the reservation day.
	bool isok; 
	string b_time, e_time;
	b_time = reservation_time;
	e_time = b_time;
	if (daily_cnt <= 0) {	// If no reservation on the reservation day, display all the services.
		cout << "\n\n\t=== No Reservation Was Found - All Services Are Available ===";
		service.print_all_serivces();
		cout << "\n\t===============================================================";
	}
	// Unless any reservation of the customer at the time slot,
	// check if there is any reservation of the customer within the service time,
	// and check if there is any reservation of the same service within the service time.
	else {
		if (isok = check_customer(b_time, e_time)) {
			reservation_info *daily_cr;
			daily_cr = new reservation_info[daily_cnt];

			set_temp_reservation(reservation_day, daily_cr);

			services::service_info *s = nullptr;
			s = service.get_all_services();

			string sid, type_id, time_id;
			for (int i = 0; i < service.get_service_cnt(); i++) {
				bool flag = true;
				int r_cnt = 0;
				for (int j = 0; j < daily_cnt; j++) {
					int bhour = stoi(b_time.substr(0, 2)),
						bmin = stoi(b_time.substr(3, 2));
					int emin = bmin + s[i].service_time;
					int ehour = bhour + emin / 60;
					emin = emin % 60;
					e_time = ((ehour < 10) ? "0" : "") + to_string(ehour) + ":" + ((emin < 10) ? "0" : "") + to_string(emin);

					if ((daily_cr[j].customer_id == customer_id)) {
						if ((daily_cr[j].app_start_time <= b_time && daily_cr[j].app_end_time > b_time) ||
							(daily_cr[j].app_start_time <= e_time && daily_cr[j].app_end_time > e_time)) {
							flag = false;
							break;
						}
					}
					else if (s[i].service_id == daily_cr[j].service_id && s[i].type_id == daily_cr[j].type_id) {
						if ((daily_cr[j].app_start_time <= b_time && daily_cr[j].app_end_time > b_time) ||
							(daily_cr[j].app_start_time < e_time && daily_cr[j].app_end_time >= e_time)) {
							r_cnt++;
							if (s[i].service_limit - r_cnt <= 0) {
								flag = false;
								break;
							}
						}
					}
				}	// for (int j...)

				
				if (flag) {
					if (s[i].service_id != sid) {
						sid = s[i].service_id;
						type_id = "", time_id = "";
						cout << "\n\t[" << sid << "] " << s[i].service_name << " Service";
					}
					if (s[i].type_id != type_id) {	
						type_id = s[i].type_id;
						time_id = "";
						cout << "\n\t\t[" << type_id << "] "
							<< (s[i].service_type.length() > 0 ? s[i].service_type : "No Type");
					}
					if (s[i].time_id != time_id) {	
						time_id = s[i].time_id;
						cout << "\n\t\t\t[" << time_id << "] " << s[i].service_time << " minutes";
					}
				}
			} // for (int i...)
			delete[] daily_cr;
			daily_cr = nullptr;
		} // if (daily_cnt...)
	}	
	return;
}

void reservations::input_services() {
	bool isok = false;
	string service_input, type_input, time_input;
	do {
		cout << "\n\n\t=================================";
		cout << "\n\tSelect the Service.";
		cout << "\n\t=================================\n";
		cout << "\n\tPlease Enter the Service ID: ";
		cin >> service_input;
		
		if (service.chk_serviceid(service_input)) {
			cout << "\n\t=================================";
			cout << "\n\tSelect the Type of Service.";
			cout << "\n\t=================================";
			cout << "\n\n\tPlease Enter the Type ID: ";
			cin >> type_input;

			if (service.chk_typeid(service_input, type_input)) {
				cout << "\n\t=================================";
				cout << "\n\tSelect the Time of Service.";
				cout << "\n\t=================================";
				cout << "\n\n\tPlease Enter the Time ID: ";
				cin >> time_input;

				if (service.chk_timeid(service_input, type_input, time_input)) {
					isok = true;
					break;
				}
			}
		}
	} while (!isok);
	
	set_range_of_time(reservation_time, service.get_service_time(service_input, type_input, time_input));
	reserve(service_input, type_input, time_input);

	return;
}

// Display all the availabe time slots for the customer by the time of check-out
void reservations::print_available_time() {
	init_reservation_arr(RESERVATION_FILE);
	customer.set_cur();
	string current_day = customer.get_cur_day();
	string chout_day = customer.get_chout_date(customer_id);

	int days_left = customer.get_days_left(current_day, customer.get_chout_date(customer_id));

	// Print the calendar header
	cout << "\n\t===================================================================================================";
	cout << "\n\tCUSTOMER: " << customer.get_c_name(customer_id);
	cout << "\n\tTODAY: " << current_day;
	cout << "\n\tCHECK-OUT: " << customer.get_chout_date(customer_id);
	cout << "\n\t---------------------------------------------------------------------------------------------------"; 
	cout << "\n\t\t\tList of All Available Time Slot";
	cout << "\n\t===================================================================================================";
	cout << "\n\t| TIME "; 

	int counter = 0;
	//bool go_next = false;

	string start_day = current_day;
	string end_day = (days_left < 7) ? get_nextday(start_day, days_left) : get_nextday(start_day, 6);
	string answer;
	
	int day_cnt = days_left;
	int turn = days_left / 7;

	for (int i = 0; i <= turn; i++) {
		if (turn > 0 && counter == 7) {
			bool isok = false;
			char ch;
			do {
				cout << "\n\t=== SEE MORE? (Y or N): ";
				cin >> answer;

				if (answer.length() == 1 && isalpha(ch = answer.at(0)) && (toupper(ch) == 'Y' || toupper(ch) == 'N')) {
					isok = true;
				}
				else {
					isok = false;
					cout << "\n\t=== [ERROR] Please Enter Y or N ===";
				}
			} while (!isok);

			if (isalpha(ch) && toupper(ch) == 'Y') {
				//go_next = true;
				start_day = current_day;
				end_day = (day_cnt < 7) ? get_nextday(start_day, day_cnt) : get_nextday(start_day, 6);
				counter = 0;
				cout << "\n\n\t===================================================================================================";
				cout << "\n\t| TIME ";
			}
			else {
				//go_next = false;
				break;
			}
		} // if (turn > 0 && counter == 7)
		do {
			cout << "| " << current_day << " ";
			current_day = get_nextday(current_day, 1);

			if (++counter == 7 || day_cnt == 0) {
				cout << "|";
				cout << "\n\t---------------------------------------------------------------------------------------------------";
			}
			day_cnt --;
		} while (customer.get_days_left(current_day, end_day) >= 0);

		// Display timeslotch
		string current_time;
		int current_hour = 8;
		int current_min = 0;

		for (int i = 0; i <= 24; i++) {
			day_cnt += counter;

			counter = 0;
			current_day = start_day;
			current_time = ((current_hour < 10) ? "0" : "") + to_string(current_hour) + ":" +
				((current_min < 10) ? "0" : "") + to_string(current_min);

			cout << "\n\t| " << current_time;

			do {
				cout << "|" << setw(12) << (check_c_reservation(current_day, current_time) ? "XXXXXXXXXXXX" : " ");
				current_day = get_nextday(current_day, 1);

				if (++counter == 7 || day_cnt == 0) 
					cout << "|";
				day_cnt--;
			} while (customer.get_days_left(current_day, end_day) >= 0);

			current_min += 30;
			current_hour += current_min / 60;
			current_min = current_min % 60;

			if (i == 24) 
				cout << "\n\t---------------------------------------------------------------------------------------------------";
		}
	}
	return;
}

string reservations::get_nextday(const string &today, const int &days) {
	string nextday;
	
	int m = stoi(today.substr(0, 2));
	int d = stoi(today.substr(3, 2));
	int y = stoi(today.substr(6, 4));

	tm t = {};
	t.tm_year = y - 1900;
	t.tm_mon = m - 1;
	t.tm_mday = d;

	t.tm_mday += days;

	mktime(&t);

	y = t.tm_year + 1900;
	m = t.tm_mon + 1;
	d = t.tm_mday;

	nextday = ((m < 10) ? "0" : "") + to_string(m) + "-" + ((d < 10) ? "0" : "") + to_string(d) +
		"-" + to_string(y);

	return nextday;
}

void reservations::reserve(string &sid, string &type_id, string &time_id) {
	customer.set_cur();
		
	ofstream rsrv_file;
	rsrv_file.open("reservations.txt", ios::out | ios::app);

	rsrv_file << customer_id << "," << sid << "," << type_id << "," << time_id << ","
		<< reservation_day << "," << begin_time << "," << end_time << ","
		<< customer.get_cur_day() << "," << customer.get_cur_hour() + ":" + customer.get_cur_min() << "," << "N" << ",,\n";
	rsrv_file.close();
	cout << "\n\t=== Reservation was Successfully Made...";
	cout << "\n\t=== The Service Fee is $"
		<< fixed << showpoint << setprecision(2)
		<< service.get_service_price(sid, type_id, time_id) * (double)service.get_service_time(sid, type_id, time_id) 
		<< " and Will Be Charged at the Time of Check-out. ===\n\n";

	return;
}

reservations::~reservations() {
	delete[] all_reservation;
	all_reservation = nullptr;
}
