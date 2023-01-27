/*
 * RequestQueue.h
 *
 *  Created on: Jul 13, 2021
 *      Author: jaw
 */

#ifndef INCLUDE_VNX_REQUESTQUEUE_H_
#define INCLUDE_VNX_REQUESTQUEUE_H_

#include <vnx/RequestQueueBase.hxx>
#include <vnx/Return.hxx>

#include <map>
#include <queue>


namespace vnx {

class RequestQueue : public RequestQueueBase {
public:
	RequestQueue(const std::string& _vnx_name);

protected:
	void init() override;

	void forward(std::shared_ptr<const Request> request) override;

	void purge() override;

	void clear() override;

	bool handle(std::shared_ptr<const Return> request) override;

	using Super::handle;

private:
	// src => fake_src
	std::map<Hash64, Hash64> src_map;

	// [fake_src, id] => original request
	std::map<std::pair<Hash64, uint64_t>, std::shared_ptr<const Request>> pending;

	std::shared_ptr<const Request> active;
	std::queue<std::shared_ptr<const Request>> queue;

	void send_next();

};


} // vnx

#endif /* INCLUDE_VNX_REQUESTQUEUE_H_ */
