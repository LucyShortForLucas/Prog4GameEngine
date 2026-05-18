#pragma once

#include "SdbmHash.h"

struct HashedString {	
	const std::string text;
	const unsigned int hash;

	template <size_t N>
	constexpr HashedString(const char(&string)[N]) {
		text = string;
		hash = make_sdbm_hash(string);
	}
};