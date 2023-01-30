
// AUTO GENERATED by vnxcppcodegen

#ifndef INCLUDE_vnx_GatewayInterface_ASYNC_CLIENT_HXX_
#define INCLUDE_vnx_GatewayInterface_ASYNC_CLIENT_HXX_

#include <vnx/AsyncClient.h>
#include <vnx/Request.hxx>


namespace vnx {

class GatewayInterfaceAsyncClient : public vnx::AsyncClient {
public:
	GatewayInterfaceAsyncClient(const std::string& service_name);
	
	GatewayInterfaceAsyncClient(vnx::Hash64 service_addr);
	
	uint64_t forward(std::shared_ptr<const ::vnx::Request> request = nullptr, 
			const std::function<void()>& _callback = std::function<void()>(),
			const std::function<void(const vnx::exception&)>& _error_callback = std::function<void(const vnx::exception&)>());
	
protected:
	int32_t vnx_purge_request(uint64_t _request_id, const vnx::exception& _ex) override;
	
	int32_t vnx_callback_switch(uint64_t _request_id, std::shared_ptr<const vnx::Value> _value) override;
	
private:
	std::unordered_map<uint64_t, std::pair<std::function<void()>, std::function<void(const vnx::exception&)>>> vnx_queue_forward;
	
};


} // namespace vnx

#endif // INCLUDE_vnx_GatewayInterface_ASYNC_CLIENT_HXX_