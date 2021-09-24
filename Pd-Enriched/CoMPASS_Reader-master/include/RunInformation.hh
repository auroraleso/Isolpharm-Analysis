#ifndef RUN_INFORMATION_H
#define RUN_INFORMATION_H

struct RunInformation
{
	std::string name;

	std::string time_start_UTC;
	std::string time_stop_UTC;

	time_t start;
	time_t stop;
};

#endif
