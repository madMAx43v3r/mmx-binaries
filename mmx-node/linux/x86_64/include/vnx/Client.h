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

#ifndef INCLUDE_VNX_CLIENT_H_
#define INCLUDE_VNX_CLIENT_H_

#include <vnx/Node.h>
#include <vnx/Type.h>
#include <vnx/Value.h>
#include <vnx/request_t.h>


namespace vnx {

/** \brief Base class for clients to perform remote procedure calls.
 * 
 * This is the base class which is inherited from in the generated code for
 * each of the specific clients.
 *
 * Client is [thread-safe] now.
 */
class Client : protected Node {
public:
	/// Creates a client for the specified service address.
	Client(Hash64 service_addr);

	~Client();
	
	/// Sets a custom session to be used for requests.
	void vnx_set_session(Hash64 id = Hash64());

	/// Sets a tunnel address for sending requests to.
	void vnx_set_tunnel(Hash64 tunnel_addr);

	/// Sets a tunnel name for sending requests to.
	void vnx_set_tunnel(const std::string& tunnel_name);
	
	/// Sets a gateway address for sending requests to.
	void vnx_set_gateway(Hash64 gateway_addr);

	/// Sets a gateway name for sending requests to.
	void vnx_set_gateway(const std::string& gateway_name);

	/// If \p non_blocking_mode == true requests will throw an exception instead of blocking (default = false)
	void vnx_set_non_blocking(bool non_blocking_mode);

protected:
	mutable std::mutex vnx_mutex;

	/// Performs the actual request, blocks in case vnx_is_async == false.
	std::shared_ptr<const Value> vnx_request(std::shared_ptr<const Value> method, const bool is_async = false);
	
private:
	Hash64 vnx_src_mac;
	Hash64 vnx_tunnel_addr;
	Hash64 vnx_gateway_addr;
	Hash64 vnx_service_addr;
	Hash64 vnx_session_id;
	bool vnx_is_non_blocking = false;
	std::atomic<uint64_t> vnx_next_id {0};
	
	std::shared_ptr<Pipe> vnx_service_pipe;
	std::shared_ptr<Pipe> vnx_return_pipe;

};


} // vnx

#endif /* INCLUDE_VNX_CLIENT_H_ */
