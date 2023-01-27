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

#ifndef INCLUDE_VNX_OUTPUTSTREAM_H_
#define INCLUDE_VNX_OUTPUTSTREAM_H_

#include <vnx/Type.h>

#include <cstring>
#include <streambuf>
#include <unordered_map>


namespace vnx {

/// Base class for output streams
class OutputStream {
public:
	virtual ~OutputStream() = default;
	
	/** \brief Writes \p len bytes to the stream
	 * 
	 * @param buf Pointer to data
	 * @param len Number of bytes to write
	 */
	virtual void write(const void* buf, size_t len) = 0;
	
	virtual int64_t get_output_pos() const { return 0; }
	
};


/// Output stream for POSIX file descriptors
class BasicOutputStream : public OutputStream {
public:
	BasicOutputStream() = default;
	
	BasicOutputStream(int fd_) : fd(fd_) {}
	
	void write(const void* buf, size_t len) override;
	
	void reset(int fd_) {
		fd = fd_;
	}
	
private:
	int fd = -1;
	
};


/// Output stream for files
class FileOutputStream : public OutputStream {
public:
	FileOutputStream() = default;
	
	FileOutputStream(::FILE* file_) : file(file_) {}
	
	void write(const void* buf, size_t len) override;
	
	int64_t get_output_pos() const override;
	
	void reset(::FILE* file_) {
		file = file_;
	}
	
private:
	::FILE* file = nullptr;
	
};


/// Output stream for std::vector<uint8_t>
class VectorOutputStream : public OutputStream {
public:
	VectorOutputStream() = default;
	
	VectorOutputStream(std::vector<uint8_t>* vector_) : vector(vector_) {}
	
	void write(const void* buf, size_t len) override;
	
	int64_t get_output_pos() const override;
	
	void reset(std::vector<uint8_t>* vector_) {
		vector = vector_;
	}
	
private:
	std::vector<uint8_t>* vector = 0;
	
};


/// Output stream for sockets
class SocketOutputStream : public OutputStream {
public:
	SocketOutputStream() = default;
	
	SocketOutputStream(int fd_) : fd(fd_) {}
	
	void write(const void* buf, size_t len) override;
	
	void reset(int fd_) {
		fd = fd_;
	}
	
private:
	int fd = -1;
	
};


/** \brief Output buffer for writing to a OutputStream.
 * 
 * Used to speed up writing of small amounts of data, while also allowing
 * efficient writing of large amounts of data by bypassing the buffer.
 */
class OutputBuffer {
public:
	OutputBuffer(OutputStream* stream_) : stream(stream_) {}
	
	~OutputBuffer() {
		// do not flush() here
	}
	
	OutputBuffer(const OutputBuffer& other) = delete;
	OutputBuffer& operator=(const OutputBuffer& other) = delete;
	
	/**
	 * Get a pointer to at least \p len number of bytes in the buffer.
	 * Will flush the buffer to the stream if not enough space available.
	 */
	char* write(size_t len) {
		if(len > VNX_BUFFER_SIZE) {
			throw std::invalid_argument("write(): buffer too small");
		}
		if(VNX_BUFFER_SIZE - pos < len) {
			flush();
		}
		char* res = buffer + pos;
		pos += len;
		return res;
	}
	
	/** \brief Write \p len number of bytes given by \p data to the stream.
	 * 
	 * Used to write large chunks of data, potentially bypassing the buffer.
	 */
	void write(const void* data, size_t len) {
		if(len < VNX_BUFFER_SIZE/2) {
			::memcpy(write(len), data, len);
		} else {
			flush();
			stream->write(data, len);
		}
	}
	
	/** \brief Flush the buffer to the stream.
	 * 
	 * Used to make sure that all data is actually written to the stream.
	 */
	void flush() {
		if(pos > 0) {
			stream->write(buffer, pos);
			pos = 0;
		}
	}
	
	/// Resets the buffer. Discards any data left over.
	void reset() {
		pos = 0;
	}
	
	/// Returns the buffer size.
	size_t get_buffer_size() const {
		return VNX_BUFFER_SIZE;
	}
	
	/// Returns the number of bytes currently left in the buffer.
	size_t get_buffer_pos() const {
		return pos;
	}
	
	/// Returns the number of bytes that have been written to the stream already, plus what is left in the buffer.
	int64_t get_output_pos() const {
		return stream->get_output_pos() + pos;
	}
	
	/// Returns pointer to buffer
	const void* get_buffer() const {
		return buffer;
	}

private:
	char buffer[VNX_BUFFER_SIZE];
	OutputStream* stream = 0;
	size_t pos = 0;
	
};


/** \brief Output buffer to write typed data to a OutputStream.
 * 
 * Used to write to a serialized data stream which will be read by a TypeInput.
 */
class TypeOutput : public OutputBuffer {
public:
	TypeOutput(OutputStream* stream_) : OutputBuffer::OutputBuffer(stream_) {}
	
	/** \brief Writes given type code to the stream if not already done so.
	 * 
	 * @return True if type code was actually written, false if already done so before.
	 */
	bool write_type_code(const TypeCode* type_code);
	
	/** \brief Resets the buffer and clears the type map.
	 * 
	 * Used as a way of creating a new TypeOutput in-place.
	 */
	void clear();
	
	/// To keep track of which type codes have already been written to the stream (and at what byte position)
	std::unordered_map<Hash64, int64_t> type_code_map;
	
	/// To disable writing type codes
	bool disable_type_codes = false;

};


/** \brief std::streambuf wrapper around OutputBuffer
 */
class TextOutput : public std::streambuf {
public:
	TextOutput(OutputStream* stream_) : buffer(stream_) {}

protected:
	std::streamsize xsputn(const char_type* s, std::streamsize n) override;

	int_type overflow(int_type c = traits_type::eof()) override;

	int sync() override;

private:
	OutputBuffer buffer;

};


/** \brief std::ostream wrapper around OutputStream
 */
class ostream : public std::ostream {
public:
    ostream(OutputStream* stream_) : std::ostream(&buffer), buffer(stream_) {}

private:
    TextOutput buffer;

};


} // vnx

#endif /* INCLUDE_VNX_OUTPUTSTREAM_H_ */
