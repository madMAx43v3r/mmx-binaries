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

#ifndef INCLUDE_VNX_TOPICPTR_H_
#define INCLUDE_VNX_TOPICPTR_H_

#include <vnx/Topic.h>
#include <vnx_base_export.h>


namespace vnx {

/// Wrapper around std::shared_ptr<Topic>
class VNX_BASE_EXPORT TopicPtr : public std::shared_ptr<Topic> {
public:
	TopicPtr();
	
	TopicPtr(const char* name_);
	
	TopicPtr(const std::string& name_);
	
	TopicPtr(const std::shared_ptr<Topic>& topic_);
	
	TopicPtr& operator=(const char* name_);
	
	TopicPtr& operator=(const std::string& name_);
	
	TopicPtr& operator=(const std::shared_ptr<Topic>& topic_);
	
	static const std::string empty;		///< "null"

};


} // vnx


namespace std {
	template<> struct hash<vnx::TopicPtr> {
		size_t operator()(const vnx::TopicPtr& x) const {
			return std::hash<std::shared_ptr<vnx::Topic>>()(x);
		}
	};
} // std

#endif /* INCLUDE_VNX_TOPICPTR_H_ */
