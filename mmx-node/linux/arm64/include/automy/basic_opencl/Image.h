/*
 * Image.h
 *
 *  Created on: Feb 5, 2021
 *      Author: mad
 */

#ifndef INCLUDE_AUTOMY_BASIC_OPENCL_IMAGE_H_
#define INCLUDE_AUTOMY_BASIC_OPENCL_IMAGE_H_

#include <automy/basic_opencl/Context.h>


namespace automy {
namespace basic_opencl {

class Image {
public:
	~Image() {
		clReleaseMemObject(data_);
	}

	cl_mem data() const {
		return data_;
	}

protected:
	cl_mem data_ = 0;

};


} // basic_opencl
} // automy

#endif /* INCLUDE_AUTOMY_BASIC_OPENCL_IMAGE_H_ */
