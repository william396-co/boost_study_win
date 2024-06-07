#pragma once
#include <string>
#include <ctime>

namespace utils {
	inline std::string make_daytime_string() {
		time_t now = time(0);
		return ctime(&now);
	}
}