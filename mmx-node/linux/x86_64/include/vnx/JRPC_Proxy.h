/*
 * JRPC_Proxy.h
 *
 *  Created on: Dec 4, 2020
 *      Author: jaw
 */

#ifndef INCLUDE_VNX_JRPC_PROXY_H_
#define INCLUDE_VNX_JRPC_PROXY_H_

#include <vnx/JRPC_ProxyBase.hxx>


namespace vnx {


class JRPC_Proxy : public JRPC_ProxyBase {
public:
	/** \brief Create new Proxy instance.
	 *
	 * @param name Name of the module
	 * @param endpoint Endpoint to connect to and re-connect in case of failure, optional
	 * @param socket Already connected socket, optional
	 */
	JRPC_Proxy(const std::string& name, std::shared_ptr<const Endpoint> endpoint = nullptr, int socket = -1);

protected:
	bool handle(std::shared_ptr<const Return> result) override;

	void select_service(const std::string &service_name) override;

private:
	void send_outgoing(std::shared_ptr<Sample> sample) override;

	void send_outgoing(std::shared_ptr<Request> request) override;

	void connect_reset() override;

	void read_socket_until_error(std::shared_ptr<Pipe> service_pipe) override;

	Object map_parameters(const Hash64 &service, const std::string &method, const std::vector<Variant> &parameters);

	mutable vnx::ostream out;
	Hash64 current_dst;
	uint64_t unique_request_id = 1;
	uint64_t unique_jrpc_id = 1;
	std::mutex mutex_request_id_incoming_jrpcmap;
	std::map<uint64_t, Variant> request_id_incoming_jrpcmap;     // [request_id => jrpc_id]
	std::mutex mutex_request_id_outgoing_jrpcmap;
	std::map<uint64_t, std::pair<Hash64, uint64_t>> request_id_outgoing_jrpcmap;   // [jrpc_id => (src_mac, request_id)]
};


} // vnx

#endif /* INCLUDE_VNX_JRPC_PROXY_H_ */
