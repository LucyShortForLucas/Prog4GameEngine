#pragma once

#include <string>

#include "SdbmHash.h"

struct HashedString final {
	const std::string text;
	const unsigned int hash;

	template <size_t N>
	constexpr HashedString(const char(&_text)[N]) :
		hash{ eng::make_sdbm_hash(_text) },
		text{ _text } {
	}

	HashedString(const std::string& string) : 
		text{string},
		hash{eng::runtime_sdbm_hash(string)} {
	}

	bool operator<(const HashedString& other) {
		return hash < other.hash;
	}

	bool operator==(const HashedString& other) {
		return hash == other.hash;
	}
};