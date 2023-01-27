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

#ifndef INCLUDE_VNX_THREAD_H
#define INCLUDE_VNX_THREAD_H

#include <vnx/Subscriber.h>
#include <vnx/Publisher.h>
#include <vnx/LogMsg.hxx>

#include <thread>


namespace vnx {

/** \brief A base class for light-weight worker threads.
 * 
 * Used in cases where a Module is too much.
 */
class Thread : protected Subscriber {
public:
	/// Creates new thread with given name
	Thread(const std::string& vnx_name_);
	
	Thread(const Thread&) = delete;
	
	Thread& operator=(const Thread&) = delete;
	
	~Thread();
	
	/// Start thread in background. Safe to call multiple times.
	void start();
	
	/// Wait for thread to finish and join, if thread is still running. (not thread-safe)
	void join();
	
	/// Trigger thread to exit and then join it, if thread is still running. (not thread-safe)
	void stop();
	
	static const int UNLIMITED = Pipe::UNLIMITED;		///< Shortcut to denote unlimited queue length

	static const int BLOCKING = Message::BLOCKING;		///< Flag to send blocking messages

	static const int ERROR = LogMsg::ERROR;
	static const int WARN = LogMsg::WARN;
	static const int INFO = LogMsg::INFO;
	static const int DEBUG = LogMsg::DEBUG;

	int vnx_log_level = INFO;			///< The display log level of this module (see LogMsg)

protected:
	std::string vnx_name;
	
	/**
	 * Will be called from inside start() to ensure initialization is done before
	 * calling thread continues.
	 */
	virtual void init() {}
	
	/**
	 * Main thread function.
	 */
	virtual void main() = 0;
	
	/** \brief Publish a copy of the value. [thread-safe]
	 *
	 * @param flags List of or'ed flags for Message, for example Message::BLOCKING
	 */
	void publish(const Value& value, TopicPtr topic, uint16_t flags = 0) const {
		publisher->publish(value.clone(), topic, flags);
	}

	/** \brief Publish the actual value directly. (zero-copy) [thread-safe]
	 *
	 * @param flags List of or'ed flags for Message, for example Message::BLOCKING
	 *
	 * WARNING: \p value may not be modified anymore after this call, since other threads have a pointer now.
	 */
	template<typename T>
	void publish(std::shared_ptr<T> value, TopicPtr topic, uint16_t flags = 0) const {
		publisher->publish(value, topic, flags);
	}

	/** \brief Publishes log output. [thread-safe]
	 *
	 * Use ERROR, WARN, INFO, DEBUG or custom level.
	 * Usage: log(level) << "..."; 		// no std::endl needed at the end
	 */
	LogPublisher log(int level) const {
		return LogPublisher(publisher, vnx_name, level, vnx_log_level);
	}

private:
	void entry();
	
private:
	std::thread thread;
	std::shared_ptr<Publisher> publisher;
	
};


} // vnx

#endif // INCLUDE_VNX_THREAD_H
