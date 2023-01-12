/*
 * Buffer.h
 *
 *  Created on: Jul 12, 2018
 *      Author: mad
 */

#ifndef INCLUDE_AUTOMY_BASIC_OPENCL_BUFFER_H_
#define INCLUDE_AUTOMY_BASIC_OPENCL_BUFFER_H_

#include <automy/basic_opencl/Context.h>


namespace automy {
namespace basic_opencl {

class Buffer {
public:
	Buffer() {}
	
	~Buffer() {
		if(data_) {
			clReleaseMemObject(data_);
		}
	}
	
	Buffer(const Buffer& buf) = delete;
	Buffer& operator=(const Buffer& buf) = delete;
	
	cl_mem data() const {
		return data_;
	}
	
protected:
	cl_mem data_ = 0;
	
};


} // basic_opencl
} // automy

#endif /* INCLUDE_AUTOMY_BASIC_OPENCL_BUFFER_H_ */
