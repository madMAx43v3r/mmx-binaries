/*
 * Context.h
 *
 *  Created on: Jul 12, 2018
 *      Author: mad
 */

#ifndef INCLUDE_AUTOMY_BASIC_OPENCL_CONTEXT_H_
#define INCLUDE_AUTOMY_BASIC_OPENCL_CONTEXT_H_

#include <automy/basic_opencl/OpenCL.h>
#include <automy/basic_opencl/CommandQueue.h>

#include <vector>
#include <string>
#include <memory>
#include <mutex>


namespace automy {
namespace basic_opencl {

std::vector<cl_platform_id> get_platforms();

std::string get_platform_name(cl_platform_id platform);

cl_platform_id find_platform_by_name(const std::string& name);

cl_context create_context(cl_platform_id platform, const std::vector<cl_device_id>& devices);

void release_context(cl_context& context);

std::vector<cl_device_id> get_devices(cl_platform_id platform, cl_device_type device_type);

cl_device_id get_device(cl_platform_id platform, cl_device_type device_type, cl_uint device);

std::string get_device_name(cl_device_id device_id);

std::shared_ptr<CommandQueue> create_command_queue(cl_context context, cl_device_id device);

std::string get_error_string(cl_int error);


} // basic_opencl
} // automy

#endif /* INCLUDE_AUTOMY_BASIC_OPENCL_CONTEXT_H_ */
