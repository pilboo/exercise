// 

#include "cancel_reservation.h"

cancel_reservation::cancel_reservation() {
	reservation_cnt = 0;

	cancel_day = "";
	cancel_time = "";
}

void cancel_reservation::set_customer_id(const string &cid) {
	customer_id = cid;
}

void cancel_reservation::set_cancel_date_time(string &c_day) {
	bool isok = false;
	bool isok_2 = false;
	customer.set_cur();
	string start_day = customer.get_cur_day();
	string end_day = customer.get_chout_date(customer_id);
	string c_time;

	do {
		cout << "\n\n\t============================================================================";
		cout << "\n\tSelect the Day of Cancelation between " << start_day << " and " << end_day;
		cout << "\n\t============================================================================";
		cout << "\n\tPlease Enter the Day of Cancelation (mm-dd-yyyy or 0 for Previous Menu): ";
		getline(cin, c_day);

		if (c_day == "0") {
			break;
		}
		else if (reservation.check_date(c_day) && reservation.check_range_of_date(c_day, start_day, end_day)) {
			if (!check_cancel_date(c_day)) {
				cout << "\n\t=== [ERROR] There is NO Reservation on " << c_day << " ===\n";
				isok = false;
			}
			else {

				do {
					cout << "\n\tPlease Enter the Reservation Time (hh:mm - e.g., 13:00): ";
					getline(cin, c_time);

					if (reservation.check_time(c_time)) {
						if (c_time >= "08:00" && c_time <= "20:00") {
							if (check_cancel_date(c_day, c_time)) {
								cancel_day = c_day;
								cancel_time = c_time;
								isok = true;
								break;
							}
							else {
								cout << "\n\t===[ERROR] There is no reservation at " <<c_time << ", " << c_day << " ===\n";
								isok_2 = false;
							}
						}
						else {
							cout << "\n\t===[ERROR] Please Enter a Valid Time (08:00 - 20:00) ===\n";
							isok_2 = false;
						}
					}
				} while (!isok_2);
			} // End of if (check_cancel_date(c_day))
		} // End of if (c_day == "Q" ...)
	} while (!isok);

	return;
}

bool cancel_reservation::check_cancel_date(const string &c_day) {
	int daily_r_cnt = reservation.set_temp_cnt(c_day);
	bool isfound = false;

	if (daily_r_cnt > 0) {
		reservations::reservation_info *daily_r = nullptr;
		daily_r = new reservations::reservation_info[daily_r_cnt];

		reservation.set_temp_reservation(c_day, daily_r);
		
		for (int i = 0; i < daily_r_cnt; i++) {
			if (daily_r[i].customer_id == customer_id && daily_r[i].app_date == c_day && daily_r[i].iscanceled != "Y") {
				isfound = true;
				break;
			}
		} // End of for ...

		delete[] daily_r;
		daily_r = nullptr;
	} // End of if (daily_r_cnt > 0)

	return isfound;
}

bool cancel_reservation::check_cancel_date(const string &c_day, const string &c_time) {
	int daily_r_cnt = reservation.set_temp_cnt(c_day);
	bool isfound = false;

	if (daily_r_cnt > 0) {
		reservations::reservation_info *daily_r = nullptr;
		daily_r = new reservations::reservation_info[daily_r_cnt];

		reservation.set_temp_reservation(c_day, daily_r);

		for (int i = 0; i < daily_r_cnt; i++) {
			if (daily_r[i].customer_id == customer_id && daily_r[i].app_date == c_day && 
				daily_r[i].app_start_time == c_time && daily_r[i].iscanceled != "Y") {
				isfound = true;
				break;
			}
		} // End of for ...

		delete[] daily_r;
		daily_r = nullptr;
	} // End of if (daily_r_cnt > 0)

	return isfound;
}

void cancel_reservation::cancel() {
	ifstream r_file(RESERVATION_FILE);
	reservations::reservation_info *temp_r = nullptr;
	int r_cnt = reservation.get_reservation_cnt();
	temp_r = new reservations::reservation_info[r_cnt];
	int index = 0, pos = 0;

	if (!r_file) {
		cout << "\n=============================================="
			<< "\n|| Error: Cannot open the reservation file. ||"
			<< "\n==============================================" << endl;
		exit(0);
	}
	else {
		string temp_line;
		
		for (index = 0; index < r_cnt; index++) {
			getline(r_file, temp_line);

			int pos1 = 0, pos2 = 0;
			string temp_arr[12];

			for (int i = 0; i < 12; i++) {
				pos2 = temp_line.find(",", pos1);
				temp_arr[i] = temp_line.substr(pos1, pos2 - pos1);
				pos1 = pos2 + 1;
			}

			temp_r[index].customer_id = temp_arr[0];
			temp_r[index].service_id = temp_arr[1];
			temp_r[index].type_id = temp_arr[2];
			temp_r[index].time_id = temp_arr[3];
			temp_r[index].app_date = temp_arr[4];
			temp_r[index].app_start_time = temp_arr[5];
			temp_r[index].app_end_time = temp_arr[6];
			temp_r[index].input_date = temp_arr[7];
			temp_r[index].input_time = temp_arr[8];
			temp_r[index].iscanceled = temp_arr[9];
			temp_r[index].cancel_date = temp_arr[10];
			temp_r[index].cancel_time = temp_arr[11];

			if (temp_r[index].customer_id == customer_id && temp_r[index].app_date == cancel_day &&
				temp_r[index].app_start_time == cancel_time && temp_r[index].iscanceled == "N")
				pos = index;

		} // End of for (int index = 0; index < r_cnt; index++)
	} // End of if (!r_file)
	
	r_file.close();

	customer.set_cur();

	string appdate = temp_r[pos].app_date;
	string apptime = temp_r[pos].app_start_time;
	string inpdate = temp_r[pos].input_date;
	string inptime = temp_r[pos].input_time;

	if (check_cancel_avail(appdate, apptime, inpdate, inptime)) {

		temp_r[pos].iscanceled = "Y";
		temp_r[pos].cancel_date = customer.get_cur_day();
		temp_r[pos].cancel_time = customer.get_cur_hour() + ":" + customer.get_cur_min();

		ofstream ro_file(RESERVATION_FILE, ios::out);

		for (int i = 0; i < index; i++) {
			ro_file << ((i == 0) ? "" : "\n") << temp_r[i].customer_id << ","
				<< temp_r[i].service_id << ","
				<< temp_r[i].type_id << ","
				<< temp_r[i].time_id << ","
				<< temp_r[i].app_date << ","
				<< temp_r[i].app_start_time << ","
				<< temp_r[i].app_end_time << ","
				<< temp_r[i].input_date << ","
				<< temp_r[i].input_time << ","
				<< temp_r[i].iscanceled << ","
				<< temp_r[i].cancel_date << ","
				<< temp_r[i].cancel_time;
		}
		ro_file.close();
		cout << "\n\n\t=== [The Reservation Was Successfully Canceled] ===\n";
	}
	else
		cout << "\n\n\t=== [Failed to Cancel the Reservation] ===";
	

	return;
}

bool cancel_reservation::check_cancel_avail(const string &app_day, const string &app_time, const string &i_day, const string &i_time) {
	customer.set_cur();
	string cur_day = customer.get_cur_day();
	string cur_time = customer.get_cur_hour() + ":" + customer.get_cur_min();

	int from_input_time = 0, from_app_time = 0;
	
	from_input_time = get_times_left(cur_day, cur_time, i_day, i_time);
	from_app_time = get_times_left(app_day, app_time, cur_day, cur_time);

	if (from_input_time > 10) {
		cout << "\n\n\t=== [More Than 10 Minutes AFTER the Time of Making the Reservation] ===";
		return false;
	}
	else if (from_app_time <= 90) {
		cout << "\n\n\t=== [Less Than 90 Minutes BEFORE the Reservation TIME] ===";
		return false;
	}
	return true;
}


int cancel_reservation::get_times_left(const string &b_day, const string &b_time, const string &e_day, const string &e_time) {
	time_t bdate, edate;
	struct tm tm_bdate, tm_edate;

	time(&bdate);
	time(&edate);
	localtime_s(&tm_bdate, &bdate);
	localtime_s(&tm_edate, &edate);

	tm_bdate.tm_mon = stoi(b_day.substr(0, 2)) - 1;
	tm_bdate.tm_mday = stoi(b_day.substr(3, 2));
	tm_bdate.tm_year = stoi(b_day.substr(6, 4)) - 1900;
	tm_bdate.tm_hour = stoi(b_time.substr(0, 2));
	tm_bdate.tm_min = stoi(b_time.substr(3, 2));

	tm_edate.tm_mon = stoi(e_day.substr(0, 2)) - 1;
	tm_edate.tm_mday = stoi(e_day.substr(3, 2));
	tm_edate.tm_year = stoi(e_day.substr(6, 4)) - 1900;
	tm_edate.tm_hour = stoi(e_time.substr(0, 2));
	tm_edate.tm_min = stoi(e_time.substr(3, 2));

	bdate = mktime(&tm_bdate);
	edate = mktime(&tm_edate);

	

	return (int)difftime(bdate, edate) / 60;
}

cancel_reservation::~cancel_reservation() {
}