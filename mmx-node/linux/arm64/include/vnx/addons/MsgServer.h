/*
 * MsgServer.h
 *
 *  Created on: Jan 22, 2022
 *      Author: mad
 */

#ifndef INCLUDE_VNX_ADDONS_MSGSERVER_H_
#define INCLUDE_VNX_ADDONS_MSGSERVER_H_

#include <vnx/addons/MsgServerBase.hxx>
#include <vnx/addons/DeflateInputStream.h>
#include <vnx/addons/DeflateOutputStream.h>

#include <vnx/Buffer.hpp>
#include <vnx/Input.hpp>
#include <vnx/Output.hpp>


namespace vnx {
namespace addons {

class MsgServer : public MsgServerBase {
public:
	MsgServer(const std::string& _vnx_name);

protected:
	struct peer_t {
		uint32_t msg_size = 0;
		uint64_t client = 0;
		uint64_t bytes_send = 0;
		uint64_t bytes_recv = 0;
		uint64_t write_queue_size = 0;

		vnx::Memory data;
		vnx::Buffer buffer;
		vnx::BufferInputStream in_stream;
		vnx::MemoryOutputStream out_stream;
		vnx::addons::DeflateInputStream deflate_in_stream;
		std::shared_ptr<vnx::addons::DeflateOutputStream> deflate_out_stream;
		vnx::TypeInput in;
		vnx::TypeInput deflate_in;
		vnx::TypeOutput out;
		std::shared_ptr<vnx::TypeOutput> deflate_out;

		peer_t()
			:	in_stream(&buffer), out_stream(&data), deflate_in_stream(nullptr),
				in(&in_stream), deflate_in(&deflate_in_stream), out(&out_stream) {}
	};

	bool send_to(std::shared_ptr<peer_t> peer, std::shared_ptr<const vnx::Value> msg);

	virtual std::shared_ptr<peer_t> get_peer_base(uint64_t client) const = 0;

	virtual void on_msg(uint64_t client, std::shared_ptr<const vnx::Value> msg) = 0;

private:
	void on_buffer(uint64_t client, void*& buffer, size_t& max_bytes) override;

	void on_read(uint64_t client, size_t num_bytes) override;

	void on_write(uint64_t client, size_t num_bytes) override;

};


} // addons
} // vnx

#endif /* INCLUDE_VNX_ADDONS_MSGSERVER_H_ */
