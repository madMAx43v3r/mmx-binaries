/*
 * Buffer3D.h
 *
 *  Created on: Jul 12, 2018
 *      Author: mad
 */

#ifndef INCLUDE_AUTOMY_BASIC_OPENCL_BUFFER3D_H_
#define INCLUDE_AUTOMY_BASIC_OPENCL_BUFFER3D_H_

#include <automy/basic_opencl/Buffer.h>

#ifdef WITH_AUTOMY_BASIC
#include <automy/basic/Image.hpp>
#endif

#ifdef WITH_AUTOMY_MATH
#include <automy/math/MatrixX.hpp>
#endif


namespace automy {
namespace basic_opencl {

template<typename T>
class Buffer3D : public Buffer {
public:
	Buffer3D() {}
	
	Buffer3D(cl_context context, size_t width, size_t height, size_t depth = 1) {
		resize(context, width, height, depth);
	}
	
	static std::shared_ptr<Buffer3D<T>> create() {
		return std::make_shared<Buffer3D<T>>();
	}
	
	static std::shared_ptr<Buffer3D<T>> create(size_t width, size_t height, size_t depth = 1) {
		return std::make_shared<Buffer3D<T>>(width, height, depth);
	}
	
	void resize(cl_context context, size_t width, size_t height, size_t depth = 1) {
		const size_t new_size = width * height * depth;
		if(new_size != size()) {
			if(data_) {
				if(cl_int err = clReleaseMemObject(data_)) {
					throw std::runtime_error("clReleaseMemObject() failed with " + get_error_string(err));
				}
				data_ = 0;
			}
			if(new_size) {
				cl_int err = 0;
				data_ = clCreateBuffer(context, 0, width * height * depth * sizeof(T), nullptr, &err);
				if(err) {
					throw std::runtime_error("clCreateBuffer() failed with " + get_error_string(err));
				}
			}
		}
		width_ = width;
		height_ = height;
		depth_ = depth;
	}
	
	size_t width() const {
		return width_;
	}
	
	size_t height() const {
		return height_;
	}
	
	size_t depth() const {
		return depth_;
	}
	
	size_t size() const {
		return width_ * height_ * depth_;
	}
	
	void upload(std::shared_ptr<CommandQueue> queue, const T* data, bool copy = true) {
		if(data_) {
			if(cl_int err = clEnqueueWriteBuffer(queue->get(), data_, copy ? CL_TRUE : CL_FALSE, 0, size() * sizeof(T), data, 0, 0, 0)) {
				throw std::runtime_error("clEnqueueWriteBuffer() failed with " + get_error_string(err));
			}
		}
	}
	
	void upload(std::shared_ptr<CommandQueue> queue, const std::vector<T>& vec, bool copy = true) {
		upload(queue, vec.data(), copy);
	}
	
	void download(std::shared_ptr<CommandQueue> queue, T* data, bool blocking = true) const {
		if(data_) {
			if(cl_int err = clEnqueueReadBuffer(queue->get(), data_, blocking ? CL_TRUE : CL_FALSE, 0, size() * sizeof(T), data, 0, 0, 0)) {
				throw std::runtime_error("clEnqueueReadBuffer() failed with " + get_error_string(err));
			}
		}
	}

#ifdef WITH_AUTOMY_BASIC
	void upload(std::shared_ptr<CommandQueue> queue, const basic::Image<T>& img, bool copy = true) {
		upload(queue, img.get_data(), copy);
	}

	void download(std::shared_ptr<CommandQueue> queue, basic::Image<T>& img, bool blocking = true) const {
		img.resize(width_, height_, depth_);
		download(queue, img.get_data(), blocking);
	}
#endif

#ifdef WITH_AUTOMY_MATH
	void upload(std::shared_ptr<CommandQueue> queue, const math::MatrixX<T>& mat, bool copy = true) {
		upload(queue, mat.get_data(), copy);
	}

	void download(std::shared_ptr<CommandQueue> queue, math::MatrixX<T>& mat, bool blocking = true) const {
		if(depth_ != 1) {
			throw std::logic_error("dimension mismatch");
		}
		mat.resize(width_, height_);
		download(queue, mat.get_data(), blocking);
	}
#endif
	
	void copy_from(std::shared_ptr<CommandQueue> queue, const Buffer3D<T>& other) {
		if(data_) {
			if(cl_int err = clEnqueueCopyBuffer(queue->get(), other.data(), data_, 0, 0, size() * sizeof(T), 0, 0, 0)) {
				throw std::runtime_error("clEnqueueCopyBuffer() failed with " + get_error_string(err));
			}
		}
	}
	
	void set_zero(std::shared_ptr<CommandQueue> queue) {
		const T zero = T();
		if(data_) {
			if(cl_int err = clEnqueueFillBuffer(queue->get(), data_, &zero, sizeof(T), 0, size() * sizeof(T), 0, 0, 0)) {
				throw std::runtime_error("clEnqueueFillBuffer() failed with " + get_error_string(err));
			}
		}
	}
	
private:
	size_t width_ = 0;
	size_t height_ = 0;
	size_t depth_ = 0;
	
};


} // basic_opencl
} // automy

#endif /* INCLUDE_AUTOMY_BASIC_OPENCL_BUFFER3D_H_ */
