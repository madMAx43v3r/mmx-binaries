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

#ifndef INCLUDE_VNX_TOPIC_H_
#define INCLUDE_VNX_TOPIC_H_

#include <vnx/Util.h>
#include <vnx/Node.h>
#include <vnx/Pipe.h>
#include <vnx/TopicInfo.hxx>

#include <unordered_map>


namespace vnx {

class Topic;

/** \brief Get topic instance for \p topic_name.
 * 
 * Creates a new topic instance if it does not already exist.
 */
std::shared_ptr<Topic> get_topic(const std::string& topic_name);

/** \brief Get topic instance for "<domain>.<topic>".
 * 
 * Creates a new topic if it does not exist already.
 */
std::shared_ptr<Topic> get_topic(const std::string& domain, const std::string& topic);

/** \brief Get a list of all topics.
 * 
 * @param include_domains If to include domains (ie. topics which have children)
 */
std::vector<std::shared_ptr<Topic>> get_all_topics(bool include_domains = false);

/** \brief Get a list of all sub-topics in given domain (recursive).
 * 
 * @param include_domains If to include domains (ie. topics which have children)
 *
 * @return List of all sub-topics, or given domain topic if none.
 */
std::vector<std::shared_ptr<Topic>> get_all_topics(std::shared_ptr<Topic> domain, bool include_domains = false);

/** \brief Returns new topic based on the given re-mapping \p map.
 *
 * Mappings are applied in longest match first.
 *
 * @param topic Topic to be (potentially) re-mapped.
 * @param map A mapping from source to destination topics / domains.
 * @return Re-mapped topic, or same as input in case no mapping was found.
 */
std::shared_ptr<Topic> remap_topic(std::shared_ptr<Topic> topic, const std::map<TopicPtr, TopicPtr>& map);


/** \brief Topic is used to distribute (ie. publish) samples to a number of subscribers.
 * 
 * A topic can also have sub-topics (ie. children), in which case a Sample is published on the entire sub-tree.
 */
class Topic {
public:
	Topic(const std::string& topic_name_);
	
	~Topic();
	
	Topic(const Topic& other) = delete;
	Topic& operator=(const Topic& other) = delete;
	
	/** \brief Returns domain name of this topic. (thread-safe)
	 * 
	 * Exampe: Returns "mydomain" for topic "mydomain.mytopic".
	 * Exampe: Returns "" for topic "mytopic".
	 */
	std::string get_domain_name() const;
	
	/** \brief Returns the topic's child name (thread-safe)
	 * 
	 * Example: Returns "mytopic" for topic "mydomain.mytopic".
	 * Example: Returns "mytopic" for topic "mytopic".
	 */
	std::string get_topic_name() const;
	
	/// Returns the full name of this topic. (thread-safe)
	std::string get_name() const { return topic_name; }
	
	/// Returns the CRC64 hash of this topic. (thread-safe)
	Hash64 get_hash() const { return topic_hash; }
	
	/// Returns the topic info for this topic. (thread-safe)
	TopicInfo get_info() const;
	
	/// Returns a pointer to the parent of this topic (ie. its domain). (thread-safe)
	std::shared_ptr<Topic> get_parent() const;
	
	/// Returns a list of all direct child topics. (thread-safe)
	std::vector<std::shared_ptr<Topic>> get_children() const;
	
	/// Add a subscriber to this topic. (thread-safe)
	void subscribe(std::shared_ptr<Pipe> pipe);
	
	/// Remove a subscriber from this topic. (thread-safe)
	void unsubscribe(std::shared_ptr<Pipe> pipe);
	
	/// Publish a sample on this topic. (thread-safe)
	void publish(std::shared_ptr<Sample> sample);
	
	/// Broadcast a message on this topic. (thread-safe)
	void broadcast(std::shared_ptr<const Message> msg);
	
	/// Add a child to this topic. Used internally only. (thread-safe)
	void add_child(std::shared_ptr<Topic> child);
	
	/// Remove a child from this topic. Used internally only. (thread-safe)
	void remove_child(Topic* child);
	
	/// Returns new topic with new domain name. (thread-safe)
	std::shared_ptr<Topic> remap(const std::string& new_domain) const;
	
	/** \brief Close this topic and all its children. (thread-safe)
	 * 
	 * Cannot be undone, used only for process shutdown.
	 */
	void close();
	
private:
	mutable std::mutex mutex;
	std::condition_variable condition;
	
	std::string topic_name;
	Hash64 topic_hash;
	
	bool is_closed = false;
	std::shared_ptr<Topic> parent;
	std::unordered_map<Hash64, std::shared_ptr<Topic>> children;
	
	std::list<std::shared_ptr<Pipe>> output;
	
	TopicInfo info;
	
};


} // vnx

#endif /* INCLUDE_VNX_TOPIC_H_ */
