/*
 * Context.h
 *
 *  Created on: Jul 12, 2018
 *      Author: mad
 */

#ifndef INCLUDE_AUTOMY_BASIC_OPENCL_CONTEXT_H_
#define INCLUDE_AUTOMY_BASIC_OPENCL_CONTEXT_H_

#include <automy/basic_opencl/CommandQueue.h>

#ifdef _MSC_VER
#include <automy_basic_opencl_export.h>
#else
#define AUTOMY_BASIC_OPENCL_EXPORT
#endif

#include <CL/cl.h>

#include <vector>
#include <string>
#include <memory>
#include <mutex>


namespace automy {
namespace basic_opencl {

AUTOMY_BASIC_OPENCL_EXPORT extern std::mutex g_mutex;

AUTOMY_BASIC_OPENCL_EXPORT extern cl_platform_id g_platform;

AUTOMY_BASIC_OPENCL_EXPORT extern cl_context g_context;


void create_context(cl_device_type device_type, const std::string& platform_name = "");

void release_context();

std::vector<cl_device_id> get_devices();

std::string get_device_name(cl_device_id id);

std::shared_ptr<CommandQueue> create_command_queue(cl_uint device);

std::string get_error_string(cl_int error);


} // basic_opencl
} // automy

#endif /* INCLUDE_AUTOMY_BASIC_OPENCL_CONTEXT_H_ */
