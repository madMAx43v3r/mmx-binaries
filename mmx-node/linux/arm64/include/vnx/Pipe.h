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

#ifndef INCLUDE_VNX_PIPE_H_
#define INCLUDE_VNX_PIPE_H_

#include <vnx/Message.hxx>

#include <queue>
#include <mutex>
#include <atomic>
#include <condition_variable>


namespace vnx {

class Node;
class Pipe;

enum {
	PIPE_PRIORITY_DEFAULT = 0,
	PIPE_PRIORITY_LOW = 10,
	PIPE_PRIORITY_HIGH = -10,
};

enum pipe_mode_e {
	PIPE_MODE_DEFAULT = 0,	// drop new samples in case queue is full
	PIPE_MODE_LATEST,		// drop oldest samples in case queue is full
};

enum pipe_code_e {
	PIPE_OK = 0,
	PIPE_PAUSED = 1,
	PIPE_CLOSED = 2,
	PIPE_SHUTDOWN = 3,
	PIPE_OVERFLOW = 4,
	PIPE_INVALID = 5,
	PIPE_NOT_FOUND = 6
};

/// Open a new pipe known as \p service_name to Node \p node.
std::shared_ptr<Pipe> open_pipe(const std::string& service_name, Node* node,
								int max_queue_ms, int max_queue_size = 0,
								int priority = PIPE_PRIORITY_DEFAULT,
								pipe_mode_e mode = PIPE_MODE_DEFAULT);

/// Open a new pipe known by \p mac_addr to Node \p node.
std::shared_ptr<Pipe> open_pipe(Hash64 mac_addr, Node* node,
								int max_queue_ms, int max_queue_size = 0,
								int priority = PIPE_PRIORITY_DEFAULT,
								pipe_mode_e mode = PIPE_MODE_DEFAULT);

/** \brief Get a pipe to a service called \p service_name.
 * 
 * Returns null if service does not exist.
 */
std::shared_ptr<Pipe> get_pipe(const std::string& service_name);

/** \brief Get a pipe to a node known by \p mac_addr.
 * 
 * Returns null if node does not exist.
 */
std::shared_ptr<Pipe> get_pipe(Hash64 mac_addr);

/** \brief Connect \p peer to \p pipe.
 * 
 * Adds \p peer to \p pipe as a peer, such that \p peer gets notified if \p pipe closes.
 * Returns false in case of failure.
 */
pipe_code_e connect(std::shared_ptr<Pipe> pipe, std::shared_ptr<Pipe> peer);

/** \brief Disconnect peer from \p pipe.
 * 
 * Returns false in case of failure.
 */
pipe_code_e disconnect(std::shared_ptr<Pipe> pipe, Hash64 peer_mac);

/** \brief Send a message to service \p service_name.
 * 
 * Will drop the message if service does not exist.
 */
pipe_code_e send_msg(const std::string& service_name, std::shared_ptr<const Message> msg);

/** \brief Send a message to a node known by \p mac_addr.
 * 
 * Will drop the message if node does not exist.
 */
pipe_code_e send_msg(Hash64 mac_addr, std::shared_ptr<const Message> msg);

/// Send a message through a pipe.
pipe_code_e send_msg(std::shared_ptr<Pipe> pipe, std::shared_ptr<const Message> msg);

/** \brief Send a message through a pipe.
 * 
 * Applies given flags before sending: msg->flags |= flags;
 */
pipe_code_e send_msg(std::shared_ptr<Pipe> pipe, std::shared_ptr<Message> msg, uint16_t flags);

/** \brief Sends a FlowMessage OPEN to \p target using the given \p flags, with \p source as the source.
 *
 * Used to punch holes, such that messages sent from the other side get routed back to \p source.
 * Implicitly connects \p target to \p source such that \p target gets notified if \p source closes.
 */
pipe_code_e open_flow(std::shared_ptr<Pipe> target, std::shared_ptr<Pipe> source, uint16_t flags = 0);

/// Connect a pipe to a node.
void connect(	std::shared_ptr<Pipe> pipe, Node* node, int max_queue_ms, int max_queue_size = 0,
				int priority = PIPE_PRIORITY_DEFAULT, pipe_mode_e mode = PIPE_MODE_DEFAULT);

/// Unregister service known by \p service_name.
void remove_pipe(const std::string& service_name);

/// Unregister node known by \p mac_addr.
void remove_pipe(Hash64 mac_addr);

/// Close the pipe going to service \p service_name.
void close_pipe(const std::string& service_name);

/// Close the pipe going to node known by \p mac_addr.
void close_pipe(Hash64 mac_addr);

/** \brief Trigger all nodes listening to any public pipe to exit.
 * 
 * Used by the shutdown procedure.
 */
void shutdown_pipes();

/// Returns a string describing the error code
std::string get_error_string(const pipe_code_e& code);


/** \brief Pipe is used to deliver messages to a Node.
 * 
 * The Pipe is one-directional and maintains a queue of messages internally.
 * The maximum queue size is specified in terms of maximum latency in milli-seconds.
 * A maximum queue length of 0 means unlimited.
 * 
 * The Pipe is used to handle any situation without segfaulting.
 * For example: - Sending a message to a Node that does not exist anymore.
 * 				- Returning error messages to Clients for Requests that got left over in the queue.
 */
class Pipe {
public:
	static const int UNLIMITED = 0;		///< Denotes unlimited queue length (max_queue_ms == 0, max_queue_size == 0)
	
	/// Create a private no-name pipe.
	Pipe();
	
	/// Internal use only
	explicit Pipe(Hash64 mac_addr);

	~Pipe();
	
	Pipe(const Pipe& other) = delete;
	Pipe& operator=(const Pipe& other) = delete;
	
	/// Create a private no-name pipe.
	static std::shared_ptr<Pipe> create();
	
	/** \brief Create a public pipe which is known inside this process by \p mac_addr.
	 *
	 * Used for services and tunnels to receive requests and messages.
	 */
	static std::shared_ptr<Pipe> create(Hash64 mac_addr);

	/// Connect a Pipe to a Node. (thread-safe)
	friend void connect(std::shared_ptr<Pipe> pipe, Node* node, int max_queue_ms, int max_queue_size, int priority, pipe_mode_e mode);
	
	/// Connect another Pipe (peer) to this Pipe, which will be notified if this pipe is closed. (thread-safe)
	friend pipe_code_e connect(std::shared_ptr<Pipe> pipe, std::shared_ptr<Pipe> peer);
	
	/// Disconnect another Pipe from this Pipe. (thread-safe)
	friend pipe_code_e disconnect(std::shared_ptr<Pipe> pipe, Hash64 peer_mac);
	
	/// Send a message through a Pipe. (thread-safe)
	friend pipe_code_e send_msg(std::shared_ptr<Pipe> pipe, std::shared_ptr<const Message> msg);
	
	/** \brief Get the next available message.
	 * 
	 * Returns 0 if no message available. (thread-safe)
	 */
	std::shared_ptr<const Message> pop();
	
	/// Temporarily prevent new messages from being pushed into the pipe, by blocking in push(). (thread-safe)
	void pause();
	
	/// Resume normal operation after pause() was called. (thread-safe)
	void resume();
	
	/// Prevent new messages from being pushed into the pipe, push() will return false. (thread-safe)
	void shutdown();

	/// Close this pipe. (thread-safe)
	void close();
	
	/** \brief Get a pointer to the node which is connected to this pipe. (thread-safe)
	 * 
	 * Only for identification purposes, the node could be deleted at any point in time!
	 */
	Node* get_node() const;
	
	/// Returns mac address of this pipe. (thread-safe)
	Hash64 get_mac_addr() const;
	
	/// Returns if pipe is currently paused. (thread-safe)
	bool is_paused() const;
	
	/// Returns true if pipe is closed. (thread-safe)
	bool is_closed() const;

private:
	void connect(std::shared_ptr<Pipe> self, Node* node, int max_queue_ms, int max_queue_size, int priority, pipe_mode_e mode);
	
	pipe_code_e connect(std::shared_ptr<Pipe> peer);
	
	pipe_code_e disconnect(Hash64 peer_mac);
	
	/// Push a new message onto the queue. Used exclusively by send_msg(). (thread-safe)
	pipe_code_e push(std::shared_ptr<Pipe> self, std::shared_ptr<const Message> msg);
	
private:
	mutable std::mutex mutex;
	mutable std::condition_variable condition;
	
	Node* node = nullptr;
	Hash64 mac_addr;

	bool is_paused_ = false;
	bool is_shutdown_ = false;
	
	int priority = 0;
	pipe_mode_e mode = PIPE_MODE_DEFAULT;

	int64_t max_queue_us = 0;
	int64_t max_queue_size = 0;
	std::queue<std::pair<std::shared_ptr<const Message>, int64_t>> queue;
	
	std::map<Hash64, std::shared_ptr<Pipe>> peer_map;
	
	friend class Node;
	
};


} // vnx

#endif /* INCLUDE_VNX_PIPE_H_ */
