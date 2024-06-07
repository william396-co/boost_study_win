#include <string>
#include <ctime>

inline std::string make_daytime_string() {
	time_t now = time(0);
	return ctime(&now);
}