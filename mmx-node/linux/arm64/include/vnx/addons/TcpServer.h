/*
 * TcpServer.h
 *
 *  Created on: Dec 17, 2021
 *      Author: mad
 */

#ifndef INCLUDE_VNX_ADDONS_TCPSERVER_H_
#define INCLUDE_VNX_ADDONS_TCPSERVER_H_

#include <vnx/addons/TcpServerBase.hxx>
#include <vnx/TcpEndpoint.hxx>


namespace vnx {
namespace addons {

class TcpServer : public TcpServerBase {
public:
	TcpServer(const std::string& _vnx_name);

	void notify(std::shared_ptr<vnx::Pipe> pipe) override;

protected:
	void init() override;

	void main() override;

	/**
	 * Pause reading from client.
	 */
	bool pause(uint64_t client);

	/**
	 * Resume reading from client.
	 */
	bool resume(uint64_t client);

	bool disconnect(uint64_t client);

	bool send_to(uint64_t client, std::shared_ptr<vnx::Buffer> data);

	uint64_t add_client(int fd, const std::string& address);

	virtual void on_buffer(uint64_t client, void*& buffer, size_t& max_bytes) = 0;

	virtual void on_read(uint64_t client, size_t num_bytes) = 0;

	/**
	 * Notify callback that data was written to socket.
	 */
	virtual void on_write(uint64_t client, size_t num_bytes) {}

	/**
	 * Called when write is blocking.
	 */
	virtual void on_pause(uint64_t client) {}

	/**
	 * Called when writing has resumed.
	 */
	virtual void on_resume(uint64_t client) {}

	virtual void on_connect(uint64_t client, const std::string& address) = 0;

	virtual void on_disconnect(uint64_t client) = 0;

	virtual void print_stats();

private:
	enum poll_bits_e : char {
		POLL_BIT_READ = 1,
		POLL_BIT_WRITE = 2
	};

	struct state_t {
		bool is_blocked = false;
		bool do_timeout = true;
		char poll_bits = 0;
		int fd = -1;
		uint64_t id = 0;
		int64_t waiting_since = -1;				// time since waiting on connection [usec]
		std::shared_ptr<vnx::Pipe> pipe;
		std::list<std::pair<std::shared_ptr<vnx::Buffer>, size_t>> write_queue;
	};

	std::shared_ptr<state_t> find_state_by_id(uint64_t id) const;

	std::shared_ptr<state_t> find_state_by_socket(int fd) const;

	std::shared_ptr<state_t> on_connect(int fd, const std::string& address);

	void on_read(std::shared_ptr<state_t> state);

	void on_write(std::shared_ptr<state_t> state);

	void on_timeout(std::shared_ptr<state_t> state);

	void on_disconnect(std::shared_ptr<state_t> state);

	void do_poll(int timeout_ms) noexcept;

private:
	int m_socket = -1;
#ifdef _WIN32
	int m_notify_socket = -1;
#else
	int m_notify_pipe[2] = {-1, -1};
#endif

	uint64_t m_next_id = 1;
	std::shared_ptr<TcpEndpoint> endpoint;

	std::unordered_map<int, std::shared_ptr<state_t>> m_state_map;								// [fd => state]
	std::unordered_map<uint64_t, std::shared_ptr<state_t>> m_client_map;						// [id => state]

	mutable size_t m_error_counter = 0;
	mutable size_t m_refused_counter = 0;
	mutable size_t m_timeout_counter = 0;
	mutable size_t m_connect_counter = 0;

};


} // addons
} // vnx

#endif /* INCLUDE_VNX_ADDONS_TCPSERVER_H_ */
