/*
 * Buffer1D.h
 *
 *  Created on: Feb 21, 2023
 *      Author: mad
 */

#ifndef INCLUDE_AUTOMY_BASIC_OPENCL_BUFFER1D_H_
#define INCLUDE_AUTOMY_BASIC_OPENCL_BUFFER1D_H_

#include <automy/basic_opencl/Buffer.h>


namespace automy {
namespace basic_opencl {

template<typename T>
class Buffer1D : public Buffer {
public:
	Buffer1D() {}

	Buffer1D(cl_context context, size_t width, cl_mem_flags flags = 0) {
		alloc(context, width, flags);
	}

	static std::shared_ptr<Buffer1D<T>> create() {
		return std::make_shared<Buffer1D<T>>();
	}

	static std::shared_ptr<Buffer1D<T>> create(size_t width, cl_mem_flags flags = 0) {
		return std::make_shared<Buffer1D<T>>(width, flags);
	}

	void alloc(cl_context context, size_t new_size, cl_mem_flags flags = 0) {
		if(new_size != size() || flags != flags_) {
			if(data_) {
				if(cl_int err = clReleaseMemObject(data_)) {
					throw std::runtime_error("clReleaseMemObject() failed with " + get_error_string(err));
				}
				data_ = nullptr;
			}
			if(new_size) {
				cl_int err = 0;
				data_ = clCreateBuffer(context, flags, new_size * sizeof(T), nullptr, &err);
				if(err) {
					throw std::runtime_error("clCreateBuffer() failed with " + get_error_string(err));
				}
			}
		}
		size_ = new_size;
		flags_ = flags;
	}

	void alloc_min(cl_context context, size_t min_size, cl_mem_flags flags = 0) {
		if(min_size > size() || flags != flags_) {
			alloc(context, min_size, flags);
		}
	}

	size_t size() const {
		return size_;
	}

	size_t num_bytes() const {
		return size() * sizeof(T);
	}

	void upload(std::shared_ptr<CommandQueue> queue, const T* data, bool copy = true) {
		if(data_) {
			if(cl_int err = clEnqueueWriteBuffer(queue->get(), data_, copy ? CL_TRUE : CL_FALSE, 0, num_bytes(), data, 0, 0, 0)) {
				throw std::runtime_error("clEnqueueWriteBuffer() failed with " + get_error_string(err));
			}
		}
	}

	void upload(std::shared_ptr<CommandQueue> queue, const std::vector<T>& vec, bool copy = true) {
		upload(queue, vec.data(), copy);
	}

	void download(std::shared_ptr<CommandQueue> queue, T* data, bool blocking = true) const {
		if(data_) {
			if(cl_int err = clEnqueueReadBuffer(queue->get(), data_, blocking ? CL_TRUE : CL_FALSE, 0, num_bytes(), data, 0, 0, 0)) {
				throw std::runtime_error("clEnqueueReadBuffer() failed with " + get_error_string(err));
			}
		}
	}

	void download_count(std::shared_ptr<CommandQueue> queue, T* data, size_t count, bool blocking = true) const {
		if(data_) {
			if(cl_int err = clEnqueueReadBuffer(queue->get(), data_, blocking ? CL_TRUE : CL_FALSE, 0, count * sizeof(T), data, 0, 0, 0)) {
				throw std::runtime_error("clEnqueueReadBuffer() failed with " + get_error_string(err));
			}
		}
	}

	std::vector<T> download(std::shared_ptr<CommandQueue> queue) const {
		std::vector<T> res(size());
		if(data_) {
			if(cl_int err = clEnqueueReadBuffer(queue->get(), data_, CL_TRUE, 0, num_bytes(), res.data(), 0, 0, 0)) {
				throw std::runtime_error("clEnqueueReadBuffer() failed with " + get_error_string(err));
			}
		}
		return res;
	}

	void copy_from(std::shared_ptr<CommandQueue> queue, const Buffer1D<T>& other) {
		if(data_) {
			if(cl_int err = clEnqueueCopyBuffer(queue->get(), other.data(), data_, 0, 0, num_bytes(), 0, 0, 0)) {
				throw std::runtime_error("clEnqueueCopyBuffer() failed with " + get_error_string(err));
			}
		}
	}

	void set_zero(std::shared_ptr<CommandQueue> queue) {
		memset(queue, 0);
	}

	void memset(std::shared_ptr<CommandQueue> queue, const T value) {
		if(data_) {
			if(cl_int err = clEnqueueFillBuffer(queue->get(), data_, &value, sizeof(T), 0, num_bytes(), 0, 0, 0)) {
				throw std::runtime_error("clEnqueueFillBuffer() failed with " + get_error_string(err));
			}
		}
	}

private:
	size_t size_ = 0;
	cl_mem_flags flags_ = 0;

};


} // basic_opencl
} // automy

#endif /* INCLUDE_AUTOMY_BASIC_OPENCL_BUFFER1D_H_ */
