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

#ifndef INCLUDE_VNX_BUFFER_H
#define INCLUDE_VNX_BUFFER_H

#include <vnx/package.hxx>
#include <vnx/InputStream.h>

#include <cstring>
 

namespace vnx {

/** \brief Class for handling a chunk of memory.
 * 
 * Similar to std::vector<char> this class handles a chunk of memory, the
 * difference being that the data is not zero-initialized with the Buffer.
 */
class Buffer {
public:
	/// Creates an empty buffer
	Buffer() = default;
	
	/// Creates an empty buffer with a capacity of \p init_capacity
	explicit Buffer(size_t init_capacity);
	
	/// Performs a deep-copy
	Buffer(const Buffer& other);
	
	/// Creates a buffer based on the given memory \p other
	Buffer(const Memory& other);

	/// Creates a buffer based on the given string \p other
	Buffer(const std::string& other);
	
	~Buffer();
	
	/// Performs a deep-copy
	Buffer& operator=(const Buffer& other);
	
	/// Performs a deep-copy
	Buffer& operator=(const Memory& other);

	/// Performs a deep-copy
	Buffer& operator=(const std::string& other);
	
	/// Same as std::vector<>::reserve()
	void reserve(size_t new_capacity);
	
	/// Same as std::vector<>::resize()
	void resize(size_t new_size);
	
	/// Same as std::vector<>::size()
	size_t size() const {
		return size_;
	}
	
	/// Same as std::vector<>::capacity()
	size_t capacity() const {
		return capacity_;
	}
	
	/// Same as std::vector<>::empty()
	bool empty() const {
		return size_ == 0;
	}

	/// Same as std::vector<>::data()
	void* data() {
		return data_;
	}
	
	/// Same as std::vector<>::data()
	const void* data() const {
		return data_;
	}
	
	/// Returns a pointer to the data at byte offset \p offset
	void* data(size_t offset) {
		return ((char*)data_ + offset);
	}
	
	/// Returns a const pointer to the data at byte offset \p offset
	const void* data(size_t offset) const {
		return ((const char*)data_ + offset);
	}
	
	/// Clears the buffer without de-allocating the data
	void release();
	
	/// Same as std::vector<>::clear()
	void clear();
	
	/// clear() + deallocate memory
	void reset();
	
	/// Returns a string representation of the buffer
	std::string as_string() const;
	
	/// Reads a file into buffer, appending the data.
	void read_file(const std::string& file_path);

private:
	void* data_ = nullptr;
	size_t size_ = 0;
	size_t capacity_ = 0;
	
};


/// Used to read from a Buffer
class BufferInputStream : public InputStream {
public:
	BufferInputStream(const Buffer* data_) : data(data_) {}
	
	size_t read(void* buf, size_t len) override;

	int64_t get_input_pos() const override {
		return int64_t(pos);
	}

	void reset(const Buffer* data_) {
		data = data_;
		reset();
	}

	void reset() {
		pos = 0;
	}

private:
	const Buffer* data = nullptr;
	size_t pos = 0;
	
};


} // vnx

#endif // INCLUDE_VNX_BUFFER_H
