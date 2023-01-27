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

#ifndef INCLUDE_VNX_GENERICASYNCCLIENT_H_
#define INCLUDE_VNX_GENERICASYNCCLIENT_H_

#include <vnx/ModuleInterfaceAsyncClient.hxx>
#include <vnx/Object.h>


namespace vnx {

/** \brief Used to send generic requests asynchronously.
 *
 * GenericAsyncClient is [thread-safe] now.
 */
class GenericAsyncClient : public ModuleInterfaceAsyncClient {
public:
	GenericAsyncClient(const std::string& service_name);

	GenericAsyncClient(vnx::Hash64 service_addr);

	/** \brief Asynchronous service request.
	 *
	 * Does not throw exceptions in any case.
	 *
	 * @param method Name of the method to call, either full type name or just the function name.
	 * @param args Arguments to the function as an Object.
	 * @param callback Callback which is called from the main thread upon receiving the return.
	 * @param error_callback Callback which is called from the main thread upon failure.
	 * @return The request id used for this call.
	 */
	uint64_t call(	const std::string& method, const Object& args,
					const std::function<void(std::shared_ptr<const Value>)>& callback = std::function<void(std::shared_ptr<const Value>)>(),
					const std::function<void(const vnx::exception&)>& error_callback = std::function<void(const vnx::exception&)>());

	uint64_t call(	std::shared_ptr<const Value> method,
					const std::function<void(std::shared_ptr<const Value>)>& callback = std::function<void(std::shared_ptr<const Value>)>(),
					const std::function<void(const vnx::exception&)>& error_callback = std::function<void(const vnx::exception&)>());

protected:
	int32_t vnx_purge_request(uint64_t request_id, const vnx::exception& ex) override;

	int32_t vnx_callback_switch(uint64_t request_id, std::shared_ptr<const vnx::Value> value) override;

private:
	std::map<uint64_t, std::pair<std::function<void(std::shared_ptr<const Value>)>,
			std::function<void(const vnx::exception&)>>> vnx_queue_generic;

};


} // vnx

#endif /* INCLUDE_VNX_GENERICASYNCCLIENT_H_ */
