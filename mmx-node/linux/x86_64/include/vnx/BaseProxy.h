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

#ifndef INCLUDE_VNX_BASEPROXY_H_
#define INCLUDE_VNX_BASEPROXY_H_

#include <vnx/BaseProxyBase.hxx>
#include <vnx/ProxyInterfaceClient.hxx>
#include <vnx/Endpoint.hxx>
#include <vnx/TimeServer.h>
#include <vnx/Authentication.h>

#include <atomic>


namespace vnx {

/** \brief Module to connect to other processes.
 * 
 * Configuration options:
 * - \b address URL to connect to, in case given \p endpoint_ == null.
 * 		For example: "hostname" (on default port 4444), "hostname:1234" or "domain.sock".
 * - \b import_list Topics to import (ie. subscribe to) from other process.
 * - \b export_list Topics to forward to other process without them asking for it.
 * - \b forward_list Services to import (ie. forward requests to) from other process.
 * - \b tunnel_map Service tunnel map, similar to forward_list, except with a local alias (the key).
 * 		For example Hash64("other.vnx.process") => "vnx.process".
 * - \b receive_tunnel Optional tunnel pipe to forward all received samples to, instead of the local process.
 * - \b request_tunnel Optional tunnel pipe to forward all received requests to, instead of the local process.
 * - \b auto_import Flag if to automatically import all topics which are subscribed to in the local process.
 * 		(dangerous, can easily cause loops, use only if local process does not export any topics)
 * - \b time_sync Flag if to synchronize our time with the other process.
 * - \b max_queue_ms Maximum default queue length when not otherwise specified.
 * - \b max_hop_count Maximum hop count for received messages, used to defuse loops.
 */
class BaseProxy : public BaseProxyBase {
public:
	BaseProxy(const std::string &vnx_name);
	
protected:
	void init() override;
	
	void main() override;
	
	void handle(std::shared_ptr<const Sample> sample) override;

	std::shared_ptr<const Return> handle(std::shared_ptr<const Request> request) override;

	void handle_resend(std::shared_ptr<const Sample> sample) override;

	void login_async(const std::string &name, const std::string &password, const request_id_t& request_id) const override;

	void enable_import(const std::string& topic_name) override;
	
	void disable_import(const std::string& topic_name) override;
	
	void enable_export(const std::string& topic_name) override;
	
	void disable_export(const std::string& topic_name) override;
	
	void enable_forward(const std::string& service_name,
						const int32_t& max_queue_ms, const int32_t& max_queue_size) override;
	
	void disable_forward(const std::string& service_name) override;
	
	void enable_tunnel(	const Hash64& tunnel_addr,
						const int32_t& max_queue_ms, const int32_t& max_queue_size) override;
	
	void disable_tunnel(const Hash64& tunnel_addr) override;
	
	void wait_on_connect_async(const request_id_t& request_id) const override;
	
	void wait_on_disconnect_async(const request_id_t &request_id) const override;

	void on_connect() override;

	void on_disconnect() override;
	
	void on_remote_connect(const Hash64& process_id) override;
	
	void on_login(const std::string &name, const std::string &password) override;

	void on_remote_login(std::shared_ptr<const Session> remote_session) override;

	std::shared_ptr<const Session> get_session() const override;

	using Super::handle; // brings the other overloads into scope

protected:
	// to be called by read_loop() only
	std::shared_ptr<Pipe> add_return_pipe(	Hash64 src_mac, std::shared_ptr<Pipe> pipe,
											int max_queue_ms, int max_queue_size, bool reconnect);

	// to be called by read_loop() only
	void process(std::shared_ptr<Frame> frame) noexcept;
	
	// to be called by read_loop() only
	void process(std::shared_ptr<Sample> sample) noexcept;

	// to be called by read_loop() only
	void process(std::shared_ptr<Request> request, std::shared_ptr<Pipe> service_pipe) noexcept;

	// to be called by read_loop() only
	void process(std::shared_ptr<Return> return_msg) noexcept;
	
	// to be called by read_loop() only
	void process(std::shared_ptr<FlowMessage> flow_msg) noexcept;

	std::shared_ptr<const Endpoint> endpoint;
	SocketOutputStream stream_out;

	std::mutex mutex_socket;
	int socket = -1;				// only read loop modifies (after setup)
	
	mutable std::mutex mutex_session;
	std::shared_ptr<const Session> session;					// only main thread modifies
	std::shared_ptr<const Session> default_session;			// read-only (never modified after start)
	std::shared_ptr<const Session> internal_session;		// read-only (never modified after start)
	
	bool is_connected = false;				// only main thread modifies / reads
	bool never_connected = true;			// only main thread modifies / reads

	Hash64 remote_addr;
	Hash64 service_addr;
	Hash64 public_service_addr;
	Hash64 remote_process_id;
	std::shared_ptr<ProxyInterfaceClient> remote;

	std::unordered_map<std::string, ssize_t> import_table;			// [topic => counter]
	std::unordered_map<std::string, ssize_t> export_table;			// [topic => counter]
	std::unordered_map<Hash64, uint64_t> forward_table;				// [service => counter]
	std::map<Hash64, std::string> forward_names;					// [dst_mac => service name]
	std::unordered_map<Hash64, std::shared_ptr<Pipe>> request_pipes;	// [service => pipe]
	std::unordered_set<std::pair<Hash64, Hash64>> outgoing;			// (src_mac, dst_mac)
	
	std::atomic<size_t> num_frames_send {0};
	std::atomic<size_t> num_frames_recv {0};
	std::atomic<size_t> num_samples_send {0};
	std::atomic<size_t> num_samples_recv {0};
	std::atomic<size_t> num_requests_send {0};
	std::atomic<size_t> num_requests_recv {0};

private:
	void update_topics();

	void print_stats();
	
	// called by read_loop() only
	bool rewire_connection();
	
	void read_loop(std::shared_ptr<const Endpoint> endpoint);

	void close_socket() noexcept;
	
	// called by main thread
	virtual void send_outgoing(std::shared_ptr<Sample> sample) = 0;

	// called by main thread
	virtual void send_outgoing(std::shared_ptr<Request> request) = 0;

	// called by main thread upon connect
	virtual void connect_reset() {}

	// called by main thread upon disconnect
	virtual void disconnect_cleanup() {}

	// called by read_loop()
	virtual void read_socket_until_error(std::shared_ptr<Pipe> service_pipe) = 0;

	Handle<TimeServer> time_server;
	std::shared_ptr<const TopicInfoList> topic_info;
	
	std::shared_ptr<AuthenticationServer> auth_server;
	mutable std::pair<std::string, std::string> login_credentials;
	mutable vnx::request_t<std::shared_ptr<const Session>> login_request;
	mutable std::vector<vnx::request_t<Hash64>> waiting_on_connect;
	mutable std::vector<vnx::request_t<Hash64>> waiting_on_disconnect;

	std::unordered_map<std::pair<Hash64, Hash64>, uint64_t> channel_map;		// for topics [(src_mac, topic) => seq_num]

	std::mutex mutex_request_map;
	std::unordered_map<Hash64, std::map<uint64_t, std::shared_ptr<const Request>>> request_map;  // [src_mac => [request_id => Request]]

	// all below belong to read_loop()
	bool is_error = false;
	std::unordered_set<std::pair<Hash64, Hash64>> incoming;						// map of incoming connections (src_mac, dst_mac)
	std::unordered_map<Hash64, std::shared_ptr<Pipe>> return_map;				// to keep track of return pipes
	std::unordered_map<std::pair<Hash64, Hash64>, std::shared_ptr<const Sample>> recv_buffer;		// last known sample per channel

};


} // vnx

#endif /* INCLUDE_VNX_BASEPROXY_H_ */
