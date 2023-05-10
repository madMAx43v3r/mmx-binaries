
#ifndef INCLUDE_AUTOMY_BASIC_OPENCL_OPENCL_H_
#define INCLUDE_AUTOMY_BASIC_OPENCL_OPENCL_H_

#define CL_TARGET_OPENCL_VERSION 120

#ifdef __APPLE__
#include <OpenCL/cl.h>
#include <OpenCL/cl_ext.h>
#else
#include <CL/cl.h>
#include <CL/cl_ext.h>
#endif

#ifdef _MSC_VER
#include <automy_basic_opencl_export.h>
#else
#define AUTOMY_BASIC_OPENCL_EXPORT
#endif


#endif // INCLUDE_AUTOMY_BASIC_OPENCL_OPENCL_H_
