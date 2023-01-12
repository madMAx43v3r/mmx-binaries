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

#ifndef INCLUDE_VNX_SUBSCRIBER_H_
#define INCLUDE_VNX_SUBSCRIBER_H_

#include <vnx/Node.h>
#include <vnx/Topic.h>
#include <vnx/TopicPtr.h>

#include <unordered_map>


namespace vnx {

/** \brief A Node that can receive Sample%s.
 * 
 * Repeated calls to subscribe() are counted and
 * an equal number of calls to unsubscribe() are needed to actually unsubscribe.
 * A preemtive call to unsubscribe() can be used to cancel out a later subscribe().
 * 
 * The Subscriber keeps track of subscriptions. It automatically unsubscribes all topics upon exit. 
 * [NOT thread safe]
 */
class Subscriber : public Node {
public:
	Subscriber();
	
	~Subscriber();
	
protected:
	std::shared_ptr<Pipe> subscribe(TopicPtr topic, int max_queue_ms = 100, int max_queue_size = Pipe::UNLIMITED,
									int priority = PIPE_PRIORITY_DEFAULT, pipe_mode_e mode = PIPE_MODE_DEFAULT);
	
	void unsubscribe(TopicPtr topic);
	
	void unsubscribe_all();
	
	/// Returns list of topics that are subscribed
	std::vector<TopicPtr> get_topics() const;
	
	/// Unsubscribe from all topics and close Node
	void close() override;
	
private:
	struct subscription_t {
		ssize_t count = 0;
		std::shared_ptr<Pipe> pipe;
	};
	
	std::unordered_map<std::shared_ptr<Topic>, subscription_t> topic_map;
	
};


} // vnx

#endif /* INCLUDE_VNX_SUBSCRIBER_H_ */
