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

#ifndef INCLUDE_VNX_INPUT_HPP_
#define INCLUDE_VNX_INPUT_HPP_

#include <vnx/JSON.h>
#include <vnx/Input.h>
#include <vnx/Object.h>
#include <vnx/Variant.h>


namespace vnx {

template<typename T>
void from_string(const std::string& str, std::shared_ptr<T>& value);

template<typename T>
void from_string(const std::string& str, std::shared_ptr<const T>& value);

/** \brief Reads a std::pair from the input stream.
 *
 * Compatible with CODE_TUPLE, CODE_LIST and CODE_DYNAMIC.
 */
template<typename K, typename V>
void read(TypeInput& in, std::pair<K, V>& value, const TypeCode* type_code, const uint16_t* code) {
	switch(code[0]) {
		case CODE_TUPLE:
		case CODE_ALT_TUPLE: {
			const int N = code[0] == CODE_TUPLE ? code[1] : flip_bytes(code[1]);
			if(N >= 1) {
				vnx::type<K>().read(in, value.first, type_code, code + code[2]);
			} else {
				value.first = K();
			}
			if(N >= 2) {
				vnx::type<V>().read(in, value.second, type_code, code + code[3]);
			} else {
				value.second = V();
			}
			for(int i = 2; i < N; ++i) {
				skip(in, type_code, code + code[2 + i]);
			}
			return;
		}
		case CODE_LIST:
		case CODE_ALT_LIST: {
			std::vector<Variant> list;
			read(in, list, type_code, code);
			if(list.size() >= 2) {
				list[0].to(value.first);
				list[1].to(value.second);
			}
			return;
		}
		case CODE_DYNAMIC:
		case CODE_ALT_DYNAMIC:
			read_dynamic(in, value);
			return;
	}
	value = std::pair<K, V>();
	skip(in, type_code, code);
}

template<size_t index, typename... T>
typename std::enable_if<index == sizeof...(T), void>::type
read_tuple(TypeInput& in, std::tuple<T...>& tuple, unsigned int N, const TypeCode* type_code, const uint16_t* code) {}

template<size_t index, typename... T>
typename std::enable_if<index < sizeof...(T), void>::type
read_tuple(TypeInput& in, std::tuple<T...>& tuple, unsigned int N, const TypeCode* type_code, const uint16_t* code) {
	const uint16_t* value_code = code + code[2 + index];
	if(index < N) {
		vnx::type<typename std::tuple_element<index, std::tuple<T...>>::type>().read(
				in, std::get<index>(tuple), type_code, value_code);
	} else {
		std::get<index>(tuple) = typename std::tuple_element<index, std::tuple<T...>>::type();
		skip(in, type_code, value_code);
	}
	read_tuple<index+1>(in, tuple, N, type_code, code);
}

/** \brief Reads a std::tuple from the input stream.
 *
 * Compatible with CODE_TUPLE and CODE_DYNAMIC.
 */
template<typename... T>
void read(TypeInput& in, std::tuple<T...>& value, const TypeCode* type_code, const uint16_t* code) {
	switch(code[0]) {
		case CODE_TUPLE:
		case CODE_ALT_TUPLE: {
			const unsigned int N = code[0] == CODE_TUPLE ? code[1] : flip_bytes(code[1]);
			read_tuple<0>(in, value, N, type_code, code);
			return;
		}
		case CODE_DYNAMIC:
		case CODE_ALT_DYNAMIC:
			read_dynamic(in, value);
			return;
	}
	value = std::tuple<T...>();
	skip(in, type_code, code);
}

/** \brief Reads a dynamically allocated map (AssociativeContainer) from the input stream.
 *
 * Compatible with CODE_MAP, CODE_LIST, CODE_OBJECT and CODE_DYNAMIC.
 */
template<typename T>
void read_map(TypeInput& in, T& map, const TypeCode* type_code, const uint16_t* code) {
	map.clear();
	uint32_t size = 0;
	const uint16_t* key_code = code + 2;
	const uint16_t* value_code = nullptr;
	switch(code[0]) {
		case CODE_MAP:
			read(in, size);
			value_code = code + code[1];
			break;
		case CODE_ALT_MAP:
			read(in, size);
			size = flip_bytes(size);
			value_code = code + flip_bytes(code[1]);
			break;
		case CODE_LIST:
		case CODE_ALT_LIST: {
			std::vector<std::pair<typename T::key_type, typename T::mapped_type>> list;
			read(in, list, type_code, code);
			for(auto& entry : list) {
				map.emplace(std::move(entry));
			}
			return;
		}
		case CODE_OBJECT:
		case CODE_ALT_OBJECT: {
			Object tmp;
			read(in, tmp, type_code, code);
			for(const auto& entry : tmp.field) {
				typename T::key_type key;
				from_string_value(entry.first, key);
				entry.second.to(map[key]);
			}
			return;
		}
		case CODE_DYNAMIC:
		case CODE_ALT_DYNAMIC: {
			read_dynamic(in, map);
			return;
		}
		default:
			skip(in, type_code, code);
			return;
	}
	for(size_t i = 0; i < size; ++i) {
		typename T::key_type key;
		vnx::type<typename T::key_type>().read(in, key, type_code, key_code);
		typename T::mapped_type& value = map[key];
		vnx::type<typename T::mapped_type>().read(in, value, type_code, value_code);
	}
}

/** \brief Reads a Value from the input stream.
 *
 * Same as read(TypeInput& in) but not at top level, ie. this function expects a code.
 * In case of incompatible data a nullptr will be returned.
 * Compatible with CODE_ANY, CODE_DYNAMIC, CODE_OBJECT and CODE_OPTIONAL.
 */
template<typename T>
void read(TypeInput& in, std::shared_ptr<T>& value, const TypeCode* type_code, const uint16_t* code) {
	if(!code) {
		throw std::logic_error("!code");
	}
	switch(code[0]) {
		case CODE_ANY:
		case CODE_ALT_ANY:
			value = std::dynamic_pointer_cast<T>(read(in));
			return;
		case CODE_DYNAMIC:
		case CODE_ALT_DYNAMIC:
			read_dynamic(in, value);
			return;
		case CODE_OBJECT:
		case CODE_ALT_OBJECT: {
			Object obj;
			read(in, obj, type_code, code);
			value = obj.as_value<T>();
			return;
		}
		case CODE_OPTIONAL:
		case CODE_ALT_OPTIONAL: {
			bool flag = false;
			vnx::read(in, flag);
			if(flag) {
				read(in, value, type_code, code + 1);
			} else {
				value = nullptr;
			}
			return;
		}
	}
	value = nullptr;
	skip(in, type_code, code);
}

/** \brief Reads something optional from the input stream.
 *
 * In case of incompatible data a default initialized value will be returned.
 * Compatible with CODE_OPTIONAL.
 */
template<typename T>
void read(TypeInput& in, vnx::optional<T>& value, const TypeCode* type_code, const uint16_t* code) {
	if(!code) {
		throw std::logic_error("!code");
	}
	switch(code[0]) {
		case CODE_NULL:
			value = nullptr;
			return;
		case CODE_OPTIONAL:
		case CODE_ALT_OPTIONAL: {
			bool flag = false;
			vnx::read(in, flag);
			if(flag) {
				read(in, value, type_code, code + 1);
			} else {
				value = nullptr;
			}
			return;
		}
	}
	std::unique_ptr<T> tmp(new T());
	vnx::type<T>().read(in, *tmp, type_code, code);
	value.reset(tmp.release());
}

template<typename T>
void read(std::istream& in, vnx::optional<T>& value);

template<typename T, size_t N>
void read(std::istream& in, std::array<T, N>& vector);

template<typename T>
void read(std::istream& in, std::vector<T>& vector);

template<typename T>
void read(std::istream& in, std::list<T>& list);

template<typename K, typename V>
void read(std::istream& in, std::pair<K, V>& pair);

template<typename... T>
void read(std::istream& in, std::tuple<T...>& tuple);

template<typename T>
void read(std::istream& in, std::set<T>& set);

template<typename K, typename V>
void read(std::istream& in, std::map<K, V>& map);

/** \brief Reads a static array from the JSON stream
 *
 * Example: [1, 2, 3] \n
 * If the input array is smaller than \p array the left-over elements are default initialized.
 */
template<typename T, size_t N>
void read(std::istream& in, std::array<T, N>& vector) {
	auto json = read_json(in);
	if(auto array = std::dynamic_pointer_cast<JSON_Array>(json)) {
		size_t i = 0;
		for(auto entry : array->get_values()) {
			if(i < N) {
				entry->to_variant().to(vector[i++]);
			} else {
				break;
			}
		}
	} else if(json && N > 0) {
		json->to_variant().to(vector[0]);
	}
}

/** \brief Reads a dynamic array from the JSON stream
 *
 * Example: [1, 2, 3]
 */
template<typename T>
void read(std::istream& in, std::vector<T>& vector) {
	auto json = read_json(in);
	if(auto array = std::dynamic_pointer_cast<JSON_Array>(json)) {
		for(auto entry : array->get_values()) {
			vector.emplace_back(entry->to_variant().to<T>());
		}
	} else if(json) {
		vector.emplace_back(json->to_variant().to<T>());
	}
}

/** \brief Reads a dynamic list from the JSON stream
 *
 * Example: [1, 2, 3]
 */
template<typename T>
void read(std::istream& in, std::list<T>& list) {
	std::vector<T> tmp;
	read(in, tmp);
	for(auto& value : tmp) {
		list.emplace_back(std::move(value));
	}
}

/** \brief Reads a pair from the JSON stream
 *
 * Example: ["key", "value"]
 */
template<typename K, typename V>
void read(std::istream& in, std::pair<K, V>& pair) {
	auto json = read_json(in);
	if(auto array = std::dynamic_pointer_cast<JSON_Array>(json)) {
		const auto& entries = array->get_values();
		if(entries.size() >= 1) {
			entries[0]->to_variant().to(pair.first);
		} else {
			pair.first = K();
		}
		if(entries.size() >= 2) {
			entries[1]->to_variant().to(pair.second);
		} else {
			pair.second = V();
		}
	} else {
		pair = std::pair<K, V>();
	}
}

template<size_t index, typename... T>
typename std::enable_if<index == sizeof...(T), void>::type
convert_to_tuple(std::tuple<T...>& tuple, const std::vector<std::shared_ptr<JSON_Value>>& data) {
}

template<size_t index, typename... T>
typename std::enable_if<index < sizeof...(T), void>::type
convert_to_tuple(std::tuple<T...>& tuple, const std::vector<std::shared_ptr<JSON_Value>>& data) {
	if(index < data.size()) {
		data[index]->to_variant().to(std::get<index>(tuple));
		convert_to_tuple<index+1>(tuple, data);
	} else {
		std::get<index>(tuple) = typename std::tuple_element<index, std::tuple<T...>>::type();
	}
}

/** \brief Reads a tuple from the JSON stream
 *
 * Example: [1, "value", 11.3]
 */
template<typename... T>
void read(std::istream& in, std::tuple<T...>& tuple) {
	auto json = read_json(in);
	if(auto array = std::dynamic_pointer_cast<JSON_Array>(json)) {
		const auto& entries = array->get_values();
		convert_to_tuple<0>(tuple, entries);
	} else {
		tuple = std::tuple<T...>();
	}
}

/** \brief Reads a set from the JSON stream
 *
 * Example: [1, 2, 3]
 */
template<typename T>
void read(std::istream& in, std::set<T>& set) {
	std::vector<T> tmp;
	read(in, tmp);
	for(auto& value : tmp) {
		set.emplace(std::move(value));
	}
}

/** \brief Reads a map from the JSON stream
 *
 * Example: [[1, 1.234], [123, 5.678]] \n
 * Example: {"foo": 1, "bar": 123}
 */
template<typename K, typename V>
void read(std::istream& in, std::map<K, V>& map) {
	read(in).to(map);
}

template<typename T>
void read(std::istream& in, vnx::optional<T>& value) {
	const auto tmp = read(in);
	if(!tmp.is_null()) {
		value = tmp.to<T>();
	}
}

/** \brief Reads a matrix from the JSON stream.
 *
 * Example: [1, 2, 3, 4]	// column vector \n
 * Example: {"size": [2, 2], "data": [1, 2, 3, 4]}		// general N-by-M matrix \n
 */
template<typename T, size_t N>
void read_matrix(std::istream& in, T* data, const std::array<size_t, N>& size) {
	size_t total_size = 1;
	for(size_t i = 0; i < N; ++i) {
		total_size *= size[i];
	}
	bool is_valid = false;
	std::vector<T> values;
	const Variant tmp = read(in);
	if(tmp.is_array()) {
		is_valid = true;
		tmp.to(values);
	} else if(tmp.is_object()) {
		const auto object = tmp.to_object();
		for(const auto& entry : object.field) {
			if(entry.first == "size") {
				const auto size_ = entry.second.to<std::vector<int>>();
				if(size_.size() == N) {
					is_valid = true;
					for(size_t i = 0; i < N; ++i) {
						if(size_[i] != int(size[i])) {
							is_valid = false;
							break;
						}
					}
				}
			} else if(entry.first == "data") {
				entry.second.to(values);
			}
		}
	}
	is_valid = is_valid && (values.size() == total_size);
	for(size_t i = 0; i < total_size; ++i) {
		if(is_valid && i < values.size()) {
			data[i] = values[i];
		} else {
			data[i] = T();
		}
	}
}

template<size_t N>
void read_image_size(std::istream& in, std::array<size_t, N>& size) {
	// not implemented yet
	for(size_t i = 0; i < N; ++i) {
		size[i] = 0;
	}
}

template<typename T, size_t N>
void read_image_data(std::istream& in, T* data, const std::array<size_t, N>& size) {
	// not implemented yet
}

template<typename T>
void type<T>::read(std::istream& in, T& value) {
	vnx::read(in, value);
}

/// Reads a value of type T from the JSON string
template<typename T>
void from_string(const std::string& str, T& value) {
	std::istringstream stream;
	stream.str(str);
	vnx::type<T>().read(stream, value);
}

template<typename T>
T from_string(const std::string& str) {
	T tmp;
	from_string(str, tmp);
	return tmp;
}

/// Reads a value of type T from a native string (ie. without quotes)
template<typename T>
void from_string_value(const std::string& str, T& value) {
	from_string(str, value);
}

template<typename T>
T from_string_value(const std::string& str) {
	T tmp;
	from_string_value(str, tmp);
	return tmp;
}

template<>
void from_string_value<std::string>(const std::string& str, std::string& value);

/// Reads a Value of type T from the JSON stream
/// If \p value is NULL will attempt to read polymorphic type via "__type" field.
template<typename T>
void from_string(const std::string& str, std::shared_ptr<T>& value) {
	if(!value) {
		Object tmp;
		vnx::from_string(str, tmp);
		const auto* type_code = vnx::get_type_code(tmp["__type"].to_string_value());
		if(type_code) {
			value = std::dynamic_pointer_cast<T>(type_code->create_value());
		}
		if(!value) {
			value = T::create();
		}
		value->from_object(tmp);
	} else {
		from_string(str, *value);
	}
}

/// Reads a Value of type T from the JSON stream
template<typename T>
void from_string(const std::string& str, std::shared_ptr<const T>& value) {
	std::shared_ptr<T> tmp;
	from_string(str, tmp);
	value = tmp;
}


} // vnx

#endif /* INCLUDE_VNX_INPUT_HPP_ */
