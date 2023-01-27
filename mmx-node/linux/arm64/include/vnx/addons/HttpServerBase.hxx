
// AUTO GENERATED by vnxcppcodegen

#ifndef INCLUDE_vnx_addons_HttpServerBase_HXX_
#define INCLUDE_vnx_addons_HttpServerBase_HXX_

#include <vnx/addons/package.hxx>
#include <vnx/Module.h>
#include <vnx/TopicPtr.hpp>
#include <vnx/addons/HttpChunk.hxx>
#include <vnx/addons/HttpData.hxx>
#include <vnx/addons/HttpRequest.hxx>
#include <vnx/addons/HttpResponse.hxx>


namespace vnx {
namespace addons {

class VNX_ADDONS_EXPORT HttpServerBase : public ::vnx::Module {
public:
	
	::vnx::TopicPtr output_request;
	::vnx::TopicPtr output_response;
	int32_t port = 8080;
	std::string host = "localhost";
	vnx::bool_t non_blocking = true;
	vnx::bool_t show_info = false;
	vnx::bool_t show_warnings = false;
	vnx::bool_t error_payload = true;
	vnx::bool_t auto_session = false;
	vnx::bool_t enable_deflate = true;
	int32_t num_threads = 4;
	int32_t session_size = 3;
	int32_t listen_queue_size = 1000;
	int32_t stats_interval_ms = 10000;
	int32_t connection_timeout_ms = 30000;
	int64_t session_timeout = 86400;
	int64_t max_payload_size = 16777216;
	int64_t max_chunk_size = 1048576;
	int64_t min_compress_size = 4096;
	std::set<std::string> do_compress;
	std::map<std::string, std::string> components;
	std::map<std::string, std::string> charset;
	std::vector<std::pair<std::string, std::string>> add_headers;
	std::map<std::string, std::string> token_map;
	std::string default_access = "VIEWER";
	std::string cookie_policy = "SameSite=Strict;";
	std::string session_coookie_name = "hsid";
	std::string token_header_name = "x-api-token";
	std::string login_path = "/login";
	std::string logout_path = "/logout";
	std::string session_path = "/session";
	
	typedef ::vnx::Module Super;
	
	static const vnx::Hash64 VNX_TYPE_HASH;
	static const vnx::Hash64 VNX_CODE_HASH;
	
	static constexpr uint64_t VNX_TYPE_ID = 0xf05b2d0ac45a8a7bull;
	
	HttpServerBase(const std::string& _vnx_name);
	
	vnx::Hash64 get_type_hash() const override;
	std::string get_type_name() const override;
	const vnx::TypeCode* get_type_code() const override;
	
	void read(std::istream& _in) override;
	void write(std::ostream& _out) const override;
	
	template<typename T>
	void accept_generic(T& _visitor) const;
	void accept(vnx::Visitor& _visitor) const override;
	
	vnx::Object to_object() const override;
	void from_object(const vnx::Object& object) override;
	
	vnx::Variant get_field(const std::string& name) const override;
	void set_field(const std::string& name, const vnx::Variant& value) override;
	
	friend std::ostream& operator<<(std::ostream& _out, const HttpServerBase& _value);
	friend std::istream& operator>>(std::istream& _in, HttpServerBase& _value);
	
	static const vnx::TypeCode* static_get_type_code();
	static std::shared_ptr<vnx::TypeCode> static_create_type_code();
	
protected:
	using Super::handle;
	
	virtual void handle(std::shared_ptr<const ::vnx::addons::HttpChunk> _value) {}
	virtual void http_request_async(std::shared_ptr<const ::vnx::addons::HttpRequest> request, const std::string& sub_path, const vnx::request_id_t& _request_id) const = 0;
	void http_request_async_return(const vnx::request_id_t& _request_id, const std::shared_ptr<const ::vnx::addons::HttpResponse>& _ret_0) const;
	virtual void http_request_chunk_async(std::shared_ptr<const ::vnx::addons::HttpRequest> request, const std::string& sub_path, const int64_t& offset, const int64_t& max_bytes, const vnx::request_id_t& _request_id) const = 0;
	void http_request_chunk_async_return(const vnx::request_id_t& _request_id, const std::shared_ptr<const ::vnx::addons::HttpData>& _ret_0) const;
	
	void vnx_handle_switch(std::shared_ptr<const vnx::Value> _value) override;
	std::shared_ptr<vnx::Value> vnx_call_switch(std::shared_ptr<const vnx::Value> _method, const vnx::request_id_t& _request_id) override;
	
};

template<typename T>
void HttpServerBase::accept_generic(T& _visitor) const {
	_visitor.template type_begin<HttpServerBase>(31);
	_visitor.type_field("output_request", 0); _visitor.accept(output_request);
	_visitor.type_field("output_response", 1); _visitor.accept(output_response);
	_visitor.type_field("port", 2); _visitor.accept(port);
	_visitor.type_field("host", 3); _visitor.accept(host);
	_visitor.type_field("non_blocking", 4); _visitor.accept(non_blocking);
	_visitor.type_field("show_info", 5); _visitor.accept(show_info);
	_visitor.type_field("show_warnings", 6); _visitor.accept(show_warnings);
	_visitor.type_field("error_payload", 7); _visitor.accept(error_payload);
	_visitor.type_field("auto_session", 8); _visitor.accept(auto_session);
	_visitor.type_field("enable_deflate", 9); _visitor.accept(enable_deflate);
	_visitor.type_field("num_threads", 10); _visitor.accept(num_threads);
	_visitor.type_field("session_size", 11); _visitor.accept(session_size);
	_visitor.type_field("listen_queue_size", 12); _visitor.accept(listen_queue_size);
	_visitor.type_field("stats_interval_ms", 13); _visitor.accept(stats_interval_ms);
	_visitor.type_field("connection_timeout_ms", 14); _visitor.accept(connection_timeout_ms);
	_visitor.type_field("session_timeout", 15); _visitor.accept(session_timeout);
	_visitor.type_field("max_payload_size", 16); _visitor.accept(max_payload_size);
	_visitor.type_field("max_chunk_size", 17); _visitor.accept(max_chunk_size);
	_visitor.type_field("min_compress_size", 18); _visitor.accept(min_compress_size);
	_visitor.type_field("do_compress", 19); _visitor.accept(do_compress);
	_visitor.type_field("components", 20); _visitor.accept(components);
	_visitor.type_field("charset", 21); _visitor.accept(charset);
	_visitor.type_field("add_headers", 22); _visitor.accept(add_headers);
	_visitor.type_field("token_map", 23); _visitor.accept(token_map);
	_visitor.type_field("default_access", 24); _visitor.accept(default_access);
	_visitor.type_field("cookie_policy", 25); _visitor.accept(cookie_policy);
	_visitor.type_field("session_coookie_name", 26); _visitor.accept(session_coookie_name);
	_visitor.type_field("token_header_name", 27); _visitor.accept(token_header_name);
	_visitor.type_field("login_path", 28); _visitor.accept(login_path);
	_visitor.type_field("logout_path", 29); _visitor.accept(logout_path);
	_visitor.type_field("session_path", 30); _visitor.accept(session_path);
	_visitor.template type_end<HttpServerBase>(31);
}


} // namespace vnx
} // namespace addons


namespace vnx {

} // vnx

#endif // INCLUDE_vnx_addons_HttpServerBase_HXX_
