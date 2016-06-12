// Implemecation file for the misc (miscellanea) class.
#include "misc.h"

misc::misc() {
}

bool misc::check_main_choice(const string &main_choice) const {
	if (main_choice.length() == 1) {
		if (isdigit(main_choice.at(0)) && stoi(main_choice) > 0 && stoi(main_choice) < 4)
			return true;
		else if (toupper(main_choice.at(0)) == 'Q')
			return true;
	}
	
	return false;
}

misc::~misc() {
}