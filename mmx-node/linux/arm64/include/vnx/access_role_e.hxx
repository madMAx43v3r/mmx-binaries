
// AUTO GENERATED by vnxcppcodegen

#ifndef INCLUDE_vnx_access_role_e_HXX_
#define INCLUDE_vnx_access_role_e_HXX_

#include <vnx/Type.h>
#include <vnx/package.hxx>


namespace vnx {

struct VNX_EXPORT access_role_e {
	
	enum enum_t : uint32_t {
		ADMIN = 2100538712l,
		DEFAULT = 490312487l,
		INSTALLER = 455339260l,
		OBSERVER = 1844813261l,
		UNAUTHENTICATED = 2279186489l,
		USER = 137129985l,
		VIEWER = 2077878907l,
	};
	
	::vnx::access_role_e::enum_t value = ::vnx::access_role_e::enum_t(0);
	
	static const vnx::Hash64 VNX_TYPE_HASH;
	static const vnx::Hash64 VNX_CODE_HASH;
	
	static constexpr uint64_t VNX_TYPE_ID = 0x915660c6100c6c6eull;
	
	access_role_e() {}
	
	access_role_e(const enum_t& _value) { value = _value; }
	access_role_e(const ::vnx::access_role_e& _other) { value = _other.value; }
	
	vnx::Hash64 get_type_hash() const;
	std::string get_type_name() const;
	const vnx::TypeCode* get_type_code() const;
	
	operator enum_t() const { return value; }
	access_role_e& operator=(const enum_t& _value) { value = _value; return *this; }
	access_role_e& operator=(const ::vnx::access_role_e& _other) { value = _other.value; return *this; }
	
	static std::shared_ptr<access_role_e> create();
	std::shared_ptr<access_role_e> clone() const;
	
	void read(vnx::TypeInput& _in, const vnx::TypeCode* _type_code, const uint16_t* _code);
	void write(vnx::TypeOutput& _out, const vnx::TypeCode* _type_code, const uint16_t* _code) const;
	
	void read(std::istream& _in);
	void write(std::ostream& _out) const;
	
	template<typename T>
	void accept_generic(T& _visitor) const;
	void accept(vnx::Visitor& _visitor) const;
	
	vnx::bool_t is_valid() const;
	std::string to_string() const;
	std::string to_string_value() const;
	std::string to_string_value_full() const;
	
	void from_string(const std::string& str);
	void from_string_value(const std::string& name);
	
	vnx::Object to_object() const;
	void from_object(const vnx::Object& object);
	
	vnx::Variant get_field(const std::string& name) const;
	void set_field(const std::string& name, const vnx::Variant& value);
	
	friend std::ostream& operator<<(std::ostream& _out, const access_role_e& _value);
	friend std::istream& operator>>(std::istream& _in, access_role_e& _value);
	
	static const vnx::TypeCode* static_get_type_code();
	static std::shared_ptr<vnx::TypeCode> static_create_type_code();
	
};

template<typename T>
void access_role_e::accept_generic(T& _visitor) const {
	_visitor.template type_begin<access_role_e>(1);
	_visitor.type_field("value", 0); _visitor.accept(value);
	_visitor.template type_end<access_role_e>(1);
}


} // namespace vnx


namespace vnx {

void read(TypeInput& in, ::vnx::access_role_e::enum_t& value, const TypeCode* type_code, const uint16_t* code); ///< \private

void write(TypeOutput& out, const ::vnx::access_role_e::enum_t& value, const TypeCode* type_code, const uint16_t* code); ///< \private

template<>
std::string to_string(const ::vnx::access_role_e& _value); ///< \private

template<>
std::string to_string_value(const ::vnx::access_role_e& _value); ///< \private

template<>
std::string to_string_value_full(const ::vnx::access_role_e& _value); ///< \private

template<>
std::string to_string(const ::vnx::access_role_e::enum_t& _value); ///< \private

template<>
std::string to_string_value(const ::vnx::access_role_e::enum_t& _value); ///< \private

template<>
std::string to_string_value_full(const ::vnx::access_role_e::enum_t& _value); ///< \private

template<>
struct is_equivalent<::vnx::access_role_e> {
	bool operator()(const uint16_t* code, const TypeCode* type_code);
};

} // vnx

#endif // INCLUDE_vnx_access_role_e_HXX_