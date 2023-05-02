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

#ifndef INCLUDE_VNX_UTIL_H_
#define INCLUDE_VNX_UTIL_H_

#include <cstdio>
#include <cstdint>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>

#include <sys/stat.h>
#include <sys/types.h>

#ifndef _MSC_VER
#include <unistd.h>
#else
void usleep(int64_t usec);
#endif


namespace vnx {

#ifdef _MSC_VER
typedef struct ::_stat64 stat;
#else
typedef struct ::stat stat;
#endif

inline int get_stat(const std::string& path, vnx::stat& info)
{
#ifdef _MSC_VER
	return ::_stat64(path.c_str(), &info);
#else
	return ::stat(path.c_str(), &info);
#endif
}

inline int64_t ftell(FILE* stream)
{
#ifdef _MSC_VER
	return ::_ftelli64(stream);
#else
	return ::ftello(stream);
#endif
}

inline int fseek(FILE* stream, int64_t offset, int whence)
{
#ifdef _MSC_VER
	return ::_fseeki64(stream, offset, whence);
#else
	return ::fseek(stream, offset, whence);
#endif
}

/// Substitudes any occurrence of \p from with \p to and returns the resulting string.
std::string string_subs(std::string str, const std::string& from, const std::string& to);

/** \brief Split string into multiple parts based on special char \p sep.
 * 
 * An empty input string results in a vector of size 1 with an empty string as the only element.
 *
 * Set \p clean to true to not get any empty strings. This can be useful if \p sep occurs
 * multiple times in a row and empty array elements are unwanted.
 * In that case, an empty input string results in an empty vector.
 */
std::vector<std::string> string_split(const std::string& str, char sep, bool clean = false);

/// Compares two strings, case insensitive
bool string_iequals(const std::string& L, const std::string& R);

/// Returns a very good 64-bit random number. (Initialized with nano-second timestamp)
uint64_t rand64();

/// Converts a binary string to a hex string, with constant length. (Without leading "0x")
std::string to_hex_string(const void* data, const size_t length, bool big_endian = false, bool lower_case = false);

/// Converts a hex string to binary vector. (Handles leading "0x")
std::vector<uint8_t> from_hex_string(const std::string& str);

/// Converts an integer to a hex string (big endian), without leading zeros. (Without leading "0x", upper case chars)
template<typename T>
std::string to_hex_string(const T& value) {
	std::stringstream ss;
	ss << std::uppercase << std::hex << value;
	return ss.str();
}

/// Converts an integer to a hex string (big endian), without leading zeros. (Without leading "0x", lower case chars)
template<typename T>
std::string to_hex_string_lower(const T& value) {
	std::stringstream ss;
	ss << std::hex << value;
	return ss.str();
}

/// Converts ASCII string to lower case
inline
std::string ascii_tolower(std::string str)
{
	std::transform(str.begin(), str.end(), str.begin(), [](char c){ return std::tolower(c); });
	return str;
}


/// Expects one line of input from the user with echoing disabled. During input, process log output is paused.
std::string input_password(const std::string &prompt="");

/// Sets a socket into non-blocking mode
void set_socket_nonblocking(int fd);

/// Get error text for last errno
std::string get_socket_error_text();

/// Get this machines host name
std::string get_host_name();

/// Get peer address for socket
std::string get_peer_address(int sock);


} // vnx

#endif /* INCLUDE_VNX_UTIL_H_ */
