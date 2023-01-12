/*
 * Gateway.h
 *
 *  Created on: Jul 12, 2021
 *      Author: mad
 */

#ifndef INCLUDE_VNX_GATEWAY_H_
#define INCLUDE_VNX_GATEWAY_H_

#include <vnx/GatewayBase.hxx>


namespace vnx {

class Gateway : public GatewayBase {
public:
	Gateway(const std::string& _vnx_name);

protected:
	void init() override;

	void forward(std::shared_ptr<const Request> request) override;

};


} // vnx

#endif /* INCLUDE_VNX_GATEWAY_H_ */
