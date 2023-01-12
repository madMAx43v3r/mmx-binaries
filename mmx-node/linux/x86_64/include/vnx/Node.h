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

#ifndef INCLUDE_VNX_NODE_H_
#define INCLUDE_VNX_NODE_H_

#include <vnx/Pipe.h>

#include <list>
#include <atomic>


namespace vnx {

/** \brief A base class for anything that wants to communicate via Pipe.
 * 
 * It gives an interface to read messages from a number of pipes.
 * It also keeps track of and automatically closes all connected pipes on shutdown.
 */
class Node {
public:
	Node() {}
	
	virtual ~Node();
	
	Node(const Node& other) = delete;
	Node& operator=(const Node& other) = delete;
	
	/**
	 * Notify this node that a new message is available on \p pipe.
	 * Needs to be thread safe!
	 */
	virtual void notify(std::shared_ptr<Pipe> pipe);
	
	/**
	 * Trigger this node to shut down.
	 * Will shutdown all pipes, to prevent any new messages from being added to the queues.
	 * Can be overridden by derived classes but make sure to call this base version also.
	 * Needs to be thread safe!
	 */
	virtual void exit();
	
protected:
	mutable std::mutex vnx_mutex;
	
	std::atomic_bool vnx_do_run {true};			///< If this node should continue to run
	
	std::shared_ptr<Pipe> vnx_pipe;				///< Pipe through which the last message was read
	
	/**
	 * Read the next message if one is available. Does not wait.
	 * Returns 0 if no message is available.
	 */
	std::shared_ptr<const Message> read();
	
	/**
	 * Read the next message. Waits indefinitely for one to arrive.
	 * Returns 0 if shutdown is triggered.
	 */
	std::shared_ptr<const Message> read_blocking();
	
	/**
	 * Read the next message. Waits timeout_us for one to arrive (-1 = infinitely).
	 * Returns 0 in case of timeout or shutdown is triggered.
	 */
	std::shared_ptr<const Message> read_blocking(int64_t timeout_us);

	/**
	 * Close all pipes and clean up.
	 * Can be overridden by derived classes but make sure to call this base version also.
	 */
	virtual void close();
	
private:
	bool add_pipe(std::shared_ptr<Pipe> pipe);		///< attach a new pipe to this node
	
	void remove_pipe(Pipe* pipe);					///< remove a pipe from this node
	
	std::shared_ptr<Pipe> pop_next();				///< get next pipe and pop it from the queue (mutex needs to be locked)

	std::shared_ptr<const Message> pop_msg(std::shared_ptr<Pipe> pipe);		///< pops next message from pipe
	
private:
	std::condition_variable condition;
	
	std::map<int, std::queue<std::shared_ptr<Pipe>>> notify_map;	///< ranking of pipes that have messages available
	
	std::list<std::shared_ptr<Pipe>> pipe_list;			///< list of pipes that are attached to this node
	
	friend class Pipe;
	
};


} // vnx

#endif /* INCLUDE_VNX_NODE_H_ */
