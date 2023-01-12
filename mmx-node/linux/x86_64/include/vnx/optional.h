/*
 * optional.h
 *
 *  Created on: Jan 12, 2021
 *      Author: mad
 */

#ifndef INCLUDE_VNX_OPTIONAL_H_
#define INCLUDE_VNX_OPTIONAL_H_

#include <utility>


namespace vnx {

template<typename T>
class optional {
public:
	optional() = default;

	optional(T* value) {
		ptr = value;
	}

	optional(const T& value) {
		ptr = new T(value);
	}

	optional(const std::nullptr_t&) {
		// nothing to do
	}

	optional(const optional& other) {
		*this = other;
	}

	optional(optional&& other) {
		*this = std::move(other);
	}

	~optional() {
		clear();
	}

	operator bool() const {
		return bool(ptr);
	}

	T& operator*() {
		return *ptr;
	}

	const T& operator*() const {
		return *ptr;
	}

	T* operator->() {
		return ptr;
	}

	const T* operator->() const {
		return ptr;
	}

	optional& operator=(T* value) {
		clear();
		ptr = value;
		return *this;
	}

	optional& operator=(const T& value) {
		if(!ptr) {
			ptr = new T(value);
		} else {
			*ptr = value;
		}
		return *this;
	}

	optional& operator=(T&& value) {
		if(!ptr) {
			ptr = new T(std::move(value));
		} else {
			*ptr = std::move(value);
		}
		return *this;
	}

	optional& operator=(const std::nullptr_t&) {
		clear();
		return *this;
	}

	optional& operator=(const optional& other) {
		if(other) {
			this->operator=(*other);
		} else {
			clear();
		}
		return *this;
	}

	optional& operator=(optional&& other) {
		clear();
		std::swap(ptr, other.ptr);
		return *this;
	}

	template<typename S>
	bool operator==(const optional<S>& other) const {
		return (ptr && other && *ptr == *other) || (!ptr && !other);
	}

	template<typename S>
	bool operator!=(const optional<S>& other) const {
		return !(*this == other);
	}

	const T* get() const {
		return ptr;
	}

	void reset(T* value) {
		clear();
		ptr = value;
	}

	T* release() {
		auto tmp = ptr;
		ptr = nullptr;
		return tmp;
	}

	void clear() {
		if(ptr) {
			delete ptr;
			ptr = nullptr;
		}
	}

private:
	T* ptr = nullptr;

};


} // vnx

#endif /* INCLUDE_VNX_OPTIONAL_H_ */
