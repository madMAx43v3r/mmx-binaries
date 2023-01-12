/*
 * BaseServer.h
 *
 *  Created on: Dec 4, 2020
 *      Author: jaw
 */

#ifndef INCLUDE_VNX_BASESERVER_H_
#define INCLUDE_VNX_BASESERVER_H_

#include <vnx/BaseServerBase.hxx>
#include <vnx/Endpoint.hxx>


namespace vnx {

/** \brief Module to listen on and accept client connections from other processes.
 *
 * Will spawn a Proxy for every new client connection.
 *
 * Configuration options:
 * - \b address Address to listen on, when given \p endpoint_ == 0. For example: "0.0.0.0" (default port 4444), "0.0.0.0:1234" or "domain.sock".
 * - \b export_list List of topics to export to every client without them asking for it.
 * - \b max_queue_ms Maximum queue length when subscribing.
 *
 */
class BaseServer : public BaseServerBase {
public:
	using BaseServerBase::BaseServerBase;

protected:
	void main() override;

	void setup();
	
	std::shared_ptr<const Endpoint> endpoint;

private:
	void accept_loop();
	
	virtual void spawn_proxy(int socket) = 0;

	int server = -1;
	std::shared_ptr<Timer> setup_timer;
	std::thread thread;
	
};


} // vnx

#endif /* INCLUDE_VNX_BASESERVER_H_ */
