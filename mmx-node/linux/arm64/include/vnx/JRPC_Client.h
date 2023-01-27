/*
 * JRPC_Client.h
 *
 *  Created on: Jan 20, 2021
 *      Author: jaw
 */


#ifndef VNX_JRPC_CLIENT_H_
#define VNX_JRPC_CLIENT_H_


#include <vnx/vnx.h>
#include <queue>
#include <thread>
#include <mutex>


namespace vnx{

class JRPC_Client{
public:
	JRPC_Client(std::shared_ptr<const Endpoint> endpoint_=nullptr, int socket=-1);
	JRPC_Client(int socket_);
	JRPC_Client(const std::string &address);
	~JRPC_Client();

	Variant call(const std::string &method, const Object &args);
	void call_async(const std::string &method, const Object &args);
	std::shared_ptr<JRPC_Notification> poll(int timeout_ms);

protected:
	bool do_run();

private:
	std::mutex mutex_keep_on_running;
	bool keep_on_running = true;
	std::shared_ptr<const Endpoint> endpoint;
	std::mutex mutex_socket;
	int socket;
	SocketInputStream stream_in;
	SocketOutputStream stream_out;
	istream in;
	ostream out;
	const std::string jrpc_version = "2.0";
	int jrpc_id = 0;
	std::mutex mutex_recv_queue;
	std::condition_variable cv_recv_queue;
	std::queue<std::shared_ptr<JSON_Object>> recv_queue;
	std::queue<Object> buffer_queue;
	std::thread reader_thread;

	void close_socket();
	bool reconnect();
	void read_loop();
	std::shared_ptr<JSON_Object> read_recv(int timeout_ms);
	bool is_request_object(const Object &object, JRPC_Request &request) const;
	bool is_response_object(const Object &object, JRPC_Response &response) const;
	std::shared_ptr<JRPC_Notification> object_to_request_object(const Object &object) const;
};



} // vnx

#endif /* VNX_JRPC_CLIENT_H_ */
