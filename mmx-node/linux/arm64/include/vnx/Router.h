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

#ifndef VNX_ROUTER_H_
#define VNX_ROUTER_H_

#include <vnx/RouterBase.hxx>


namespace vnx {

/** \brief Module to route messages between different processes.
 * 
 * The Router employs a TCP Server, a UNIX domain Server as well as a number of Proxy modules.
 * 
 * Configuration options:
 * - \b port TCP port for the TCP Server.
 * - \b unix_path Unix domain socket path for the UNIX domain Server.
 * - \b max_queue_ms Maximum queue length when subscribing.
 * - \b import_map Map of which topics to import from which processes. (node url => list of topics)
 * - \b export_map Map of which topics to export to which processes. (node url => list of topics)
 * - \b export_list List of topics to export to every process that connects to the TCP or UNIX domain Server.
 * 
 * Used by vnxrouter.
 */
class Router : public RouterBase {
public:
	Router(const std::string& _vnx_name = "Router");
	
protected:
	void main() override;
	
};


} // vnx

#endif /* VNX_ROUTER_H_ */
