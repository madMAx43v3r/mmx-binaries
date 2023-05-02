/*
 * Program.h
 *
 *  Created on: Jul 12, 2018
 *      Author: mad
 */

#ifndef INCLUDE_AUTOMY_BASIC_OPENCL_PROGRAM_H_
#define INCLUDE_AUTOMY_BASIC_OPENCL_PROGRAM_H_

#include <automy/basic_opencl/Context.h>
#include <automy/basic_opencl/Kernel.h>

#include <set>
#include <vector>
#include <string>


namespace automy {
namespace basic_opencl {

class Program {
public:
	std::string options;
	
	std::vector<std::string> build_log;
	
	Program(cl_context context);
	
	Program(const Program&) = delete;
	Program& operator=(const Program&) = delete;
	
	~Program();
	
	static std::shared_ptr<Program> create(cl_context context);
	
	void add_source(const std::string& file_name);
	
	void add_source_code(const std::string& source);

	void add_include_path(const std::string& path);

	void create_from_source();
	
	bool build(const std::vector<cl_device_id>& devices, bool with_arg_names = true);
	
	void print_sources(std::ostream& out) const;
	
	void print_build_log(std::ostream& out) const;
	
	std::shared_ptr<Kernel> create_kernel(const std::string& name) const;
	
private:
	cl_context context;
	cl_program program = nullptr;
	bool have_arg_info = false;
	
	std::set<std::string> includes;
	std::vector<std::string> sources;

};


} // basic_opencl
} // automy

#endif /* INCLUDE_AUTOMY_BASIC_OPENCL_PROGRAM_H_ */
