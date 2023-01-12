/*
 * JRPC_Server.h
 *
 *  Created on: Dec 4, 2020
 *      Author: jaw
 */

#ifndef INCLUDE_VNX_JRPC_SERVER_H_
#define INCLUDE_VNX_JRPC_SERVER_H_

#include <vnx/JRPC_ServerBase.hxx>


namespace vnx {


class JRPC_Server : public JRPC_ServerBase {
public:
	/// Create a Server listening on \p endpoint_, or if null, on \b address
	JRPC_Server(const std::string& name_, std::shared_ptr<const Endpoint> endpoint_ = nullptr);

private:
	void spawn_proxy(int socket) override;
	
};


} // vnx

#endif /* INCLUDE_VNX_JRPC_SERVER_H_ */
