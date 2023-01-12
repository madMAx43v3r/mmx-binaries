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

#ifndef INCLUDE_VNX_BUFFER_HPP_
#define INCLUDE_VNX_BUFFER_HPP_

#include <vnx/Buffer.h>


namespace vnx {

inline
Buffer::Buffer(size_t init_capacity) {
	reserve(init_capacity);
}

inline
Buffer::Buffer(const Buffer& other) {
	*this = other;
}

inline
Buffer::Buffer(const std::string& other) {
	*this = other;
}

inline
Buffer::~Buffer() {
	reset();
}

inline
Buffer& Buffer::operator=(const Buffer& other) {
	resize(other.size_);
	::memcpy(data_, other.data_, other.size_);
	return *this;
}

inline
Buffer& Buffer::operator=(const std::string& other) {
	resize(other.size());
	::memcpy(data_, other.data(), other.size());
	return *this;
}

inline
void Buffer::release() {
	data_ = nullptr;
	size_ = 0;
	capacity_ = 0;
}

inline
void Buffer::clear() {
	size_ = 0;
}

inline
void Buffer::reset() {
	if(data_) {
		::free(data_);
	}
	data_ = nullptr;
	clear();
	capacity_ = 0;
}

inline
std::string Buffer::as_string() const {
	std::string result;
	result.resize(size_);
	::memcpy(&result[0], data_, size_);
	return result;
}


} // vnx

#endif /* INCLUDE_VNX_BUFFER_HPP_ */
