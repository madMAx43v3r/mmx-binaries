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

#ifndef INCLUDE_VNX_STRUCT_H
#define INCLUDE_VNX_STRUCT_H

#include <vnx/Value.h>
#include <vnx/Object.h>


namespace vnx {

/** \brief Wrapper class to hold a struct as a Value.
 * 
 * Used to pass structs as a Value.
 */
template<typename T>
class Struct : public Value {
public:
	T data;
	
	vnx::Hash64 get_type_hash() const override {
		return data.get_type_hash();
	}
	
	std::string get_type_name() const override {
		return data.get_type_name();
	}
	
	const TypeCode* get_type_code() const override {
		return data.get_type_code();
	}
	
	static std::shared_ptr<Struct<T>> create() {
		return std::make_shared<Struct<T>>();
	}
	
	std::shared_ptr<Value> clone() const override {
		return std::make_shared<Struct<T>>(*this);
	}
	
	void read(TypeInput& in, const TypeCode* type_code, const uint16_t* code) override {
		data.read(in, type_code, code);
	}
	
	void write(TypeOutput& out, const TypeCode* type_code, const uint16_t* code) const override {
		data.write(out, type_code, code);
	}
	
	void read(std::istream& in) override {
		data.read(in);
	}
	
	void write(std::ostream& out) const override {
		data.write(out);
	}
	
	void accept(Visitor& visitor) const override {
		data.accept(visitor);
	}
	
	Object to_object() const override {
		return data.to_object();
	}
	
	void from_object(const Object& object) override {
		data.from_object(object);
	}
	
	Variant get_field(const std::string& name) const override {
		return data.get_field(name);
	}

	void set_field(const std::string& name, const Variant& value) override {
		data.set_field(name, value);
	}

	friend std::ostream& operator<<(std::ostream& _out, const Struct& _value) {
		_out << _value.data;
		return _out;
	}
	
	friend std::istream& operator>>(std::istream& _in, Struct& _value) {
		_in >> _value.data;
		return _in;
	}
	
};


} // vnx

#endif // INCLUDE_VNX_STRUCT_H
