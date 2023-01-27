/*
 * ToObject.h
 *
 *  Created on: Dec 16, 2020
 *      Author: mad
 */

#ifndef INCLUDE_VNX_TOOBJECT_H_
#define INCLUDE_VNX_TOOBJECT_H_

#include <vnx/Visitor.h>
#include <vnx/Object.h>


namespace vnx {

/// A Visitor that generates an Object if possible.
class ToObject : public Visitor {
public:
	ToObject();

	/// Returns the value (could be an object, array or scalar)
	Variant get_value() const;

	/// Returns the object (empty if none was seen)
	Object get_object() const;

	void visit_null() override;
	void visit(const bool& value) override;
	void visit(const uint8_t& value) override;
	void visit(const uint16_t& value) override;
	void visit(const uint32_t& value) override;
	void visit(const uint64_t& value) override;
	void visit(const int8_t& value) override;
	void visit(const int16_t& value) override;
	void visit(const int32_t& value) override;
	void visit(const int64_t& value) override;
	void visit(const float32_t& value) override;
	void visit(const float64_t& value) override;
	void visit(const std::string& value) override;

	void list_begin(size_t size) override;
	void list_element(size_t index) override;
	void list_end(size_t size) override;

	void type_begin(size_t num_fields, const std::string* type_name) override;
	void type_field(const std::string& field, size_t index) override;
	void type_end(size_t num_fields, const std::string* type_name) override;

private:
	template<typename T>
	void set_value(const T& value);

private:
	struct entry_t {
		Variant value;
		Object object;
		std::string field;
		std::vector<Variant> array;

		entry_t() = default;
		entry_t(size_t size) : array(size) {}
		entry_t(const std::string& name) : field(name) {}
	};

	std::vector<entry_t> stack;

};


} // vnx

#endif /* INCLUDE_VNX_TOOBJECT_H_ */
