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

#ifndef INCLUDE_VNX_VARIANT_HPP_
#define INCLUDE_VNX_VARIANT_HPP_

#include <vnx/Variant.h>
#include <vnx/Object.h>
#include <vnx/Input.hpp>
#include <vnx/Output.hpp>


namespace vnx {

template<typename T>
Variant& Variant::assign(const T& value) {
	clear();
	VectorOutputStream stream(&data);
	TypeOutput out(&stream);
	vnx::write_dynamic(out, value);
	out.flush();
	return *this;
}

template<typename T>
void Variant::to(T& value) const {
	VectorInputStream stream(&data);
	TypeInput in(&stream);
	if(data.empty()) {
		const uint16_t code = CODE_NULL;
		vnx::type<T>().read(in, value, nullptr, &code);
	} else {
		vnx::read_dynamic(in, value);
	}
}

inline
std::string Variant::to_string() const {
	return vnx::to_string(*this);
}

inline
std::string Variant::to_string_value() const {
	return vnx::to_string_value(*this);
}

inline
Object Variant::to_object() const {
	return to<Object>();
}

inline
std::ostream& operator<<(std::ostream& out, const Variant& value) {
	vnx::write(out, value);
	return out;
}

inline
std::istream& operator>>(std::istream& in, Variant& value) {
	vnx::read(in, value);
	return in;
}


} // vnx

#endif /* INCLUDE_VNX_VARIANT_HPP_ */
