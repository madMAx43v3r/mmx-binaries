/*
 * Kernel.h
 *
 *  Created on: Jul 12, 2018
 *      Author: mad
 */

#ifndef INCLUDE_AUTOMY_BASIC_OPENCL_KERNEL_H_
#define INCLUDE_AUTOMY_BASIC_OPENCL_KERNEL_H_

#include <automy/basic_opencl/Context.h>
#include <automy/basic_opencl/Buffer.h>
#include <automy/basic_opencl/Image.h>

#include <map>
#include <string>
#include <array>
#include <vector>
#include <ostream>


namespace automy {
namespace basic_opencl {

class Kernel {
public:
	Kernel(cl_kernel kernel_, bool with_arg_map);
	
	~Kernel();
	
	Kernel(const Kernel&) = delete;
	Kernel& operator=(const Kernel&) = delete;
	
	static std::shared_ptr<Kernel> create(cl_kernel kernel, bool with_arg_map);
	
	void set(const cl_uint arg, const cl_int& value) { set_arg(arg, value); }
	void set(const cl_uint arg, const cl_long& value) { set_arg(arg, value); }
	void set(const cl_uint arg, const cl_uint& value) { set_arg(arg, value); }
	void set(const cl_uint arg, const cl_ulong& value) { set_arg(arg, value); }
	void set(const cl_uint arg, const cl_float& value) { set_arg(arg, value); }
	void set(const cl_uint arg, const Buffer& value) { set_arg(arg, value.data()); }
	void set(const cl_uint arg, const Image& value) { set_arg(arg, value.data()); }
	void set(const cl_uint arg, std::shared_ptr<const Buffer> value) { set_arg(arg, value->data()); }
	void set(const cl_uint arg, std::shared_ptr<const Image> value) { set_arg(arg, value->data()); }

	void set(const std::string& arg, const cl_int& value) { set_arg(arg, value); }
	void set(const std::string& arg, const cl_long& value) { set_arg(arg, value); }
	void set(const std::string& arg, const cl_uint& value) { set_arg(arg, value); }
	void set(const std::string& arg, const cl_ulong& value) { set_arg(arg, value); }
	void set(const std::string& arg, const cl_float& value) { set_arg(arg, value); }
	void set(const std::string& arg, const Buffer& value) { set_arg(arg, value.data()); }
	void set(const std::string& arg, const Image& value) { set_arg(arg, value.data()); }
	void set(const std::string& arg, std::shared_ptr<const Buffer> value) { set_arg(arg, value->data()); }
	void set(const std::string& arg, std::shared_ptr<const Image> value) { set_arg(arg, value->data()); }
	
	void set_local(const std::string& arg, const size_t& num_bytes);
	
	void enqueue(std::shared_ptr<CommandQueue> queue, const size_t& global_size);
	void enqueue(std::shared_ptr<CommandQueue> queue, const size_t& global_size, const size_t& local_size);
	void enqueue_ceiled(std::shared_ptr<CommandQueue> queue, const size_t& global_size, const size_t& local_size);

	void enqueue_2D(std::shared_ptr<CommandQueue> queue, const std::array<size_t, 2>& global_size);
	void enqueue_2D(std::shared_ptr<CommandQueue> queue, const std::array<size_t, 2>& global_size, const std::array<size_t, 2>& local_size);
	void enqueue_ceiled_2D(std::shared_ptr<CommandQueue> queue, const std::array<size_t, 2>& global_size, const std::array<size_t, 2>& local_size);

	void enqueue_3D(std::shared_ptr<CommandQueue> queue, const std::array<size_t, 3>& global_size);
	void enqueue_3D(std::shared_ptr<CommandQueue> queue, const std::array<size_t, 3>& global_size, const std::array<size_t, 3>& local_size);
	void enqueue_ceiled_3D(std::shared_ptr<CommandQueue> queue, const std::array<size_t, 3>& global_size, const std::array<size_t, 3>& local_size);
	
	void print_info(std::ostream& out);
	
protected:
	template<typename T>
	void set_arg(const cl_uint arg, const T& value) {
		if(clSetKernelArg(kernel, arg, sizeof(T), &value)) {
			throw std::runtime_error("clSetKernelArg() failed for " + name + " : " + std::to_string(arg));
		}
	}

	template<typename T>
	void set_arg(const std::string& arg, const T& value) {
		auto it = arg_map.find(arg);
		if(it != arg_map.end()) {
			if(clSetKernelArg(kernel, it->second, sizeof(T), &value)) {
				throw std::runtime_error("clSetKernelArg() failed for " + name + " : " + arg);
			}
		} else {
			throw std::logic_error("no such argument '" + arg + "' in kernel '" + name + "'");
		}
	}
	
private:
	cl_kernel kernel = nullptr;
	
	std::string name;
	std::vector<std::string> arg_list;
	std::map<std::string, cl_uint> arg_map;
	
};


} // basic_opencl
} // automy

#endif /* INCLUDE_AUTOMY_BASIC_OPENCL_KERNEL_H_ */
