/*
 * Util.hpp
 *
 *  Created on: Nov 30, 2020
 *      Author: jaw
 */

#ifndef INCLUDE_VNX_UTIL_HPP_
#define INCLUDE_VNX_UTIL_HPP_

#include <vnx/Util.h>
#include <vnx/Input.hpp>
#include <vnx/Generic.hxx>

#include <string>


namespace vnx {

template<typename T>
std::string string_join(const T& collection, const std::string& sep) {
	std::string result = "";
	auto iter = collection.begin();
	while(iter != collection.end()) {
		result += vnx::to_string_value(*iter);
		iter++;
		if(iter != collection.end()) {
			result += sep;
		}
	}
	return result;
}

/// Generic fall-back to reading at top level
template<typename T>
void read_generic(TypeInput& in, T& value) {
	if(auto generic = std::dynamic_pointer_cast<Generic>(vnx::read(in))) {
		generic->data.to(value);
	} else {
		value = T();
	}
}


} // vnx

#endif /* INCLUDE_VNX_UTIL_HPP_ */
