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

#ifndef INCLUDE_VNX_TEST_TEST_H
#define INCLUDE_VNX_TEST_TEST_H

#include <vnx/Input.h>
#include <vnx/Output.h>
#include <vnx/Variant.h>

#include <vnx_base_export.h>

VNX_BASE_EXPORT extern std::string _vnx_test_name;

#define VNX_TEST_BEGIN(name) {\
	const std::string _vnx_test_domain = _vnx_test_name;\
	const std::string _vnx_test_name = _vnx_test_domain + "." + name;\
	try {

#define VNX_TEST_END() \
		::vnx::test::pass(_vnx_test_name);\
	} catch(const std::exception& ex) {\
		::vnx::test::fail(_vnx_test_name, ex);\
	}}


namespace vnx {
namespace test {

template<typename A, typename B>
void expect(const A& value, const B& expected) {
	if(!(value == expected)) {
		throw std::logic_error("expected " + vnx::to_string(expected) + " but got " + vnx::to_string(value));
	}
}

template<typename B>
void expect(const vnx::Variant& value, const B& expected) {
	expect<B, B>(value.to<B>(), expected);
}

void init(const std::string& test_domain);

void pass(const std::string& test_name);

void fail(const std::string& test_name, const std::exception& exception);

void exit();

	
} // test
} // vnx

#endif // INCLUDE_VNX_TEST_TEST_H
