 /*************************************************************************
 *
 *  [2017] - [2018] Automy Inc.
 *  All Rights Reserved.
 *
 * NOTICE:  All information contained herein is, and remains
 * the property of Automy Incorporated and its suppliers,
 * if any.  The intellectual and technical concepts contained
 * herein are proprietary to Automy Incorporated
 * and its suppliers and may be covered by U.S. and Foreign Patents,
 * patents in process, and are protected by trade secret or copyright law.
 * Dissemination of this information or reproduction of this material
 * is strictly forbidden unless prior written permission is obtained
 * from Automy Incorporated.
 */

#ifndef INCLUDE_VNX_GENERICCLIENT_H_
#define INCLUDE_VNX_GENERICCLIENT_H_

#include <vnx/ModuleInterfaceClient.hxx>
#include <vnx/Object.h>


namespace vnx {

/** \brief Used to send generic requests.
 *
 * GenericClient is [thread-safe] now.
 */
class GenericClient : public ModuleInterfaceClient {
public:
	GenericClient(const std::string& service_name);

	GenericClient(vnx::Hash64 service_addr);

	/** \brief Synchronous service request.
	 *
	 * Will throw exceptions in case of failure.
	 *
	 * @param method Name of the method to call, either full type name or just the function name.
	 * @param args Arguments to the function as an Object.
	 */
	std::shared_ptr<const Value> call(const std::string& method, const Object& args);

	std::shared_ptr<const Value> call(std::shared_ptr<const Value> method);

	/** \brief Asynchronous service request.
	 *
	 * Does not throw exceptions in any case.
	 *
	 * @param method Name of the method to call, either full type name or just the function name.
	 * @param args Arguments to the function as an Object.
	 */
	void call_async(const std::string& method, const Object& args);

};


} // vnx

#endif /* INCLUDE_VNX_GENERICCLIENT_H_ */
