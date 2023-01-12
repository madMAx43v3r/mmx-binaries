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

#ifndef INCLUDE_VNX_PROXY_H_
#define INCLUDE_VNX_PROXY_H_

#include <vnx/ProxyBase.hxx>
#include <vnx/Endpoint.hxx>


namespace vnx {


class VNX_EXPORT Proxy : public ProxyBase {
public:
	/** \brief Create new Proxy instance.
	 * 
	 * @param name Name of the module
	 * @param endpoint Endpoint to connect to and re-connect in case of failure, optional
	 * @param socket Already connected socket, optional
	 */
	Proxy(const std::string& name, std::shared_ptr<const Endpoint> endpoint = nullptr, int socket = -1);

protected:
	void handle(std::shared_ptr<const Frame> frame) override;
	
	bool handle(std::shared_ptr<const Return> result) override;

	void handle(std::shared_ptr<const FlowMessage> flow_msg) override;

private:
	void send_outgoing(std::shared_ptr<Sample> sample) override;

	void send_outgoing(std::shared_ptr<Request> request) override;

	void connect_reset() override;

	void read_socket_until_error(std::shared_ptr<Pipe> service_pipe) override;

private:
	TypeOutput out;

};


} // vnx

#endif /* INCLUDE_VNX_PROXY_H_ */
