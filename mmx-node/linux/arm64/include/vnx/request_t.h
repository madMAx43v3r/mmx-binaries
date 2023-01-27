/*
 * request_t.h
 *
 *  Created on: Oct 30, 2020
 *      Author: mad
 */

#ifndef INCLUDE_VNX_REQUEST_T_H_
#define INCLUDE_VNX_REQUEST_T_H_

#include <vnx/Exception.hxx>

#include <stdexcept>
#include <functional>


namespace vnx {

/// Data type to store a pending async request.
template<typename... T>
struct request_t {

	std::function<void(T...)> async_return;

	std::function<void(std::shared_ptr<const Value>)> async_error;

	request_t() = default;

	request_t(const request_t &other) = delete;

	request_t(request_t &&other) {
		*this = std::move(other);
	}

	request_t(	const std::function<void(T...)>& async_return_,
				const std::function<void(std::shared_ptr<const Value>)>& async_error_)
		:	async_return(async_return_), async_error(async_error_)
	{
	}

	~request_t() {
		cancel();
	}

	request_t &operator=(const request_t &other) = delete;

	request_t &operator=(request_t &&other) {
		cancel();
		async_return = std::move(other.async_return);
		async_error = std::move(other.async_error);
		other.clear();
		return *this;
	}

	operator bool() const {
		return bool(async_return);
	}

	void callback(const T &...return_value) {
		if(async_return) {
			async_return(return_value...);
		}
		clear();
	}

	void callback_error(const std::exception& ex) {
		callback_error(vnx::Exception::from_what(ex.what()));
	}

	void callback_error(const std::string& ex_what) {
		callback_error(vnx::Exception::from_what(ex_what));
	}

	void callback_error(std::shared_ptr<const Value> ex) {
		if(async_error) {
			async_error(ex);
		}
		clear();
	}

	void clear() {
		async_return = std::function<void(T...)>();
		async_error = std::function<void(std::shared_ptr<const Value>)>();
	}

	void cancel() {
		if(async_error) {
			async_error(vnx::Exception::from_what("request aborted"));
		}
		clear();
	}

};

typedef request_t<> void_request_t;


} // vnx

#endif /* VNX_INCLUDE_VNX_REQUEST_T_H_ */
