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

#ifndef INCLUDE_VNX_OBJECT_H
#define INCLUDE_VNX_OBJECT_H

#include <vnx/package.hxx>
#include <vnx/Value.h>
#include <vnx/Variant.h>

#include <initializer_list>


namespace vnx {

/** \brief A dynamically typed object consisting of a map of Variant fields.
 * 
 * Object can be used to hold dynamically typed data when creating a VNI definition
 * and generating code is not possible or when performance is not an issue.
 */
class Object : public Value {
public:
	std::map<std::string, Variant> field;		///< Data fields (name => value)
	
	Object() = default;
	Object(const Object&) = default;
	Object(std::initializer_list<std::pair<const std::string, Variant>> entries);

	Object& operator=(const Object&) = default;

	/// Clears the object and sets the given fields
	Object& operator=(std::initializer_list<std::pair<const std::string, Variant>> entries);

	/// Returns reference to field value
	Variant& operator[](const std::string& name) {
		return field[name];
	}
	
	/// Returns reference to field value
	const Variant& operator[](const std::string& name) const {
		auto iter = field.find(name);
		if(iter != field.end()) {
			return iter->second;
		}
		return Variant::static_empty();
	}
	
	bool operator==(const Object& other) const {
		return field == other.field;
	}
	
	bool operator!=(const Object& other) const {
		return field != other.field;
	}
	
	/// Sets the given fields, without clearing the object first.
	Object& insert(std::initializer_list<std::pair<const std::string, Variant>> entries);

	bool empty() const {
		return field.empty();
	}
	
	void erase(const std::string& name) {
		field.erase(name);
	}

	void clear() {
		field.clear();
	}
	
	Hash64 get_type_hash() const override {
		return Hash64(get_type_name());
	}
	
	/// Returns type name if "__type" field is available, otherwise "vnx.Object"
	std::string get_type_name() const override;
	
	/// Returns type code if "__type" field is available
	const TypeCode* get_type_code() const override;
	
	static std::shared_ptr<Object> create() {
		return std::make_shared<Object>();
	}
	
	std::shared_ptr<Value> clone() const override {
		return std::make_shared<Object>(*this);
	}
	
	void read(TypeInput& in, const TypeCode* type_code, const uint16_t* code) override {
		vnx::read(in, *this, type_code, code);
	}
	
	void write(TypeOutput& out, const TypeCode* type_code, const uint16_t* code) const override {
		vnx::write(out, *this, type_code, code);
	}
	
	void read(std::istream& in) override {
		vnx::read(in, *this);
	}
	
	void write(std::ostream& out) const override {
		vnx::write(out, *this);
	}
	
	void accept(Visitor& visitor) const override {
		vnx::accept(visitor, *this);
	}
	
	Object to_object() const override {
		return *this;
	}
	
	void from_object(const Object& object) override {
		*this = object;
	}
	
	Variant get_field(const std::string& name) const override {
		return (*this)[name];
	}

	void set_field(const std::string& name, const Variant& value) override {
		field[name] = value;
	}

	/** \brief Returns polymorphic Value (using "__type" field)
	 *
	 * @param type_map Optional type map for mapping a received type (the key) to another type.
	 */
	std::shared_ptr<Value> to_value(
			const std::map<std::string, std::string>& type_map = std::map<std::string, std::string>()) const;

	/** \brief Returns polymorphic Value (using "__type" field) or specified type T.
	 *
	 * Same as to_value() in case "__type" field is present, plus a dynamic cast.
	 * Otherwise a value of type T is being created from the object using from_object().
	 */
	template<typename T>
	std::shared_ptr<T> as_value() const;

	/// Returns true if this object is an instance of T
	template<typename T>
	bool instanceof() const;

	friend std::ostream& operator<<(std::ostream& out, const Object& value);
	
	friend std::istream& operator>>(std::istream& in, Object& value);
	
	static const uint16_t CODE[];
	static const uint16_t ALT_CODE[];
	
};


} // vnx

#endif // INCLUDE_VNX_OBJECT_H
