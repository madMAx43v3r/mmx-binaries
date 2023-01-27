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

#ifndef INCLUDE_VNX_TIMESERVER_H_
#define INCLUDE_VNX_TIMESERVER_H_

#include <vnx/TimeServerBase.hxx>
#include <vnx/ProcessClient.hxx>


namespace vnx {

/** \brief Module that performs time synchronization.
 * 
 * Used by Proxy to sync time with another process.
 */
class TimeServer : public TimeServerBase {
public:
	TimeServer(const std::string& _vnx_name);
	
protected:
	void main() override;
	
	void update();
	
	void print_stats();
	
private:
	std::shared_ptr<ProcessClient> client;
	TimeSync state;
	int64_t latency = 0;
	bool have_init = false;
	
};


} // vnx

#endif /* INCLUDE_VNX_TIMESERVER_H_ */
