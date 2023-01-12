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

#ifndef INCLUDE_VNX_BINARY_H_
#define INCLUDE_VNX_BINARY_H_

#include <vnx/Value.h>
#include <vnx/Object.h>
#include <vnx/Memory.h>
#include <vnx/InputStream.h>
#include <vnx/OutputStream.h>


namespace vnx {

/** \brief Represents a Value in binary serialized form.
 * 
 * Binary is used when reading or receiving a non-native value, for which the current process
 * does not have a native C++ type compiled in.
 */
class Binary : public Value {
public:
	Memory data;
	const TypeCode* type_code = 0;
	
	typedef vnx::Value Super;
	
	static const Hash64 VNX_TYPE_HASH;
	static const Hash64 VNX_CODE_HASH;
	
	Binary();
	~Binary() override;
	
	Hash64 get_type_hash() const override;
	std::string get_type_name() const override;
	const TypeCode* get_type_code() const override;
	
	static std::shared_ptr<Binary> create();
	std::shared_ptr<Value> clone() const override;
	
	void read(TypeInput& in, const TypeCode* type_code, const uint16_t* code) override;
	void write(TypeOutput& out, const TypeCode* type_code, const uint16_t* code) const override;
	
	void read(std::istream& in) override;
	void write(std::ostream& out) const override;
	
	void accept(Visitor& visitor) const override;
	
	Object to_object() const override;
	void from_object(const Object& object) override;
	
	Variant get_field(const std::string& name) const override;
	void set_field(const std::string& name, const Variant& value) override;

	void clear();		///< Creates a new object in place
	
};


/// Used to read from a Binary
class BinaryInputStream : public MemoryInputStream {
public:
	BinaryInputStream(const Binary* binary) : MemoryInputStream(&binary->data) {}
};

/// Used to write to a Binary
class BinaryOutputStream : public MemoryOutputStream {
public:
	BinaryOutputStream(Binary* binary) : MemoryOutputStream(&binary->data) {}
};


void read(TypeInput& in, vnx::Binary& binary, const TypeCode* type_code, const uint16_t* code);

void write(TypeOutput& out, const vnx::Binary& binary, const TypeCode* type_code, const uint16_t* code);

void write(std::ostream& out, const vnx::Binary& binary);

void accept(Visitor& visitor, const vnx::Binary& binary);


} // vnx

#endif /* INCLUDE_VNX_BINARY_H_ */
