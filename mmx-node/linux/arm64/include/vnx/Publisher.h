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

#ifndef INCLUDE_VNX_PUBLISHER_H_
#define INCLUDE_VNX_PUBLISHER_H_

#include <vnx/Topic.h>
#include <vnx/TopicPtr.h>
#include <vnx/Util.h>
#include <vnx/Sample.hxx>

#include <mutex>
#include <atomic>
#include <sstream>


namespace vnx {

/** \brief Class used to publish Sample%s.
 * 
 * Each Publisher has its own unique and randomly created "src_mac" (ie. identity).
 * Together with the sequence number a sample can be uniquely identified.
 * 
 * The Publisher automatically attaches a sequence number to each sample published.
 * It keeps a map of current sequence numbers, one for each topic.
 */
class Publisher {
public:
	Publisher();
	
	/// Destructor is not thread-safe!
	~Publisher();

	Publisher(const Publisher& other) = delete;
	Publisher& operator=(const Publisher& other) = delete;
	
	/** \brief Publish a copy of the value. [thread-safe]
	 * 
	 * @param flags List of or'ed flags for Message, for example Message::BLOCKING
	 */
	void publish(const Value& value, TopicPtr topic, uint16_t flags = 0);
	
	/** \brief Publish the actual value directly. (zero-copy) [thread-safe]
	 * 
	 * @param flags List of or'ed flags for Message, for example Message::BLOCKING
	 * 
	 * WARNING: \p value may not be modified anymore after this call, since other threads have a pointer now.
	 */
	template<typename T>
	void publish(std::shared_ptr<T> value, TopicPtr topic, uint16_t flags = 0) {
		publish(std::shared_ptr<const Value>(value), topic, flags);
	}
	
	/** \brief Publish the actual value directly. (zero-copy) [thread-safe]
	 * 
	 * @param flags List of or'ed flags for Message, for example Message::BLOCKING
	 * 
	 * WARNING: \p value may not be modified anymore after this call, since other threads have a pointer now.
	 */
	void publish(std::shared_ptr<const Value> value, TopicPtr topic, uint16_t flags = 0);
	
	/// Returns source identity
	Hash64 get_src_mac() const {
		return src_mac;
	}
	
	/// Returns list of topics that have been published so far
	std::vector<TopicPtr> get_topics() const;
	
	/// Returns true if this publisher is still open.
	bool is_open() const;

	/// Prevents any further publishes
	void close();

private:
	mutable std::mutex mutex;
	const Hash64 src_mac;
	std::atomic_bool is_open_ {true};
	std::unordered_map<std::shared_ptr<Topic>, uint64_t> topic_map;
	
};


/** \brief Will publish a LogMsg upon destruction of the object.
 * 
 * Used internally by log() functions.
 */
class LogPublisher : public std::ostringstream {
public:
	LogPublisher(const LogPublisher& other);

	LogPublisher(std::shared_ptr<Publisher> publisher, const std::string& module, int level, int display_level);
	
	~LogPublisher();
	
	std::ostringstream& out;
	
private:
	std::shared_ptr<Publisher> publisher;
	std::string module;
	int level = 0;
	int display_level = 0;
	
};

LogPublisher log_error();		///< Log a ERROR message
LogPublisher log_warn();		///< Log a WARN message
LogPublisher log_info();		///< Log a INFO message
LogPublisher log_debug();		///< Log a DEBUG message


} // vnx

#endif /* INCLUDE_VNX_PUBLISHER_H_ */
