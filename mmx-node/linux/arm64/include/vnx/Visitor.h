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

#ifndef INCLUDE_VNX_VISITOR_H_
#define INCLUDE_VNX_VISITOR_H_

#include <vnx/Type.h>


namespace vnx {

/** \brief A Visitor is used to traverse any type in a generic manner.
 * 
 * See DefaultPrinter or PrettyPrinter for an example.
 */
class Visitor {
public:
	virtual ~Visitor() {}
	
	virtual void visit_null() = 0;
	virtual void visit(const bool& value) = 0;
	virtual void visit(const uint8_t& value) = 0;
	virtual void visit(const uint16_t& value) = 0;
	virtual void visit(const uint32_t& value) = 0;
	virtual void visit(const uint64_t& value) = 0;
	virtual void visit(const int8_t& value) = 0;
	virtual void visit(const int16_t& value) = 0;
	virtual void visit(const int32_t& value) = 0;
	virtual void visit(const int64_t& value) = 0;
	virtual void visit(const float32_t& value) = 0;
	virtual void visit(const float64_t& value) = 0;
	virtual void visit(const std::string& value) = 0;
	
	virtual void list_begin(size_t size) = 0;
	virtual void list_element(size_t index) = 0;
	virtual void list_end(size_t size) = 0;
	
	virtual void map_begin(size_t size);								///< By default calls list_begin(size)
	virtual void map_key(size_t index);									///< By default calls list_begin(2) + list_element(1)
	virtual void map_value(size_t index);								///< By default calls list_element(1)
	virtual void map_end(size_t size);									///< By default calls list_end(size)

	virtual void type_begin(size_t num_fields, const std::string* type_name = nullptr);		///< By default calls map_begin(num_fields)
	virtual void type_field(const std::string& field, size_t index);						///< By default calls map_key() + map_value()
	virtual void type_end(size_t num_fields, const std::string* type_name = nullptr);		///< By default calls map_end(num_fields)
	
	/** \brief Called for enum values.
	 * 
	 * @param value Integer enum value
	 * @param name String enum value
	 *
	 * By default calls visit(const std::string&), or visit(const uint32_t&) in case of unknown string.
	 */
	virtual void enum_value(uint32_t value, const std::string& name);
	
	virtual void type_begin(const TypeCode& type);						///< By default calls type_begin(num_fields, type.name)
	virtual void type_field(const TypeField& field, size_t index);		///< By default calls type_field(field.name, index)
	virtual void type_end(const TypeCode& type);						///< By default calls type_end(num_fields, type.name)
	
};


inline void accept(Visitor& visitor, const bool& value) { visitor.visit(value); }
inline void accept(Visitor& visitor, const uint8_t& value) { visitor.visit(value); }
inline void accept(Visitor& visitor, const uint16_t& value) { visitor.visit(value); }
inline void accept(Visitor& visitor, const uint32_t& value) { visitor.visit(value); }
inline void accept(Visitor& visitor, const uint64_t& value) { visitor.visit(value); }
inline void accept(Visitor& visitor, const int8_t& value) { visitor.visit(value); }
inline void accept(Visitor& visitor, const int16_t& value) { visitor.visit(value); }
inline void accept(Visitor& visitor, const int32_t& value) { visitor.visit(value); }
inline void accept(Visitor& visitor, const int64_t& value) { visitor.visit(value); }
inline void accept(Visitor& visitor, const float32_t& value) { visitor.visit(value); }
inline void accept(Visitor& visitor, const float64_t& value) { visitor.visit(value); }
inline void accept(Visitor& visitor, const std::string& value) { visitor.visit(value); }

template<typename T, size_t N>
void accept(Visitor& visitor, const std::array<T, N>& array) {
	visitor.list_begin(N);
	for(size_t i = 0; i < N; ++i) {
		visitor.list_element(i);
		vnx::type<T>().accept(visitor, array[i]);
	}
	visitor.list_end(N);
}

template<typename T>
void accept(Visitor& visitor, const std::vector<T>& vector) {
	visitor.list_begin(vector.size());
	for(size_t i = 0; i < vector.size(); ++i) {
		visitor.list_element(i);
		vnx::type<T>().accept(visitor, vector[i]);
	}
	visitor.list_end(vector.size());
}

template<typename T>
void accept(Visitor& visitor, const std::list<T>& list) {
	visitor.list_begin(list.size());
	auto iter = list.begin();
	for(size_t i = 0; iter != list.end(); ++i, ++iter) {
		visitor.list_element(i);
		vnx::type<T>().accept(visitor, *iter);
	}
	visitor.list_end(list.size());
}

template<typename T>
void accept(Visitor& visitor, const std::set<T>& set) {
	visitor.list_begin(set.size());
	size_t i = 0;
	for(const auto& entry : set) {
		visitor.list_element(i);
		vnx::type<T>().accept(visitor, entry);
		i++;
	}
	visitor.list_end(set.size());
}

template<typename K, typename V>
void accept(Visitor& visitor, const std::map<K, V>& map) {
	visitor.map_begin(map.size());
	size_t i = 0;
	for(const auto& entry : map) {
		visitor.map_key(i);
		vnx::type<K>().accept(visitor, entry.first);
		visitor.map_value(i);
		vnx::type<V>().accept(visitor, entry.second);
		i++;
	}
	visitor.map_end(map.size());
}

template<typename K, typename V>
void accept(Visitor& visitor, const std::pair<K, V>& value) {
	visitor.list_begin(2);
	visitor.list_element(0);
	vnx::type<K>().accept(visitor, value.first);
	visitor.list_element(1);
	vnx::type<V>().accept(visitor, value.second);
	visitor.list_end(2);
}

template<size_t index, typename... T>
typename std::enable_if<index == sizeof...(T), void>::type
visit_tuple(Visitor& visitor, const std::tuple<T...>& value) {}

template<size_t index, typename... T>
typename std::enable_if<index < sizeof...(T), void>::type
visit_tuple(Visitor& visitor, const std::tuple<T...>& value) {
	visitor.list_element(index);
	vnx::type<typename std::tuple_element<index, std::tuple<T...>>::type>().accept(visitor, std::get<index>(value));
	visit_tuple<index+1>(visitor, value);
}

template<typename... T>
void accept(Visitor& visitor, const std::tuple<T...>& value) {
	visitor.list_begin(sizeof...(T));
	visit_tuple<0>(visitor, value);
	visitor.list_end(sizeof...(T));
}

void accept(Visitor& visitor, const Value& value);

void accept(Visitor& visitor, const std::nullptr_t& value);

void accept(Visitor& visitor, TypeInput& in, const TypeCode* type_code = nullptr, const uint16_t* code = nullptr);

template<typename T>
void accept(Visitor& visitor, std::shared_ptr<T> value) {
	if(value) {
		accept(visitor, *value);
	} else {
		visitor.visit_null();
	}
}

template<typename T>
void accept(Visitor& visitor, const vnx::optional<T>& value) {
	if(value) {
		vnx::type<T>().accept(visitor, *value);
	} else {
		visitor.visit_null();
	}
}

template<typename T, size_t N>
void accept_matrix(Visitor& visitor, const T* data, const std::array<size_t, N>& size) {
	size_t total_size = 1;
	bool is_array = true;
	for(size_t i = 0; i < N; ++i) {
		total_size *= size[i];
		if(i > 0 && size[i] != 1) {
			is_array = false;
		}
	}
	if(is_array) {
		visitor.list_begin(total_size);
		for(size_t i = 0; i < total_size; ++i) {
			visitor.list_element(i);
			accept(visitor, data[i]);
		}
		visitor.list_end(total_size);
	} else {
		visitor.type_begin(2);
		visitor.type_field("size", 0);
		visitor.list_begin(N);
		for(size_t i = 0; i < N; ++i) {
			visitor.list_element(i);
			visitor.visit(size[i]);
		}
		visitor.list_end(N);
		visitor.type_field("data", 1);
		if(data) {
			visitor.list_begin(total_size);
			for(size_t i = 0; i < total_size; ++i) {
				visitor.list_element(i);
				accept(visitor, data[i]);
			}
			visitor.list_end(total_size);
		} else {
			visitor.visit_null();
		}
		visitor.type_end(2);
	}
}

template<typename T, size_t N>
void accept_image(Visitor& visitor, const T* data, const std::array<size_t, N>& size) {
	accept_matrix(visitor, data, size);
}

template<typename T>
void type<T>::accept(Visitor& visitor, const T& value) {
	vnx::accept(visitor, value);
}


} // vnx

#endif /* INCLUDE_VNX_VISITOR_H_ */
