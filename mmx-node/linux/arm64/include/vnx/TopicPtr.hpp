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

#ifndef INCLUDE_VNX_TOPICPTR_HPP_
#define INCLUDE_VNX_TOPICPTR_HPP_

#include <vnx/package.hxx>
#include <vnx/TopicPtr.h>
#include <vnx/Input.hpp>
#include <vnx/Output.hpp>
#include <vnx/Visitor.h>


namespace vnx {

inline
TopicPtr::TopicPtr() {}

inline
TopicPtr::TopicPtr(const char* name_) {
	*this = name_;
}

inline
TopicPtr::TopicPtr(const std::string& name_) {
	*this = name_;
}

inline
TopicPtr::TopicPtr(const std::shared_ptr<Topic>& topic_) {
	*this = topic_;
}

inline
TopicPtr& TopicPtr::operator=(const char* name_) {
	if(name_) {
		*this = std::string(name_);
	} else {
		reset();
	}
	return *this;
}

inline
TopicPtr& TopicPtr::operator=(const std::string& name_) {
	if(name_ == TopicPtr::empty) {
		reset();
	} else {
		std::shared_ptr<Topic>::operator=(vnx::get_topic(name_));
	}
	return *this;
}

inline
TopicPtr& TopicPtr::operator=(const std::shared_ptr<Topic>& topic_) {
	std::shared_ptr<Topic>::operator=(topic_);
	return *this;
}

inline
void read(TypeInput& in, TopicPtr& value, const TypeCode* type_code, const uint16_t* code) {
	std::string name;
	read(in, name, type_code, code);
	value = name;
}

inline
void write(TypeOutput& out, const TopicPtr& value, const TypeCode* type_code, const uint16_t* code) {
	if(value) {
		write(out, value->get_name(), type_code, code);
	} else {
		write(out, TopicPtr::empty, type_code, code);
	}
}

inline
void read(std::istream& in, TopicPtr& value) {
	std::string name;
	read(in, name);
	value = name;
}

inline
void write(std::ostream& out, const TopicPtr& value) {
	if(value) {
		write(out, value->get_name());
	} else {
		write(out, nullptr);
	}
}

inline
void accept(Visitor& visitor, const TopicPtr& value) {
	if(value) {
		accept(visitor, value->get_name());
	} else {
		accept(visitor, nullptr);
	}
}


} // vnx

#endif /* INCLUDE_VNX_TOPICPTR_HPP_ */
