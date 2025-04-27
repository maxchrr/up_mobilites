#ifndef UTILS_H_
#define UTILS_H_

int rand_range(int min, int max);

struct Date
{
	int day;
	int month;
	int year;
};

struct Date rand_date(int start_year, int end_year);

#endif // UTILS_H_
