/*
 * CommandQueue.h
 *
 *  Created on: Jul 13, 2018
 *      Author: mad
 */

#ifndef INCLUDE_AUTOMY_BASIC_OPENCL_COMMANDQUEUE_H_
#define INCLUDE_AUTOMY_BASIC_OPENCL_COMMANDQUEUE_H_

#include <automy/basic_opencl/OpenCL.h>

#include <stdexcept>
#include <memory>


namespace automy {
namespace basic_opencl {

class CommandQueue {
public:
	CommandQueue(cl_command_queue queue_) : queue(queue_) {}
	
	~CommandQueue() {
		clReleaseCommandQueue(queue);
	}
	
	CommandQueue(const CommandQueue& buf) = delete;
	CommandQueue& operator=(const CommandQueue& buf) = delete;
	
	static std::shared_ptr<CommandQueue> create(cl_command_queue queue_) {
		return std::make_shared<CommandQueue>(queue_);
	}
	
	cl_command_queue get() const {
		return queue;
	}
	
	void flush() {
		if(clFlush(queue)) {
			throw std::runtime_error("clFlush() failed");
		}
	}
	
	void finish() {
		if(clFinish(queue)) {
			throw std::runtime_error("clFinish() failed");
		}
	}
	
private:
	cl_command_queue queue;
	
};


} // basic_opencl
} // automy

#endif /* INCLUDE_AUTOMY_BASIC_OPENCL_COMMANDQUEUE_H_ */
