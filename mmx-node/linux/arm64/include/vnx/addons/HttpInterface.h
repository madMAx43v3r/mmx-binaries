/*
 * HttpInterface.h
 *
 *  Created on: Dec 21, 2021
 *      Author: mad
 */

#ifndef INCLUDE_VNX_ADDONS_HTTPINTERFACE_H_
#define INCLUDE_VNX_ADDONS_HTTPINTERFACE_H_

#include <vnx/GenericAsyncClient.h>


namespace vnx {
namespace addons {

template<typename T>
class HttpInterface : public GenericAsyncClient {
public:
	HttpInterface(T* module, const std::string& service_name)
		:	HttpInterface(module, Hash64(service_name))
	{
	}

	HttpInterface(T* module, const Hash64& service_addr)
		:	GenericAsyncClient(service_addr),
			module(module)
	{
		vnx_set_non_blocking(true);
	}

	void http_request(	std::shared_ptr<const HttpRequest> request, const std::string& sub_path,
						const request_id_t& request_id, const Hash64& session_id)
	{
		if(sub_path.empty()) {
			throw std::logic_error("path empty");
		}
		const std::string method = sub_path.substr(1);
		if(method.empty()) {
			const TypeCode* type_code = module->get_type_code();
			std::vector<std::string> methods;
			for(auto method : type_code->methods) {
				methods.push_back(method->method_name);
			}
			module->http_request_async_return(request_id, HttpResponse::from_string_json(vnx::to_string(methods)));
			return;
		}

		Object args;
		if(request->payload.size()) {
			vnx::from_string(request->payload.as_string(), args);
		}
		for(const auto& entry : request->query_params) {
			Variant value;
			vnx::from_string(entry.second, value);
			args[entry.first] = value;
		}
		vnx_set_session(session_id);

		call(method, args,
			[this, request, request_id](std::shared_ptr<const Value> result) {
				std::shared_ptr<const HttpResponse> response;
				if(auto except = std::dynamic_pointer_cast<const Exception>(result)) {
					response = HttpResponse::from_value_json_ex(except, 500);
				} else if(result->is_void()) {
					response = HttpResponse::from_status(200);
				} else {
					response = HttpResponse::from_string_json(result->get_field_by_index(0).to_string());
				}
				module->http_request_async_return(request_id, response);
			},
			[this, request_id](const vnx::exception& ex) {
				module->vnx_async_return(request_id, ex.value());
			});
	}

private:
	T* module = nullptr;

};


} // addons
} // vnx

#endif /* INCLUDE_VNX_ADDONS_HTTPINTERFACE_H_ */
