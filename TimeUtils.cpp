//Routines that get the system date and time and produce them as a formatted strings

#include "TimeUtils.h"

void getSystemTime(int& hrs, int& mins, int& secs) 
{ //get time from system
	//set to system time
	time_t now(time(0));
	struct tm t;
	localtime_s(&t, &now);
	hrs  = t.tm_hour ;
	mins = t.tm_min ;
	secs = t.tm_sec ;
}

const string timeToString(int h, int m, int s)
{ //convert the time to a string in 24-h digital clock format (00:00:00)
    ostringstream os;
    const char prev(os.fill ('0'));
    os << setw(2) << h << ":"
       << setw(2) << m << ":"
       << setw(2) << s;
    os.fill(prev);
    return os.str();
}

const string getTime() 
{ //return the current time in a string format
	int hrs, mins, secs;		//hold the current time
	getSystemTime(hrs, mins, secs);
	return timeToString(hrs, mins, secs);
}

void getSystemDate(int& day, int& month, int& year)
{ //get date from system
	time_t now(time(0));
	struct tm t;
	localtime_s(&t, &now);
	day = t.tm_mday;
	month = t.tm_mon + 1;
	year = t.tm_year + 1900;
}

const string dateToString(int day, int month, int year) 
{ //convert the date to a string in format (dd/mm/yyyy)
	ostringstream os;
    const char prev(os.fill ('0'));
    os << setw(2) << day << "/"
       << setw(2) << month << "/"
       << setw(4) << year;
    os.fill(prev);
    return os.str();
}

const string getDate() 
{ //return the current date in a string format
	int day, month, year;		//hold the current date
	getSystemDate(day, month, year);
	return dateToString(day, month, year);
}
