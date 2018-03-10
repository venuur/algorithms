#pragma once

#include <iostream>
#include <unordered_map>

using std::ostream;
using std::unordered_map;

template<class keyT, class valueT>
ostream& operator<<(ostream& os, const unordered_map<keyT, valueT>& out_map) {
	for (auto it : out_map) {
		cout << "(" << it.first << ", " << it.second << ")\n";
	}

	return os;
}

template<class keyT, class valueT>
ostream& operator<<(ostream& os, const unordered_map<keyT, valueT*>& out_map) {
	for (auto it : out_map) {
		cout << "(" << it.first << ", " << *(it.second) << ")\n";
	}

	return os;
}
