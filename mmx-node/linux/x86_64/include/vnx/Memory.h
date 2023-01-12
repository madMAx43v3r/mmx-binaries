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

#ifndef INCLUDE_VNX_MEMORY_H
#define INCLUDE_VNX_MEMORY_H

#include <vnx/Type.h>
#include <vnx/InputStream.h>
#include <vnx/OutputStream.h>


namespace vnx {

/** \brief Class to handle a list of memory chunks as one blob.
 * 
 * Memory is used as a buffer to serialize into and de-serialize from if needed.
 * Internally it holds a list of memory chunks to avoid re-allocation during serialization.
 */
class Memory {
public:
	struct chunk_t {
		size_t size = 0;
		char* data = nullptr;
		chunk_t* next = nullptr;
		
		chunk_t(size_t num_bytes) {
			size = num_bytes;
			data = (char*)::malloc(num_bytes);
		}
		~chunk_t() {
			::free(data);
			data = nullptr;
		}
	};
	
	Memory() = default;
	
	~Memory() {
		clear();
	}
	
	Memory(const Memory&) = delete;
	Memory& operator=(const Memory&) = delete;
	
	bool empty() const {
		return front_ == nullptr;
	}
	
	/// Returns pointer to first chunk
	const chunk_t* front() const {
		return front_;
	}
	
	/// Computes CRC64 hash of memory content
	Hash64 get_hash() const;
	
	/// Adds a chunk of \p num_bytes bytes and returns pointer to chunk data
	char* add_chunk(size_t num_bytes);
	
	/// Returns total size in bytes
	size_t get_size() const;
	
	/// Writes memory content to \p out
	void write(OutputStream& out) const;
	
	/// Writes memory content to \p out
	void write(OutputBuffer& out) const;
	
	/// Frees all memory
	void clear();
	
	/// Returns a string representation of the memory content
	std::string as_string() const;
	
private:
	chunk_t* front_ = nullptr;
	chunk_t* back_ = nullptr;
	
};


/// Used to write to a Memory object
class MemoryOutputStream : public OutputStream {
public:
	MemoryOutputStream(Memory* data_) : data(data_) {
		pos = data->get_size();
	}
	
	void write(const void* buf, size_t len) override {
		::memcpy(data->add_chunk(len), buf, len);
		pos += len;
	}
	
	int64_t get_output_pos() const override {
		return int64_t(pos);
	}
	
private:
	Memory* data = nullptr;
	size_t pos = 0;
	
};


/// Used to read from a Memory object
class MemoryInputStream : public InputStream {
public:
	MemoryInputStream(const Memory* data_) : data(data_) {
		chunk = data_->front();
	}
	
	size_t read(void* buf, size_t len) override {
		if(!chunk) {
			return 0;
		}
		const size_t left = chunk->size - offset;
		if(len > left) {
			len = left;
		}
		::memcpy(buf, chunk->data + offset, len);
		offset += len;
		if(offset >= chunk->size) {
			chunk = chunk->next;
			pos += offset;
			offset = 0;
		}
		return len;
	}

	int64_t get_input_pos() const override {
		return int64_t(pos + offset);
	}

	void reset(const Memory* data_) {
		data = data_;
		reset();
	}

	void reset() {
		chunk = data->front();
		offset = 0;
		pos = 0;
	}

private:
	const Memory* data = nullptr;
	const Memory::chunk_t* chunk = nullptr;
	size_t offset = 0;
	size_t pos = 0;
	
};


} // vnx

#endif /* INCLUDE_VNX_MEMORY_H */
