
#ifndef INCLUDE_VNX_TOBINARYSTRING_H
#define INCLUDE_VNX_TOBINARYSTRING_H

#include <vnx/Visitor.h>
#include <vnx/Memory.h>
#include <vnx/Output.h>

#include <cmath>


namespace vnx {

/** \brief Creates a binary string that is unique for the semantic content.
 * 
 * The generated string is invariant against different byte order, different integer size and type,
 * different array types and different list types.
 * 
 * Negative integers are not equal to any positive integer.
 * 
 * Floating point values that are integer are equal to an integer of the same value.
 */
class ToBinaryString : public Visitor {
public:
	ToBinaryString() : stream(&string), out(&stream) {}
	
	const Memory& get_string() {
		out.flush();
		return string;
	}
	
	void visit_null() override {}
	
	void visit(const bool& value) override {
		vnx::write(out, uint16_t(0xffaa));
		vnx::write(out, uint64_t(value));
	}

	void visit(const uint8_t& value) override {
		vnx::write(out, uint16_t(0xffaa));
		vnx::write(out, uint64_t(value));
	}
	
	void visit(const uint16_t& value) override {
		vnx::write(out, uint16_t(0xffaa));
		vnx::write(out, uint64_t(value));
	}
	
	void visit(const uint32_t& value) override {
		vnx::write(out, uint16_t(0xffaa));
		vnx::write(out, uint64_t(value));
	}
	
	void visit(const uint64_t& value) override {
		vnx::write(out, uint16_t(0xffaa));
		vnx::write(out, uint64_t(value));
	}
	
	void visit(const int8_t& value) override {
		if(value >= 0) {
			vnx::write(out, uint16_t(0xffaa));
		} else {
			vnx::write(out, uint16_t(0xffbb));
		}
		vnx::write(out, int64_t(value));
	}
	
	void visit(const int16_t& value) override {
		if(value >= 0) {
			vnx::write(out, uint16_t(0xffaa));
		} else {
			vnx::write(out, uint16_t(0xffbb));
		}
		vnx::write(out, int64_t(value));
	}
	
	void visit(const int32_t& value) override {
		if(value >= 0) {
			vnx::write(out, uint16_t(0xffaa));
		} else {
			vnx::write(out, uint16_t(0xffbb));
		}
		vnx::write(out, int64_t(value));
	}
	
	void visit(const int64_t& value) override {
		if(value >= 0) {
			vnx::write(out, uint16_t(0xffaa));
		} else {
			vnx::write(out, uint16_t(0xffbb));
		}
		vnx::write(out, int64_t(value));
	}
	
	void visit(const float32_t& value) override {
		if(std::floor(value) == value && std::fabs(value) <= 16777216.f) {
			visit(int64_t(value));
		} else {
			vnx::write(out, uint16_t(0xffcc));
			vnx::write(out, float64_t(value));
		}
	}
	
	void visit(const float64_t& value) override {
		if(std::floor(value) == value && std::fabs(value) <= 9007199254740993.) {
			visit(int64_t(value));
		} else {
			vnx::write(out, uint16_t(0xffcc));
			vnx::write(out, float64_t(value));
		}
	}
	
	void visit(const std::string& value) override {
		vnx::write(out, uint32_t(0xf5b72973));
		out.write(value.data(), value.size());
	}
	
	void list_begin(size_t size) override {}
	
	void list_element(size_t index) override {
		vnx::write(out, uint32_t(0xa4f82412));
	}
	
	void list_end(size_t size) override {
		vnx::write(out, uint32_t(0x7d804d8b));
	}
	
	void map_begin(size_t size) override {}
	
	void map_entry_begin(size_t index) override {
		vnx::write(out, uint32_t(0x294b5acb));
	}
	
	void map_entry_value(size_t index) override {
		vnx::write(out, uint32_t(0x294b5acc));
	}
	
	void map_entry_end(size_t index) override {}
	
	void map_end(size_t size) override {
		vnx::write(out, uint32_t(0x7d804d8b));
	}
	
	void type_begin(size_t num_fields, const std::string& type_name) override {}
	
	void type_field(const std::string& field, size_t index) override {
		vnx::write(out, uint32_t(0x749354c0));
		out.write(field.data(), field.size());
	}
	
	void type_end(size_t num_fields, const std::string& type_name) override {
		vnx::write(out, uint32_t(0x7d804d8b));
	}
	
	void enum_value(uint32_t value, const std::string& name) override {
		vnx::write(out, value);
	}
	
private:
	Memory string;
	MemoryOutputStream stream;
	TypeOutput out;
	
};


} // vnx

#endif // INCLUDE_VNX_TOBINARYSTRING_H
