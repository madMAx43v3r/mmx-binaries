/*
 * HttpServer.h
 *
 *  Created on: Jul 8, 2020
 *      Author: mad
 */

#ifndef INCLUDE_VNX_ADDONS_HTTPSERVER_H_
#define INCLUDE_VNX_ADDONS_HTTPSERVER_H_

#include <vnx/addons/HttpServerBase.hxx>
#include <vnx/addons/HttpComponentAsyncClient.hxx>
#include <vnx/addons/DeflateOutputStream.h>

#include <vnx/ThreadPool.h>

#include <llhttp.h>


namespace vnx {
namespace addons {

class HttpServer : public HttpServerBase {
public:
	HttpServer(const std::string& _vnx_name);

protected:
	enum poll_bits_e : char {
		POLL_BIT_READ = 1,
		POLL_BIT_WRITE = 2
	};

	enum encoding_type_e : char {
		IDENTITY,
		DEFLATE
	};

	struct state_t {
		bool is_blocked = false;
		bool is_pending = false;
		bool is_chunked_reply = false;
		bool is_chunked_reply_pending = false;
		bool is_chunked_transfer = false;
		bool do_keep_alive = false;
		bool do_timeout = true;
		char poll_bits = 0;
		encoding_type_e input_encoding = IDENTITY;
		encoding_type_e output_encoding = IDENTITY;
		char buffer[16384];
		int fd = -1;
		uint32_t offset = 0;					// offset into buffer
		int64_t waiting_since = -1;				// time since waiting on connection [usec]
		size_t payload_size = 0;
		HttpServer* server = nullptr;
		llhttp_t parser = {};
		struct {
			std::string key;
			std::string value;
		} header;
		std::string sub_path;
		std::shared_ptr<vnx::Pipe> pipe;
		std::shared_ptr<vnx::Stream> stream;
		std::shared_ptr<vnx::Memory> payload;
		std::shared_ptr<HttpRequest> request;
		std::shared_ptr<const HttpResponse> response;
		std::shared_ptr<DeflateOutputStream> deflate;
		std::shared_ptr<HttpComponentAsyncClient> module;
		std::list<std::pair<std::shared_ptr<const HttpData>, size_t>> write_queue;
	};

	void notify(std::shared_ptr<vnx::Pipe> pipe) override;

	void init() override;

	void main() override;

	void handle(std::shared_ptr<const HttpChunk> value) override;

	void http_request_async(std::shared_ptr<const HttpRequest> request,
							const std::string& sub_path,
							const vnx::request_id_t& request_id) const override;

	void http_request_chunk_async(std::shared_ptr<const HttpRequest> request,
							const std::string& sub_path,
							const int64_t& offset,
							const int64_t& max_bytes,
							const vnx::request_id_t& _request_id) const override;

private:
	void process(state_t* state);

	void reply(uint64_t id, std::shared_ptr<const HttpResponse> response);

	void reply_error(uint64_t id, const vnx::exception& ex);

	std::shared_ptr<HttpSession> create_session() const;

	void add_session(std::shared_ptr<HttpSession> session) const;

	void remove_session(const std::string& http_sid) const;

	std::string get_session_cookie(std::shared_ptr<const HttpSession> session) const;

	void update();

	std::shared_ptr<state_t> find_state_by_id(uint64_t id) const;

	std::shared_ptr<state_t> find_state_by_socket(int fd) const;

	void on_connect(int fd);

	void on_request(std::shared_ptr<state_t> state);

	void on_resume(std::shared_ptr<state_t> state);

	void on_parse(std::shared_ptr<state_t> state);

	void on_read(std::shared_ptr<state_t> state);

	void on_write(std::shared_ptr<state_t> state);

	void on_deflate(std::shared_ptr<state_t> state);

	void on_finish(std::shared_ptr<state_t> state);

	void on_timeout(std::shared_ptr<state_t> state);

	void on_disconnect(std::shared_ptr<state_t> state);

	void on_write_data(uint64_t id, std::shared_ptr<const HttpData> chunk, bool encode = true);

	void do_write_data(std::shared_ptr<state_t> state, std::shared_ptr<const HttpData> chunk, bool encode = true);

	void on_write_error(uint64_t id, const vnx::exception& ex);

	void do_poll(int timeout_ms) noexcept;

	void deflate_write_task(uint64_t id,
							std::shared_ptr<DeflateOutputStream> stream,
							std::shared_ptr<const HttpData> chunk) noexcept;

	int set_socket_nonblocking(int fd);

	static std::string get_socket_error_text();

	static int on_url(llhttp_t* parser, const char* at, size_t length);
	static int on_header_field(llhttp_t* parser, const char* at, size_t length);
	static int on_header_field_complete(llhttp_t* parser);
	static int on_header_value(llhttp_t* parser, const char* at, size_t length);
	static int on_header_value_complete(llhttp_t* parser);
	static int on_headers_complete(llhttp_t* parser);
	static int on_body(llhttp_t* parser, const char* at, size_t length);
	static int on_message_complete(llhttp_t* parser);

private:
	int m_socket = -1;
#ifdef _WIN32
	int m_notify_socket = -1;
#else
	int m_notify_pipe[2] = {-1, -1};
#endif

	uint64_t m_next_id = 1;
	llhttp_settings_t m_settings = {};

	std::unordered_map<int, std::shared_ptr<state_t>> m_state_map;								// [fd => state]
	std::unordered_map<uint64_t, std::shared_ptr<state_t>> m_request_map;						// [request id => state]
	std::map<std::string, std::shared_ptr<HttpComponentAsyncClient>> m_client_map;				// [url path => clients]
	std::vector<std::shared_ptr<vnx::ThreadPool>> m_threads;

	std::shared_ptr<const HttpSession> m_default_session;
	std::map<std::string, std::shared_ptr<const HttpSession>> m_token_sessions;
	mutable std::unordered_map<std::string, std::shared_ptr<const HttpSession>> m_session_map;	// [http session id => session]
	mutable std::multimap<int64_t, std::string> m_session_timeout_queue;						// [deadline => http session id]

	mutable size_t m_error_counter = 0;
	mutable size_t m_timeout_counter = 0;
	mutable size_t m_request_counter = 0;
	mutable size_t m_connect_counter = 0;
	mutable std::map<int, size_t> m_error_map;

};


} // addons
} // vnx

#endif /* INCLUDE_VNX_ADDONS_HTTPSERVER_H_ */
