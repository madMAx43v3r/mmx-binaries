/*
 * JSON.h
 *
 *  Created on: Dec 3, 2020
 *      Author: mad
 */

#ifndef VNX_INCLUDE_VNX_JSON_H_
#define VNX_INCLUDE_VNX_JSON_H_

#include <vnx/Type.h>
#include <vnx/Object.h>
#include <vnx/Variant.h>


namespace vnx {

class JSON_Value {
public:
	virtual ~JSON_Value() {}

	virtual Variant to_variant() const = 0;

	virtual Object to_object() const = 0;
	
	virtual std::string to_string() const = 0;
	
	void write(std::ostream& out) const {
		out << to_string();
	}

};

void write(std::ostream& out, const JSON_Value& value);


class JSON_Variant : public JSON_Value {
public:
	JSON_Variant() = default;

	JSON_Variant(const Variant& var) : data(var) {}

	Variant to_variant() const override {
		return data;
	}

	Object to_object() const override {
		return data.to_object();
	}
	
	std::string to_string() const override {
		return data.to_string();
	}

private:
	Variant data;

};


class JSON_String : public JSON_Value {
public:
	JSON_String() = default;

	JSON_String(const std::string& str) : data(str) {}
	
	const std::string& get_value() const {
		return data;
	}

	Variant to_variant() const override {
		return Variant(data);
	}

	Object to_object() const override {
		return Object();
	}
	
	std::string to_string() const override;

private:
	std::string data;

};


class JSON_Array : public JSON_Value {
public:
	JSON_Array() = default;

	void push_back(std::shared_ptr<JSON_Value> value) {
		values.push_back(value);
	}

	const std::vector<std::shared_ptr<JSON_Value>>& get_values() const {
		return values;
	}

	Variant to_variant() const override;

	Object to_object() const override {
		return Object();
	}
	
	std::string to_string() const override;

private:
	std::vector<std::shared_ptr<JSON_Value>> values;

};


class JSON_Object : public JSON_Value {
public:
	JSON_Object() = default;

	std::shared_ptr<JSON_Value>& operator[](const std::string& name) {
		return field[name];
	}

	const std::map<std::string, std::shared_ptr<JSON_Value>>& get_fields() const {
		return field;
	}

	Variant to_variant() const override {
		return Variant(to_object());
	}

	Object to_object() const override;
	
	std::string to_string() const override;

private:
	std::map<std::string, std::shared_ptr<JSON_Value>> field;

};


/** \brief Reads a JSON value from the stream.
 *
 * @param until_eof Keep reading until EOF, do not stop on special chars like ":,]}".
 * @param until_space Stop when encountering whitespace and a value was read.
 *
 * Returns nullptr in case there is no input.
 */
std::shared_ptr<JSON_Value> read_json(std::istream& in, bool until_eof = false, bool until_space = false);


} // vnx

#endif /* VNX_INCLUDE_VNX_JSON_H_ */
