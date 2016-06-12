// Specification file for the misc (miscellanea) class.
#ifndef MISC_H
#define MISC_H

#include <string>
using namespace std;

class misc {
private:

public:
	misc();
	bool check_main_choice(const string&) const;
	~misc();
};

#endif
