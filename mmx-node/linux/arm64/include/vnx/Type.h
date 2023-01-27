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

/** \file vnx/Type.h
 * 
 * Type.h is the top-most header for VNX and is included almost everywhere.
 * 
 */

#ifndef INCLUDE_VNX_TYPE_H_
#define INCLUDE_VNX_TYPE_H_

#include <cstdint>
#include <utility>
#include <memory>
#include <string>
#include <vector>
#include <list>
#include <array>
#include <map>
#include <set>
#include <tuple>
#include <unordered_map>
#include <unordered_set>
#include <iostream>
#include <functional>
#include <stdexcept>
#include <type_traits>

#ifdef _MSC_VER
#include <BaseTsd.h>
typedef SSIZE_T ssize_t;
#endif

#include <vnx_base_export.h>
#include <vnx/optional.h>
#include <vnx/Hash64.h>
#include <vnx/Util.h>

/// Export macro
#define VNX_EXPORT VNX_BASE_EXPORT

/** \brief The maximum size of dynamic containers
 * 
 * Applies for example to CODE_LIST and CODE_MAP.
 */
#define VNX_MAX_SIZE 0xFFFFFFFF

/** \brief The maximum size of static containers
 * 
 * Applies for example to CODE_ARRAY.
 */
#define VNX_MAX_STATIC_SIZE 0xFFFF

/** \brief The maximum size of dynamic code.
 * 
 * Applies to CODE_DYNAMIC.
 * Absolute maximum is 255 to detect different byte order.
 */
#define VNX_MAX_BYTE_CODE_SIZE 32

/** \brief The buffer size for InputBuffer and OutputBuffer.
 * 
 * This is also the maximum combined size of all primitive fields in a struct or class.
 */
#define VNX_BUFFER_SIZE 16384


/// VNX namespace
namespace vnx {

typedef bool bool_t;			///< bool type
typedef float float32_t;		///< 32-bit IEEE float type
typedef double float64_t;		///< 64-bit IEEE float type

class Buffer;
class Memory;
class Visitor;
class Value;
class Object;
class Module;
class Variant;
class Topic;
class TopicPtr;
class TypeCode;
class TypeInput;
class TypeOutput;

/// Register new TypeCode for given type and code hash
const TypeCode* register_type_code(std::shared_ptr<TypeCode> type_code);

/// Register new TypeCode for \p hash
const TypeCode* register_type_code(Hash64 hash, std::shared_ptr<TypeCode> type_code);

/// Register an alias (typedef) for type hash \p type_hash with given \p new_type_hash
const TypeCode* register_type_alias(Hash64 type_hash, Hash64 new_type_hash);

/// Returns pointer to global TypeCode instance for the given type or code hash.
const TypeCode* get_type_code(Hash64 hash);

/// Returns pointer to global TypeCode instance for the given type name.
const TypeCode* get_type_code(const std::string& type_name);

/// Returns all TypeCode instances known to this process
std::vector<const TypeCode*> get_all_type_codes();

/** \brief Validates given code, throws exception in case not valid.
 * 
 * @return Pointer to last code element in given \p code. Can be used to compute size of the code.
 */
const uint16_t* validate_code(const uint16_t* code, const TypeCode* type_code = nullptr, size_t size = size_t(-1), size_t pos = 0);

/// Converts a code to humand readable format
std::string code_to_string(const uint16_t* code, const TypeCode* type_code = nullptr);
std::string code_to_string(const std::vector<uint16_t>& code, const TypeCode* type_code = nullptr);

/** \brief Codes used by the VNX serialization type system.
 * 
 * Codes are limited to 8-bit but are serialized as 16-bit values to detect a different byte order.
 * Alternate (ALT_) codes are from machines with different byte order.
 */
enum {
	CODE_NULL = 0,				///< used for nullptr or empty Variant
	
	CODE_UINT8 = 1,				///< 8-bit unsigned integer
	CODE_UINT16 = 2,			///< 16-bit unsigned integer
	CODE_UINT32 = 3,			///< 32-bit unsigned integer
	CODE_UINT64 = 4,			///< 64-bit unsigned integer
	
	CODE_INT8 = 5,				///< 8-bit signed integer
	CODE_INT16 = 6,				///< 16-bit signed integer
	CODE_INT32 = 7,				///< 32-bit signed integer
	CODE_INT64 = 8,				///< 64-bit signed integer
	
	CODE_FLOAT = 9,				///< 32-bit IEEE floating point
	CODE_DOUBLE = 10,			///< 64-bit IEEE floating point
	
	CODE_ARRAY = 11,			///< static array (std::array), code = {11, <size>, <type code>}
	CODE_LIST = 12,				///< dynamic list (std::list or std::vector), code = {12, <type code>}, header = {UINT32 <size>}
	CODE_MAP = 13,				///< dynamic map (std::map), code = {13, <code offset for value type starting at 13>, <key type code>, <value type code>}, header = {UINT32 <size>}
	
	CODE_TYPE_CODE = 14,		///< code for a vnx::TypeCode
	CODE_TYPE = 15,				///< code for a type to follow, header = {15, UINT64 <code hash>}
	CODE_ANY = 16,				///< code for anything (currently only CODE_TYPE_CODE, CODE_TYPE, CODE_OBJECT, CODE_ANY or CODE_PADDING allowed) 
	CODE_DYNAMIC = 17,			///< code for dynamic code (ie. the actual code is in the data), header = {UINT16 <code size>}
	CODE_PADDING = 18,			///< code for zero padding (used by RecordWriter for example), header = {18, UINT32 <padding size>}
	CODE_STRUCT = 19,			///< code for nested struct, code = {19, <depends index>}
	
	CODE_MATRIX = 21,			///< static ND array, code = {21, <number of dimensions>, <size of dim 0>, <size of dim 1>, ..., <type code>}
	CODE_IMAGE = 22,			///< dynamic ND array, code = {22, <number of dimensions>, <type code>}, header = {UINT32 <size of dim 0>, UINT32 <size of dim 1>, ...}
	CODE_TUPLE = 23,			///< array of different types, code = {23, <size>, <type code offset 0>, ..., <type code>, ...}
	CODE_OBJECT = 24,			///< same as CODE_MAP with {MAP, 4, LIST, INT8, DYNAMIC} but intended to be a dynamically defined type instead of a map
	
	CODE_BOOL = 31,				///< 8-bit unsigned integer (0 = false, otherwise = true)
	CODE_STRING = 32,			///< 8-bit signed integer list (same as {CODE_LIST, CODE_INT8}, but different meaning, used to denote a ASCII/UTF8 string)
	CODE_OPTIONAL = 33,			///< optional value using 8-bit bool to indicate if data is to follow, code = {33, ...}

	CODE_MAGIC = 0x3713,		///< magic number for VNX to detect binary files containing serialized data
	CODE_NONE = 0xFFFF,			///< used to detect binary file vs. text file, irrespective of byte order
	
	CODE_ALT_UINT8 = 0x100,		///< Same as CODE_UINT8 but with alternate byte order
	CODE_ALT_UINT16 = 0x200,	///< Same as CODE_UINT16 but with alternate byte order
	CODE_ALT_UINT32 = 0x300,	///< Same as CODE_UINT32 but with alternate byte order
	CODE_ALT_UINT64 = 0x400,	///< Same as CODE_UINT64 but with alternate byte order
	
	CODE_ALT_INT8 = 0x500,		///< Same as CODE_INT8 but with alternate byte order
	CODE_ALT_INT16 = 0x600,		///< Same as CODE_INT16 but with alternate byte order
	CODE_ALT_INT32 = 0x700,		///< Same as CODE_INT32 but with alternate byte order
	CODE_ALT_INT64 = 0x800,		///< Same as CODE_INT64 but with alternate byte order
	
	CODE_ALT_FLOAT = 0x900,		///< Same as CODE_FLOAT but with alternate byte order
	CODE_ALT_DOUBLE = 0xA00,	///< Same as CODE_DOUBLE but with alternate byte order
	
	CODE_ALT_ARRAY = 0xB00,		///< Same as CODE_ARRAY but with alternate byte order
	CODE_ALT_LIST = 0xC00,		///< Same as CODE_LIST but with alternate byte order
	CODE_ALT_MAP = 0xD00,		///< Same as CODE_MAP but with alternate byte order
	
	CODE_ALT_TYPE_CODE = 0xE00,	///< Same as CODE_TYPE_CODE but with alternate byte order
	CODE_ALT_TYPE = 0xF00,		///< Same as CODE_TYPE but with alternate byte order
	CODE_ALT_ANY = 0x1000,		///< Same as CODE_ANY but with alternate byte order
	CODE_ALT_DYNAMIC = 0x1100,	///< Same as CODE_DYNAMIC but with alternate byte order
	CODE_ALT_PADDING = 0x1200,	///< Same as CODE_PADDING but with alternate byte order
	CODE_ALT_STRUCT = 0x1300,	///< Same as CODE_STRUCT but with alternate byte order
	
	CODE_ALT_MATRIX = 0x1500,	///< Same as CODE_MATRIX but with alternate byte order
	CODE_ALT_IMAGE = 0x1600,	///< Same as CODE_IMAGE but with alternate byte order
	CODE_ALT_TUPLE = 0x1700,	///< Same as CODE_TUPLE but with alternate byte order
	CODE_ALT_OBJECT = 0x1800,	///< Same as CODE_OBJECT but with alternate byte order
	
	CODE_ALT_BOOL = 0x1F00,		///< Same as CODE_BOOL but with alternate byte order
	CODE_ALT_STRING = 0x2000,	///< Same as CODE_CHAR but with alternate byte order
	CODE_ALT_OPTIONAL = 0x2100,	///< Same as CODE_OPTIONAL but with alternate byte order

	CODE_ALT_MAGIC = 0x1337,	///< Same as CODE_MAGIC but with alternate byte order
	
};

/// Returns size of type \p code in bytes
size_t get_value_size(const uint16_t* code, const TypeCode* type_code);

/** \brief Returns true if T is memcpy() equivalent to \p code
 * 
 * Special cases are:
 * - signed and unsigned integers are equivalent
 * - BOOL, UINT8 and ALT_UINT8 are equivalent
 * - INT8 and ALT_INT8 are equivalent
 * - CODE_ARRAY
 * - CODE_TUPLE
 * - CODE_STRUCT
 */
template<typename T>
struct is_equivalent {
	bool operator()(const uint16_t* code, const TypeCode* type_code) {
		return false;
	}
};

/// \private
template<>
struct is_equivalent<uint8_t> {
	inline bool operator()(const uint16_t* code, const TypeCode* type_code) {
		switch(code[0]) {
			case CODE_BOOL:
				if(sizeof(vnx::bool_t) != 1) {
					break;
				}
				/* no break */
			case CODE_UINT8:
			case CODE_INT8:
			case CODE_ALT_BOOL:
			case CODE_ALT_UINT8:
			case CODE_ALT_INT8:
				return true;
		}
		return false;
	}
};

/// \private
template<>
struct is_equivalent<uint16_t> {
	inline bool operator()(const uint16_t* code, const TypeCode* type_code) {
		switch(code[0]) {
			case CODE_UINT16:
			case CODE_INT16:
				return true;
		}
		return false;
	}
};

/// \private
template<>
struct is_equivalent<uint32_t> {
	inline bool operator()(const uint16_t* code, const TypeCode* type_code) {
		switch(code[0]) {
			case CODE_UINT32:
			case CODE_INT32:
				return true;
		}
		return false;
	}
};

/// \private
template<>
struct is_equivalent<uint64_t> {
	inline bool operator()(const uint16_t* code, const TypeCode* type_code) {
		switch(code[0]) {
			case CODE_UINT64:
			case CODE_INT64:
				return true;
		}
		return false;
	}
};

/// \private
template<>
struct is_equivalent<float32_t> {
	inline bool operator()(const uint16_t* code, const TypeCode* type_code) {
		switch(code[0]) {
			case CODE_FLOAT:
				return true;
		}
		return false;
	}
};

/// \private
template<>
struct is_equivalent<float64_t> {
	inline bool operator()(const uint16_t* code, const TypeCode* type_code) {
		switch(code[0]) {
			case CODE_DOUBLE:
				return true;
		}
		return false;
	}
};

/// \private
template<>
struct is_equivalent<int8_t> {
	inline bool operator()(const uint16_t* code, const TypeCode* type_code) {
		return is_equivalent<uint8_t>{}(code, type_code);
	}
};

/// \private
template<>
struct is_equivalent<int16_t> {
	inline bool operator()(const uint16_t* code, const TypeCode* type_code) {
		return is_equivalent<uint16_t>{}(code, type_code);
	}
};

/// \private
template<>
struct is_equivalent<int32_t> {
	inline bool operator()(const uint16_t* code, const TypeCode* type_code) {
		return is_equivalent<uint32_t>{}(code, type_code);
	}
};

/// \private
template<>
struct is_equivalent<int64_t> {
	inline bool operator()(const uint16_t* code, const TypeCode* type_code) {
		return is_equivalent<uint64_t>{}(code, type_code);
	}
};

/// \private
template<>
struct is_equivalent<bool> {
	inline bool operator()(const uint16_t* code, const TypeCode* type_code) {
		if(sizeof(bool) == 1) {
			return is_equivalent<uint8_t>{}(code, type_code);
		}
		return false;
	}
};

/// \private
template<>
struct is_equivalent<char> {
	inline bool operator()(const uint16_t* code, const TypeCode* type_code) {
		if(sizeof(char) == 1) {
			return is_equivalent<int8_t>{}(code, type_code);
		}
		return false;
	}
};

/// \private
template<typename T, size_t N>
struct is_equivalent<std::array<T, N>> {
	bool operator()(const uint16_t* code, const TypeCode* type_code) {
		switch(code[0]) {
			case CODE_ARRAY:
				if(code[1] == N) {
					return is_equivalent<T>{}(code + 2, type_code);
				}
				break;
		}
		return false;
	}
};

/// \private
template<typename K, typename V>
struct is_equivalent<std::pair<K, V>> {
	bool operator()(const uint16_t* code, const TypeCode* type_code) {
		switch(code[0]) {
			case CODE_TUPLE:
				if(code[1] == 2) {
					const uint16_t* key_code = code + code[2];
					const uint16_t* value_code = code + code[3];
					return is_equivalent<K>{}(key_code, type_code) && is_equivalent<V>{}(value_code, type_code)
						&& sizeof(std::pair<K, V>) == (get_value_size(key_code, type_code) + get_value_size(value_code, type_code));
				}
				break;
		}
		return false;
	}
};

template<typename... T>
struct tuple_is_equivalent;

template<>
struct tuple_is_equivalent<> {
	bool operator()(const uint16_t* code, const TypeCode* type_code, size_t index) {
		return true;
	}
};

template<typename F, typename... T>
struct tuple_is_equivalent<F, T...> {
	bool operator()(const uint16_t* code, const TypeCode* type_code, size_t index) {
		const uint16_t* value_code = code + code[2 + index];
		return is_equivalent<F>{}(value_code, type_code) && tuple_is_equivalent<T...>{}(code, type_code, index + 1);
	}
};

/// \private
template<typename... T>
struct is_equivalent<std::tuple<T...>> {
	bool operator()(const uint16_t* code, const TypeCode* type_code) {
		switch(code[0]) {
			case CODE_TUPLE:
				if(code[1] == std::tuple_size<T...>::value) {
					size_t value_size = 0;
					for(size_t i = 0; i < sizeof...(T); i++) {
						value_size += get_value_size(code + code[2 + i], type_code);
					}
					return tuple_is_equivalent<T...>{}(code, type_code, 0) && value_size == sizeof(std::tuple<T...>);
				}
				break;
		}
		return false;
	}
};

/// Same as is_equivalent but based on two codes (does not consider native padding)
bool is_equivalent_code(const uint16_t* code_lhs, const uint16_t* code_rhs);

/// \private
inline void create_dynamic_code(std::vector<uint16_t>& code, const bool& value, bool special = false) { code.push_back(CODE_BOOL); }
/// \private
inline void create_dynamic_code(std::vector<uint16_t>& code, const uint8_t& value, bool special = false) { code.push_back(CODE_UINT8); }
/// \private
inline void create_dynamic_code(std::vector<uint16_t>& code, const uint16_t& value, bool special = false) { code.push_back(CODE_UINT16); }
/// \private
inline void create_dynamic_code(std::vector<uint16_t>& code, const uint32_t& value, bool special = false) { code.push_back(CODE_UINT32); }
/// \private
inline void create_dynamic_code(std::vector<uint16_t>& code, const uint64_t& value, bool special = false) { code.push_back(CODE_UINT64); }
/// \private
inline void create_dynamic_code(std::vector<uint16_t>& code, const char& value, bool special = false) { code.push_back(CODE_INT8); }
/// \private
inline void create_dynamic_code(std::vector<uint16_t>& code, const int8_t& value, bool special = false) { code.push_back(CODE_INT8); }
/// \private
inline void create_dynamic_code(std::vector<uint16_t>& code, const int16_t& value, bool special = false) { code.push_back(CODE_INT16); }
/// \private
inline void create_dynamic_code(std::vector<uint16_t>& code, const int32_t& value, bool special = false) { code.push_back(CODE_INT32); }
/// \private
inline void create_dynamic_code(std::vector<uint16_t>& code, const int64_t& value, bool special = false) { code.push_back(CODE_INT64); }
/// \private
inline void create_dynamic_code(std::vector<uint16_t>& code, const float32_t& value, bool special = false) { code.push_back(CODE_FLOAT); }
/// \private
inline void create_dynamic_code(std::vector<uint16_t>& code, const float64_t& value, bool special = false) { code.push_back(CODE_DOUBLE); }

/// \private
inline void create_dynamic_code(std::vector<uint16_t>& code, const std::string& value, bool special = false) {
	code.push_back(CODE_STRING);
}

template<typename T, size_t N>
void create_dynamic_code(std::vector<uint16_t>& code, const std::array<T, N>& value, bool special = false);

template<typename T>
void create_dynamic_code(std::vector<uint16_t>& code, const std::vector<T>& value, bool special = false);

template<typename T>
void create_dynamic_code(std::vector<uint16_t>& code, const std::list<T>& value, bool special = false);

template<typename T>
void create_dynamic_code(std::vector<uint16_t>& code, const std::set<T>& value, bool special = false);

template<typename T, typename C>
void create_dynamic_code(std::vector<uint16_t>& code, const std::set<T, C>& value, bool special = false);

template<typename T>
void create_dynamic_code(std::vector<uint16_t>& code, const std::unordered_set<T>& value, bool special = false);

template<typename T, typename C>
void create_dynamic_code(std::vector<uint16_t>& code, const std::unordered_set<T, C>& value, bool special = false);

template<typename K, typename V>
void create_dynamic_code(std::vector<uint16_t>& code, const std::map<K, V>& value, bool special = false);

template<typename K, typename V, typename C>
void create_dynamic_code(std::vector<uint16_t>& code, const std::map<K, V, C>& value, bool special = false);

template<typename K, typename V>
void create_dynamic_code(std::vector<uint16_t>& code, const std::unordered_map<K, V>& value, bool special = false);

template<typename K, typename V, typename C>
void create_dynamic_code(std::vector<uint16_t>& code, const std::unordered_map<K, V, C>& value, bool special = false);

template<typename K, typename V>
void create_dynamic_code(std::vector<uint16_t>& code, const std::pair<K, V>& value, bool special = false);

template<typename... T>
void create_dynamic_code(std::vector<uint16_t>& code, const std::tuple<T...>& value, bool special = false);

void create_dynamic_code(std::vector<uint16_t>& code, const std::nullptr_t& value, bool special = false); ///< \private

void create_dynamic_code(std::vector<uint16_t>& code, const Value& value, bool special = false); ///< \private

void create_dynamic_code(std::vector<uint16_t>& code, const TypeCode& value, bool special = false); ///< \private

void create_dynamic_code(std::vector<uint16_t>& code, std::shared_ptr<const Value> value, bool special = false); ///< \private

template<typename T>
void create_dynamic_code(std::vector<uint16_t>& code, const vnx::optional<T>& value, bool special = false); ///< \private


/// Class for a field in a TypeCode.
class TypeField {
public:
	uint16_t version = 2;			///< Version number
	
	std::string name;				///< Field name
	std::string value;				///< Default value in JSON format
	std::vector<uint16_t> code;		///< Field code
	uint16_t data_size = 0;			///< Serialized data size
	bool is_extended = false;		///< Extended means dynamic size
	
	/*
	 * The following fields are computed at runtime. They are not serialized.
	 */
	size_t index = 0;				///< Field index in the original TypeCode::fields array
	size_t offset = 0;				///< Byte offset in the original static data section
	
	int32_t native_index = -1;		///< Field index in the native TypeCode::fields array
	
	void compile(const TypeCode* type_code);
	
	void accept(Visitor& visitor, const TypeCode* type_code = nullptr) const;

	void from_object(const Object& object);

	static std::shared_ptr<TypeField> create() {
		return std::make_shared<TypeField>();
	}

};

/** \brief Represents a VNI struct, class, method or enum.
 * 
 * This is the C++ equivalent to a VNI struct, class, method or enum.
 * It is used to serialize and de-serialize data, as well as for runtime type information.
 */
class TypeCode {
public:
	TypeCode();

	uint16_t version = 2;	///< Version number
	
	Hash64 type_hash;		///< The identity of this type of _any_ version (ie. only depends on the full type name)
	Hash64 code_hash;		///< The identity of this type of a _specific_ version (changes when fields are added/removed or changed)
	
	std::vector<const TypeCode*> parents;		///< The inheritance line (used by instanceof())
	std::vector<const TypeCode*> depends;		///< The types which are used by this type's fields
	const TypeCode* return_type = nullptr;		///< In case of a method type this is the return type

	std::string name;							///< Full unique name of the type (including the namespace)
	std::vector<TypeField> fields;
	std::vector<TypeField> static_fields;		///< ie. constants
	std::vector<const TypeCode*> methods;
	std::map<uint32_t, std::string> enum_map;			///< Map from enum values to their names (only for enum types)
	std::map<std::string, std::string> alias_map;		///< Map for field matching in case of changed field names
	std::string permission;								///< Required permission (only for method types)
	
	bool is_enum = false;			///< If type is an enum
	bool is_class = false;			///< If type is a class (ie. inherits from Value)
	bool is_method = false;			///< If type is a method
	bool is_return = false;			///< If type is a method return type
	bool is_const = false;			///< If method is const
	bool is_async = false;			///< If method is async
	
	/*
	 * The following fields are computed at runtime. They are not serialized.
	 */
	bool is_native = false;								///< If type is native, ie. corresponds to a C++ class
	bool is_matched = false;							///< If type is matched to a native C++ type
	bool is_equivalent = false;							///< If type is memory equivalent to native C++ type (memcpy() is possible)
	const TypeCode* super = nullptr;					///< Direct parent type if any
	const TypeCode* native = nullptr;					///< Native type code if available (equals "this" in case already native)
	size_t native_size = 0;								///< Native struct size in bytes (sizeof(...))
	size_t total_field_size = 0;						///< Total size of all primitive fields [bytes]
	std::string method_name;							///< Method name (without type prefix) in case of a method
	std::vector<TypeField*> field_map;					///< Map from primitive field in data to field in native type
	std::vector<TypeField*> ext_fields;					///< Map from extended field in data to field in native type
	std::map<std::string, uint32_t> inv_enum_map;		///< Inverse enum map, name to value
	
	std::function<std::shared_ptr<Value>(void)> create_value;		///< Function that creates a new value of this type
	
	/**
	 * Must be called after de-serializing a TypeCode.
	 */
	void build();
	
	/**
	 * Returns true if this type is a sub-class (or direct match) of the given type.
	 */
	bool instanceof(Hash64 type_hash_) const;
	
	/**
	 * Returns method name, without the type name, only if is_method == true. Otherwise empty string.
	 */
	std::string get_method_name() const;

	/**
	 * Returns type code of given field, if it has one (struct, class, enum).
	 * Otherwise returns nullptr.
	 */
	const TypeCode* get_field_type_code(const std::string& name) const;

	void accept(Visitor& visitor) const;

	void print(std::ostream& out) const;

	Object to_object() const;

	void from_object(const Object& object);

	static std::shared_ptr<TypeCode> create() {
		return std::make_shared<TypeCode>();
	}

private:
	void compile();
	
	void link();
	
	/**
	 * Will try to match this type to the native type.
	 * If successful we can deserialize this type into a native struct or class.
	 */
	void match(const TypeCode* native_);
	
};

void read(TypeInput& in, TypeField& field, const TypeCode* type_code, const uint16_t* code); ///< \private
void write(TypeOutput& out, const TypeField& field, const TypeCode* type_code, const uint16_t* code); ///< \private

void write(TypeOutput& out, const TypeCode& value); ///< \private
void read(std::istream& in, TypeCode& value);  ///< \private
void write(std::ostream& out, const TypeCode& value);  ///< \private
void accept(Visitor& visitor, const TypeCode& value);  ///< \private


/** \brief Wrapper struct to overloaded type functions.
 * 
 * Used to circumvent a limitation in C++ which is that template functions can only
 * see functions that have been declared before their first instantiation.
 */
template<typename T>
struct type {
	void read(TypeInput& in, T& value, const TypeCode* type_code, const uint16_t* code);
	void write(TypeOutput& out, const T& value, const TypeCode* type_code, const uint16_t* code);
	void read(std::istream& in, T& value);
	void write(std::ostream& out, const T& value);
	void accept(Visitor& visitor, const T& value);
	void create_dynamic_code(std::vector<uint16_t>& code, const T& value = T(), bool special = false);
	const TypeCode* get_type_code();
};

template<typename T>
const TypeCode* type<T>::get_type_code() {
	return nullptr;
}

template<typename T>
void type<T>::create_dynamic_code(std::vector<uint16_t>& code, const T& value, bool special) {
	vnx::create_dynamic_code(code, value, special);
}

/// \private
template<typename T, size_t N>
void create_dynamic_code(std::vector<uint16_t>& code, const std::array<T, N>& value, bool special) {
	if(N > VNX_MAX_STATIC_SIZE) {
		throw std::invalid_argument("create_dynamic_code(std::array<T, N>): N > VNX_MAX_STATIC_SIZE");
	}
	code.push_back(CODE_ARRAY);
	code.push_back(N);
	vnx::type<T>().create_dynamic_code(code);
}

/// \private
template<typename T>
void create_dynamic_code(std::vector<uint16_t>& code, const std::vector<T>& value, bool special) {
	code.push_back(CODE_LIST);
	vnx::type<T>().create_dynamic_code(code);
}

/// \private
template<typename T>
void create_dynamic_code(std::vector<uint16_t>& code, const std::list<T>& value, bool special) {
	create_dynamic_code(code, std::vector<T>());
}

/// \private
template<typename T>
void create_dynamic_code(std::vector<uint16_t>& code, const std::set<T>& value, bool special) {
	create_dynamic_code(code, std::vector<T>());
}

/// \private
template<typename T, typename C>
void create_dynamic_code(std::vector<uint16_t>& code, const std::set<T, C>& value, bool special) {
	create_dynamic_code(code, std::vector<T>());
}

/// \private
template<typename T>
void create_dynamic_code(std::vector<uint16_t>& code, const std::unordered_set<T>& value, bool special) {
	create_dynamic_code(code, std::vector<T>());
}

/// \private
template<typename T, typename C>
void create_dynamic_code(std::vector<uint16_t>& code, const std::unordered_set<T, C>& value, bool special) {
	create_dynamic_code(code, std::vector<T>());
}

/// \private
template<typename K, typename V>
void create_dynamic_code(std::vector<uint16_t>& code, const std::map<K, V>& value, bool special) {
	const size_t begin = code.size();
	code.push_back(CODE_MAP);
	code.push_back(0);
	vnx::type<K>().create_dynamic_code(code);
	code[begin + 1] = uint16_t(code.size() - begin);
	vnx::type<V>().create_dynamic_code(code);
}

/// \private
template<typename K, typename V, typename C>
void create_dynamic_code(std::vector<uint16_t>& code, const std::map<K, V, C>& value, bool special) {
	create_dynamic_code(code, std::map<K, V>());
}

/// \private
template<typename K, typename V>
void create_dynamic_code(std::vector<uint16_t>& code, const std::unordered_map<K, V>& value, bool special) {
	create_dynamic_code(code, std::map<K, V>());
}

/// \private
template<typename K, typename V, typename C>
void create_dynamic_code(std::vector<uint16_t>& code, const std::unordered_map<K, V>& value, bool special) {
	create_dynamic_code(code, std::map<K, V>());
}

/// \private
template<typename K, typename V>
void create_dynamic_code(std::vector<uint16_t>& code, const std::pair<K, V>& value, bool special) {
	const size_t begin = code.size();
	code.push_back(CODE_TUPLE);
	code.push_back(2);
	code.push_back(0);
	code.push_back(0);
	code[begin + 2] = uint16_t(code.size() - begin);
	vnx::type<K>().create_dynamic_code(code);
	code[begin + 3] = uint16_t(code.size() - begin);
	vnx::type<V>().create_dynamic_code(code);
}

template<typename... T>
typename std::enable_if<sizeof...(T)==0, void>::type
create_dynamic_code_tuple(std::vector<uint16_t>& code, size_t begin, size_t index) {}

template<typename F, typename... T>
void create_dynamic_code_tuple(std::vector<uint16_t>& code, size_t begin, size_t index) {
	code[begin + index] = uint16_t(code.size() - begin);
	vnx::type<F>().create_dynamic_code(code);
	create_dynamic_code_tuple<T...>(code, begin, index + 1);
}

/// \private
template<typename... T>
void create_dynamic_code(std::vector<uint16_t>& code, const std::tuple<T...>& value, bool special) {
	const size_t begin = code.size();
	code.push_back(CODE_TUPLE);
	code.push_back(sizeof...(T));
	for(size_t i = 0; i < sizeof...(T); i++){
		code.push_back(0);
	}
	create_dynamic_code_tuple<T...>(code, begin, 2);
}

/// \private
inline void create_dynamic_code(std::vector<uint16_t>& code, const std::nullptr_t& value, bool special) {
	code.push_back(CODE_NULL);
}

/// \private
inline void create_dynamic_code(std::vector<uint16_t>& code, const Value& value, bool special) {
	code.push_back(CODE_OBJECT);
}

/// \private
inline void create_dynamic_code(std::vector<uint16_t>& code, const TypeCode& value, bool special) {
	code.push_back(CODE_TYPE_CODE);
}

/// \private
inline void create_dynamic_code(std::vector<uint16_t>& code, std::shared_ptr<const Value> value, bool special) {
	if(!special || value) {
		code.push_back(CODE_OBJECT);
	} else {
		code.push_back(CODE_NULL);
	}
}

/// \private
template<typename T>
void create_dynamic_code(std::vector<uint16_t>& code, const vnx::optional<T>& value, bool special) {
	if(!special || value) {
		if(!special) {
			code.push_back(CODE_OPTIONAL);
		}
		vnx::type<T>().create_dynamic_code(code);
	} else {
		code.push_back(CODE_NULL);
	}
}

/// Convert value to a JSON string
template<typename T>
std::string to_string(const T& value);

/// Convert value to a plain string
template<typename T>
std::string to_string_value(const T& value);

/// Convert value to a plain string (with full type name if applicable)
template<typename T>
std::string to_string_value_full(const T& value);

/// Convert a JSON value (string with quotes) to a specific type.
template<typename T>
void from_string(const std::string& str, T& value);

/// Convert a string value (without quotes) to a specific type.
template<typename T>
void from_string_value(const std::string& str, T& value);


} // vnx

#endif /* INCLUDE_VNX_TYPE_H_ */
