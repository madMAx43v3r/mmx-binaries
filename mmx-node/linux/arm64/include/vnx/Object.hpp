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

#ifndef INCLUDE_VNX_OBJECT_HPP_
#define INCLUDE_VNX_OBJECT_HPP_

#include <vnx/Object.h>
#include <vnx/Variant.hpp>
#include <vnx/Hash64.hpp>


namespace vnx {

template<typename T>
std::shared_ptr<T> Object::as_value() const {
	if(auto value = std::dynamic_pointer_cast<T>(to_value())) {
		return value;
	}
	auto tmp = T::create();
	if(tmp) {
		tmp->from_object(*this);
	}
	return tmp;
}

template<typename T>
bool Object::instanceof() const {
	if(const auto* type_code = get_type_code()) {
		return type_code->instanceof(T::VNX_TYPE_HASH);
	}
	return false;
}


} // vnx

#endif /* INCLUDE_VNX_OBJECT_HPP_ */
