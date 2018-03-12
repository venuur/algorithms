#pragma once

#include <iostream>
#include <unordered_map>

template<class keyT, class valueT>
std::ostream& operator<<(std::ostream& os, const std::unordered_map<keyT, valueT>& out_map) {
	for (auto it : out_map) {
		os << "(" << it.first << ", " << it.second << ")\n";
	}

	return os;
}

template<class keyT, class valueT>
std::ostream& operator<<(std::ostream& os, const std::unordered_map<keyT, valueT*>& out_map) {
	for (auto it : out_map) {
		os << "(" << it.first << ", " << *(it.second) << ")\n";
	}

	return os;
}

template<class valueT>
std::ostream& operator<<(std::ostream& os, const std::vector<valueT> vec) {
	for (auto& it = begin(vec); it != end(vec); ++it) {
		os << *it << "\n";
	}
	return os;
}
