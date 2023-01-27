/*
 * exception.h
 *
 *  Created on: Apr 5, 2020
 *      Author: mad
 */

#ifndef INCLUDE_VNX_EXCEPTION_H_
#define INCLUDE_VNX_EXCEPTION_H_

#include <vnx/Exception.hxx>
#include <vnx/permission_e.hxx>

#include <stdexcept>


namespace vnx {

class exception : public std::exception {
public:
	exception() = default;
	
	exception(const std::string& message) : what_(message) {}

	exception(std::shared_ptr<const Exception> value)
		:	value_(value)
	{
		if(value) {
			if(value->what.empty()) {
				what_ = value->get_type_name();
			} else {
				what_ = value->what;
			}
		} else {
			what_ = "unknown exception";
		}
	}
	
	std::shared_ptr<const Exception> value() const {
		return value_;
	}
	
	const char* what() const noexcept override {
		return what_.c_str();
	}
	
private:
	std::shared_ptr<const Exception> value_;
	
	std::string what_;
	
};


class permission_denied : public std::runtime_error {
public:
	permission_denied(const std::string& perm)
		:	runtime_error("permission denied (" + perm + ")") {}

	template<typename T>
	permission_denied(const T& perm)
		:	runtime_error("permission denied (" + vnx::to_string_value_full(perm) + ")") {}

};


} // vnx

#endif /* INCLUDE_VNX_EXCEPTION_H_ */
