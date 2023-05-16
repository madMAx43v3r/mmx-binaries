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

#ifndef INCLUDE_VNX_INPUTSTREAM_H_
#define INCLUDE_VNX_INPUTSTREAM_H_

#include <vnx/Type.h>

#include <cstring>
#include <streambuf>
#include <unordered_map>


namespace vnx {

/// Base class for input streams
class InputStream {
public:
	virtual ~InputStream() {}
	
	/** \brief Reads up to \p len bytes into \p buf, returns number of bytes read.
	 * 
	 * @param buf Pointer to a buffer of \p len bytes.
	 * @param len Maximum number of bytes to read into \p buf.
	 * @return Number of bytes read into \p buf.
	 */
	virtual size_t read(void* buf, size_t len) = 0;
	
	/** \brief Returns total number of bytes read so far from stream.
	 * 
	 * In case of a file it returns the current read position, ie. seeking
	 * should be correctly handled.
	 */
	virtual int64_t get_input_pos() const { return 0; }
	
};


/// Input stream for file descriptors
class BasicInputStream : public InputStream {
public:
	BasicInputStream() = default;
	
	BasicInputStream(int fd_) : fd(fd_) {}
	
	size_t read(void* buf, size_t len) override;
	
	void reset(int fd_) {
		fd = fd_;
	}
	
private:
	int fd = -1;
	
};


/// Input stream for files
class FileInputStream : public InputStream {
public:
	FileInputStream() = default;
	
	FileInputStream(::FILE* file_) : file(file_) {}
	
	size_t read(void* buf, size_t len) override;
	
	int64_t get_input_pos() const override;
	
	void reset(::FILE* file_) {
		file = file_;
	}
	
private:
	::FILE* file = nullptr;
	
};


/// Input stream for a section of a file
class FileSectionInputStream : public InputStream {
public:
	FileSectionInputStream() = default;
	
	/* @param file_ File pointer
	 * @param offset_ Byte offset from beginning of file
	 * @param length_ Size of section to read, -1 = remaining
	 */
	FileSectionInputStream(::FILE* file_, int64_t offset_ = 0, int64_t length_ = -1, size_t buffer_size_ = 0) {
		reset(file_, offset_, length_, buffer_size_);
	}
	
	size_t read(void* buf, size_t len) override;
	
	int64_t get_input_pos() const override;
	
	/* @param file_ File pointer
	 * @param offset_ Byte offset from beginning of file
	 * @param length_ Size of section to read, -1 = remaining
	 */
	void reset(::FILE* file_, int64_t offset_ = 0, int64_t length_ = -1, size_t buffer_size_ = 0);
	
private:
	int fd = -1;
	int64_t offset = 0;
	int64_t length = 0;
	size_t buffer_size = 0;
	int64_t pos = 0;
	
};


/// Input stream for arbitrary memory
class PointerInputStream : public InputStream {
public:
	PointerInputStream() = default;
	
	PointerInputStream(const void* data_, size_t size_)
		:	data((const char*)data_),
			size(size_)
	{}
	
	size_t read(void* buf, size_t len) override;
	
	int64_t get_input_pos() const override {
		return pos;
	}
	
	void reset(const void* data_, size_t size_) {
		data = (const char*)data_;
		size = size_;
		pos = 0;
	}
	
	void set_input_pos(size_t pos_) {
		if(pos_ > size) {
			throw std::logic_error("pos_ > size");
		}
		pos = pos_;
	}
	
private:
	const char* data = 0;
	size_t size = 0;
	size_t pos = 0;
	
};


/// Input stream for binary vector
class VectorInputStream : public InputStream {
public:
	VectorInputStream() = default;
	
	VectorInputStream(const std::vector<uint8_t>* vector_) : vector(vector_) {}
	
	size_t read(void* buf, size_t len) override;
	
	int64_t get_input_pos() const override {
		return pos;
	}
	
	void reset(const std::vector<uint8_t>* vector_) {
		vector = vector_;
		pos = 0;
	}
	
	void set_input_pos(size_t pos_) {
		if(pos_ > vector->size()) {
			throw std::logic_error("pos_ > vector->size()");
		}
		pos = pos_;
	}
	
private:
	const std::vector<uint8_t>* vector = 0;
	size_t pos = 0;
	
};


/// Input stream for sockets
class SocketInputStream : public InputStream {
public:
	SocketInputStream() = default;

	SocketInputStream(int socket_) : socket(socket_) {}

	size_t read(void* buf, size_t len) override;

	void reset(int socket_) {
		socket = socket_;
	}

private:
	int socket = -1;

};


/** \brief Input buffer for reading from a InputStream.
 * 
 * Used to speed up reading of small amounts of data, while also allowing
 * efficient reading of large amounts of data by bypassing the buffer.
 */
class InputBuffer {
public:
	InputBuffer(InputStream* stream_) : stream(stream_) {}
	
	InputBuffer(const InputBuffer& other) = delete;
	InputBuffer& operator=(const InputBuffer& other) = delete;
	
	/** \brief Reads \p len bytes and returns a pointer to the data.
	 * 
	 * Returns a pointer to the next available data and advances the internal pointer by \p len bytes.
	 * Reads new data from the stream if available bytes are less than \p len.
	 * Argument \p len cannot be larger than the buffer size, which is VNX_BUFFER_SIZE.
	 */
	const char* read(size_t len) {
		if(len > VNX_BUFFER_SIZE) {
			throw std::invalid_argument("read(): buffer too small");
		}
		while(end - pos < len) {
			// move left over data to the beginning
			::memmove(buffer, buffer + pos, end - pos);
			end -= pos;
			pos = 0;
			// read new data into the buffer (after the left over data)
			const size_t num_bytes = stream->read(buffer + end, VNX_BUFFER_SIZE - end);
			if(!num_bytes) {
				throw std::underflow_error("read(): EOF");
			}
			end += num_bytes;
		}
		const char* res = buffer + pos;
		pos += len;
		return res;
	}
	
	/** \brief Reads \p len bytes into buffer given by \p buf.
	 * 
	 * This function blocks until all \p len bytes are read into the buffer \p buf.
	 * Used to read large chunks of data, potentially bypassing the buffer.
	 */
	void read(void* buf_, size_t len) {
		char* buf = (char*)buf_;
		const size_t left = end - pos;
		if(len < VNX_BUFFER_SIZE/2 || len < left) {
			// for small chunks just take it from the buffer if enough bytes are available
			::memcpy(buf, read(len), len);
			return;
		}
		if(left) {
			// first take whatever is left in the buffer
			::memcpy(buf, buffer + pos, left);
			buf += left;
			len -= left;
			pos = 0;
			end = 0;
		}
		while(len > 0) {
			// read remaining bytes directly from the stream
			const size_t num_bytes = stream->read(buf, len);
			if(!num_bytes) {
				throw std::underflow_error("read(): EOF");
			}
			buf += num_bytes;
			len -= num_bytes;
		}
	}
	
	/// Tries to have \p num_bytes ready in the buffer for reading (at least one byte), returns get_num_avail().
	size_t fetch(size_t num_bytes) {
		if(num_bytes > end - pos) {
			if(pos >= VNX_BUFFER_SIZE) {
				reset();
			}
			end += stream->read(buffer + end, VNX_BUFFER_SIZE - end);
		}
		return get_num_avail();
	}
	
	/// Resets the buffer, discarding any data left over
	void reset() {
		pos = 0;
		end = 0;
	}
	
	/// Returns number of bytes available in the buffer
	size_t get_num_avail() const {
		return end - pos;
	}
	
	/// Returns stream read position, while taking buffered data into account.
	int64_t get_input_pos() const {
		return stream->get_input_pos() - get_num_avail();
	}
	
	/// Direct buffer access, relative to current position, see get_num_avail() and fetch().
	const char& operator[](size_t i) const {
		return buffer[pos + i];
	}

private:
	char buffer[VNX_BUFFER_SIZE];
	InputStream* stream = 0;
	size_t pos = 0;
	size_t end = 0;
	
};


/** \brief Input buffer to read typed data from a InputStream.
 * 
 * Used to read from a serialized data stream which was written by TypeOutput.
 */
class TypeInput : public InputBuffer {
public:
	TypeInput(InputStream* stream_) : InputBuffer::InputBuffer(stream_) {}
	
	/// Same as vnx::get_type_code()
	const TypeCode* get_type_code(Hash64 code_hash) const;
	
	/** \brief Resets the buffer and clears the type map.
	 * 
	 * Used as a way of creating a new TypeInput for the same stream in-place.
	 */
	void clear();
	
	bool safe_read = false;											///< to avoid pre-allocation of large buffers

	size_t max_list_size = VNX_MAX_SIZE;							///< to limit maximum list size

	std::unordered_map<Hash64, const TypeCode*> type_code_map;		///< for faster lock-free lookup
	
};


/** \brief std::streambuf wrapper around InputBuffer
 */
class TextInput : public std::streambuf {
public:
	TextInput(InputStream* stream_) : buffer(stream_) {}

protected:
	std::streamsize showmanyc() override;

	std::streamsize xsgetn(char_type* s, std::streamsize n) override;

	int_type underflow() override;

private:
	InputBuffer buffer;

};


/** \brief std::istream wrapper around InputStream
 */
class istream : public std::istream {
public:
    istream(InputStream* stream_) : std::istream(&buffer), buffer(stream_) {}

private:
    TextInput buffer;

};


} // vnx

#endif /* INCLUDE_VNX_INPUTSTREAM_H_ */
