 /*************************************************************************
 *
 *  [2017] - [2018] Automy Inc.
 *  All Rights Reserved.
 *
 * NOTICE:  All information contained herein is, and remains
 * the property of Automy Incorporated and its suppliers,
 * if any.  The intellectual and technical concepts contained
 * herein are proprietary to Automy Incorporated
 * and its suppliers and may be covered by U.S. and Foreign Patents,
 * patents in process, and are protected by trade secret or copyright law.
 * Dissemination of this information or reproduction of this material
 * is strictly forbidden unless prior written permission is obtained
 * from Automy Incorporated.
 */

#ifndef INCLUDE_VNX_OUTPUT_HPP_
#define INCLUDE_VNX_OUTPUT_HPP_

#include <vnx/Output.h>
#include <vnx/ToStringValue.h>
#include <vnx/Directory.h>
#include <vnx/File.h>
#include <fstream>


namespace vnx {

/** \brief Converts value to regular string
 *
 * Same as to_string<T>() except strings and enum values are without quotes.
 * In addition null values result in an empty string instead of "null".
 */
template<typename T>
std::string to_string_value(const T& value) {
	std::ostringstream stream;
	ToStringValue visitor(stream);
	vnx::type<T>().accept(visitor, value);
	return stream.str();
}

template<> inline
std::string to_string_value(const std::string& value) {
	return value;
}


/// Same as to_string_value(), except for enums (will include type name)
template<typename T>
std::string to_string_value_full(const T& value) {
	return to_string_value(value);
}


template<typename T>
void write_to_file_json(const std::string& file_name, const T& value) {
	if(file_name.empty()) {
		return;
	}
	size_t slash = file_name.find_last_of("/\\");
	if(slash != std::string::npos){
		std::string dir_name = file_name.substr(0, slash);
		Directory dir(dir_name);
		dir.create();
	}
	const std::string tmp_file_name = file_name + ".tmp";
	{
		std::ofstream stream(tmp_file_name);
		write(stream, value);
	}
#ifdef _WIN32
	::remove(file_name.c_str());
#endif
	if(::rename(tmp_file_name.c_str(), file_name.c_str())) {
		throw std::runtime_error("rename('" + tmp_file_name + "', '" + file_name + "') failed with: " + std::string(strerror(errno)));
	}
}



} // vnx

#endif /* INCLUDE_VNX_OUTPUT_HPP_ */
