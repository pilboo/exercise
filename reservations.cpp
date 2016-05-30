#include "reservations.h"
#include "services.h"

reservations::reservations() {
	rsrv_cnt = 0;
	srvc_tm = 0;
	c_srvc_tm = 0;
	s_name = "";
	t_name = "";
	rsrv_arr = nullptr;
}

reservations::reservations(string &txt) {
	rsrv_cnt = 0;
	srvc_tm = 0;
	c_srvc_tm = 0;
	s_name = "";
	t_name = "";
	init_rsrv_arr(txt);
}

void reservations::init_rsrv_arr(string &txt) {
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
			rsrv_cnt++;
		}

		reservation_file.clear();
		reservation_file.seekg(0, ios::beg);

		rsrv_arr = new s_reservation[rsrv_cnt];
		int arr_index = 0;
		int pos1, pos2;

		while (!reservation_file.eof()) {
			getline(reservation_file, temp_line);

			pos1 = 0;
			pos2 = temp_line.find(",", pos1);
			rsrv_arr[arr_index].customer_id = temp_line.substr(pos1, pos2 - pos1);

			pos1 = pos2 + 1;
			pos2 = temp_line.find(",", pos1);
			rsrv_arr[arr_index].service_id = temp_line.substr(pos1, pos2 - pos1);

			pos1 = pos2 + 1;
			pos2 = temp_line.find(",", pos1);
			rsrv_arr[arr_index].type_id = temp_line.substr(pos1, pos2 - pos1);

			pos1 = pos2 + 1;
			pos2 = temp_line.find(",", pos1);
			rsrv_arr[arr_index].time_id = temp_line.substr(pos1, pos2 - pos1);

			pos1 = pos2 + 1;
			pos2 = temp_line.find(",", pos1);
			rsrv_arr[arr_index].app_date = temp_line.substr(pos1, pos2 - pos1);

			pos1 = pos2 + 1;
			pos2 = temp_line.find(",", pos1);
			rsrv_arr[arr_index].app_start_time = temp_line.substr(pos1, pos2 - pos1);

			pos1 = pos2 + 1;
			pos2 = temp_line.find(",", pos1);
			rsrv_arr[arr_index].app_end_time = temp_line.substr(pos1, pos2 - pos1);

			pos1 = pos2 + 1;
			pos2 = temp_line.find(",", pos1);
			rsrv_arr[arr_index].input_date = temp_line.substr(pos1, pos2 - pos1);

			pos1 = pos2 + 1;
			pos2 = temp_line.find(",", pos1);
			rsrv_arr[arr_index].input_time = temp_line.substr(pos1, pos2 - pos1);

			pos1 = pos2 + 1;
			pos2 = temp_line.find(",", pos1);
			rsrv_arr[arr_index].iscancle = temp_line.substr(pos1, pos2 - pos1);

			pos1 = pos2 + 1;
			pos2 = temp_line.find(",", pos1);
			rsrv_arr[arr_index].cancel_date = temp_line.substr(pos1, pos2 - pos1);

			pos1 = pos2 + 1;
			pos2 = temp_line.find(",", pos1);
			rsrv_arr[arr_index].cancel_time = temp_line.substr(pos1, pos2 - pos1);

			arr_index++;
		}
	}
	reservation_file.close();

	return;
}

void reservations::print_service_cal(string &sid, string &tid, string &rdate, string &rtime) {
	for (int i = 0; i < rsrv_cnt; i++) {
		if (rdate == rsrv_arr[i].app_date && 
			rtime >= rsrv_arr[i].app_start_time && rtime <= rsrv_arr[i].app_end_time &&
			sid == rsrv_arr[i].service_id && tid == rsrv_arr[i].type_id &&
			rsrv_arr[i].iscancle != "Y") {
			
			cout << "\tRESERVED";
			break;
		}
		
		if (i == rsrv_cnt - 1)
			cout << "\t        ";
	}
	return;
}

void reservations::print_customer_cal(string &cid, string &rdate, string &rtime) {
	for (int i = 0; i < rsrv_cnt; i++) {
		if (cid == rsrv_arr[i].customer_id && rdate == rsrv_arr[i].app_date &&
			rtime >= rsrv_arr[i].app_start_time && rtime <= rsrv_arr[i].app_end_time &&
			rsrv_arr[i].iscancle != "Y") {

			string s_id = rsrv_arr[i].service_id;
			string ty_id = rsrv_arr[i].type_id;
			string tm_id = rsrv_arr[i].time_id;

			string SF_NAME = "services.txt";
			services s(SF_NAME);

			s_name = s.get_servicename(s_id);
			t_name = s.get_servicetype(s_id, ty_id);

			s.~services();
			
			cout << "\t\t" << s_name << ((t_name.length() > 0) ? "-" : "") << t_name;
			break;
		}
	}
	return;
}

bool reservations::chk_apptime(string &apptime, int stime, int bhour, int bmin, int ehour, int emin) {
	bool isok = false;
	string app_stime = apptime;
	string app_etime;
		
	int temp_hour = stoi(app_stime.substr(0, 2));
	int temp_min = stoi(app_stime.substr(3, 2));

	temp_min += stime;
	temp_hour = temp_hour + temp_min / 60;
	temp_min = temp_min % 60;

	app_etime = (temp_hour < 10 ? "0" : "") + to_string(temp_hour) + ":" + (temp_min < 10 ? "0" : "") + to_string(temp_min);
	string btime = (bhour < 10 ? "0" : "") + to_string(bhour) + ":" + (bmin < 10 ? "0" : "") + to_string(bmin);
	string etime = (ehour < 10 ? "0" : "") + to_string(ehour) + ":" + (emin < 10 ? "0" : "") + to_string(emin);

	
	if (app_stime >= btime && app_etime <= etime) {
		isok = true;
	}

	return isok;
}

bool reservations::chk_service(string &sid, string &tid, string &adate, string &stime, string &etime, int slimt) {
	int r_count = 0;
	for (int i = 0; i < rsrv_cnt; i++) {
		if (sid == rsrv_arr[i].service_id && tid == rsrv_arr[i].type_id && adate == rsrv_arr[i].app_date) {
			if ((stime >= rsrv_arr[i].app_start_time && stime < rsrv_arr[i].app_end_time) ||
				(etime >= rsrv_arr[i].app_start_time && etime < rsrv_arr[i].app_end_time)) {
				r_count++;
				if (slimt - r_count <= 0)
					cout << "\n\n\t=== [ERROR] Duplicated reservation is not allowed ===\n";
					return false;
			}
		}
	}

	return true;
}

bool reservations::chk_customer(string &cid, string &adate, string &stime, string &etime) {
	for (int i = 0; i < rsrv_cnt; i++) {
		if (cid == rsrv_arr[i].customer_id && adate == rsrv_arr[i].app_date && rsrv_arr[i].iscancle != "Y") {
			if ((stime >= rsrv_arr[i].app_start_time && stime < rsrv_arr[i].app_end_time) ||
				(etime >= rsrv_arr[i].app_start_time && etime < rsrv_arr[i].app_end_time)) {
				cout << "\n\n\t=== [ERROR] Duplicated reservation is not allowed ===\n";
				return false;
			}
		}
	}

	return true;
}

bool reservations::reserve(string &cid, string &sid, string &tyid, string &tid, string &adt, string &atm, int stm, string &cdt, string &ctm) {
	string SF_NAME = "services.txt";
	services s(SF_NAME);

	int s_limit = s.get_servicelimit(sid, tyid, tid);
	double s_price = s.get_serviceprice(sid, tyid, tid);

	s.~services();

	string start_time = atm;
	string end_time;

	int temp_hour = stoi(start_time.substr(0, 2));
	int temp_min = stoi(start_time.substr(3, 2));

	temp_min += stm;
	temp_hour = temp_hour + temp_min / 60;
	temp_min = temp_min % 60;

	end_time = (temp_hour < 10 ? "0" : "") + to_string(temp_hour) + ":" +
		(temp_min < 10 ? "0" : "") + to_string(temp_min);

	bool serviceok = chk_service(sid, tyid, adt, start_time, end_time, s_limit);
	bool customerok = chk_customer(cid, adt, start_time, end_time);

	if (serviceok && customerok) {
		ofstream rsrv_file;
		rsrv_file.open("reservations.txt", ios::out | ios::app);
		rsrv_file << cid << "," << sid << "," << tyid << "," << tid << "," 
			<< adt << "," << start_time << "," << end_time << ","
			<< cdt << "," << ctm << "," << "N" << ",,\n";
		rsrv_file.close();
		cout << "\n\tReservation was successfully made.\n";
		return true;
	} 

	return false;
}

reservations::~reservations() {
	delete[] rsrv_arr;
	rsrv_arr = nullptr;
}
