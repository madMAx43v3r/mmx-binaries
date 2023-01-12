/*
 * Image2D.h
 *
 *  Created on: Feb 5, 2021
 *      Author: mad
 */

#ifndef INCLUDE_AUTOMY_BASIC_OPENCL_IMAGE2D_H_
#define INCLUDE_AUTOMY_BASIC_OPENCL_IMAGE2D_H_

#include <automy/basic_opencl/Image.h>
#include <automy/basic_opencl/Context.h>
#include <automy/basic_opencl/Buffer3D.h>
#include <automy/basic_opencl/CommandQueue.h>


namespace automy {
namespace basic_opencl {

template<typename T>
class Image2D : public Image {
public:
	Image2D(size_t width, size_t height, cl_mem_flags flags,
			cl_channel_order order, cl_channel_type type, const T* data = nullptr)
		:	Image(), width_(width), height_(height)
	{
		format_.image_channel_order = order;
		format_.image_channel_data_type = type;
		desc_.image_type = CL_MEM_OBJECT_IMAGE2D;
		desc_.image_width = width;
		desc_.image_height = height;
		cl_int err = 0;
		data_ = clCreateImage(g_context, flags, &format_, &desc_, (void*)data, &err);
		if(err) {
			throw std::runtime_error("clCreateImage2D() failed with " + get_error_string(err));
		}
	}

	size_t width() const {
		return width_;
	}

	size_t height() const {
		return height_;
	}

	size_t size() const {
		return width_ * height_;
	}

	void copy_from(std::shared_ptr<CommandQueue> queue, const Buffer3D<T>& buffer)
	{
		if(cl_int err = clEnqueueCopyBufferToImage(queue.get(), buffer.data(), data_, 0, {0, 0, 0}, {width_, height_, 1}, 0, nullptr, nullptr)) {
			throw std::runtime_error("clEnqueueCopyBufferToImage() failed with " + get_error_string(err));
		}
	}

private:
	size_t width_ = 0;
	size_t height_ = 0;

	cl_image_format format_ = {};
	cl_image_desc desc_ = {};

};


} // basic_opencl
} // automy

#endif /* INCLUDE_AUTOMY_BASIC_OPENCL_IMAGE2D_H_ */
