/*
 * AsyncClient.h
 *
 *  Created on: Mar 7, 2020
 *      Author: mad
 */

#ifndef INCLUDE_VNX_ASYNCCLIENT_H_
#define INCLUDE_VNX_ASYNCCLIENT_H_

#include <vnx/Node.h>
#include <vnx/Binary.h>
#include <vnx/exception.h>
#include <vnx/NoSuchService.hxx>
#include <vnx/NoSuchMethod.hxx>
#include <vnx/InternalError.hxx>
#include <vnx/OverflowException.hxx>
#include <vnx/Return.hxx>
#include <vnx/request_t.h>

#include <functional>


namespace vnx {

/** \brief Same as Client but asynchronous, callbacks will be executed from the Modules main thread.
 *
 * This is the base class which is inherited from in the generated code for
 * each of the specific clients.
 *
 * AsyncClient is [thread-safe] now.
 */
class AsyncClient {
public:
	/// Creates a client for the specified service address.
	AsyncClient(Hash64 service_addr);
	
	virtual ~AsyncClient();
	
	/// Returns service address
	Hash64 vnx_get_service_addr() const {
		return vnx_service_addr;
	}
	
	/// Returns this client's src_mac (unique identifier)
	Hash64 vnx_get_src_mac() const {
		return vnx_src_mac;
	}
	
	/// Returns the number of pending requests
	size_t vnx_get_num_pending() const {
		return vnx_pending.size();
	}
	
	/// Sets the node which will receive and process the returns
	void vnx_set_node(Node* node);
	
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

	/// Called by the receiving node when a return has been received
	void vnx_callback(std::shared_ptr<const Return> result);
	
	/// Called by the receiving node when the remote service has closed
	void vnx_shutdown(const std::string& message);
	
	/** \brief Register a callback function which will be called when a request fails.
	 * 
	 * @param callback Callback function with signature (uint64_t request_id, const std::exception& exception).
	 */
	void vnx_set_error_callback(const std::function<void(uint64_t, const vnx::exception&)>& callback);
	
	/// Returns list of pending request ids
	std::vector<uint64_t> vnx_get_pending_ids() const;
	
protected:
	mutable std::mutex vnx_mutex;
	std::atomic<uint64_t> vnx_next_id {0};
	std::unordered_map<uint64_t, int32_t> vnx_pending;
	std::function<void(uint64_t, const vnx::exception&)> vnx_error_callback;
	
	/// Performs the actual request, non-blocking and without exceptions.
	void vnx_request(std::shared_ptr<const Value> method, const uint64_t request_id);
	
	/// Calls error callbacks with given exception
	void vnx_callback_error(uint64_t request_id, const vnx::exception& ex);

	virtual int32_t vnx_purge_request(uint64_t request_id, const vnx::exception& ex) = 0;
	
	virtual int32_t vnx_callback_switch(uint64_t request_id, std::shared_ptr<const Value> value) = 0;
	
private:
	Node* vnx_node = nullptr;
	Hash64 vnx_src_mac;
	Hash64 vnx_tunnel_addr;
	Hash64 vnx_gateway_addr;
	Hash64 vnx_service_addr;
	Hash64 vnx_session_id;
	bool vnx_is_non_blocking = false;
	
	std::shared_ptr<Pipe> vnx_service_pipe;
	std::shared_ptr<Pipe> vnx_return_pipe;

};


} // vnx

#endif /* INCLUDE_VNX_ASYNCCLIENT_H_ */
