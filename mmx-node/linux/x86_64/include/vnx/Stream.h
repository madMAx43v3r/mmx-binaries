/*
 * Stream.h
 *
 *  Created on: Feb 2, 2021
 *      Author: mad
 */

#ifndef INCLUDE_VNX_STREAM_H_
#define INCLUDE_VNX_STREAM_H_

#include <vnx/Pipe.h>
#include <vnx/Frame.hxx>


namespace vnx {

/** \brief Stream is used to send vnx::Frame messages.
 *
 * Messages are sent with BLOCKING flag enabled and wrapped inside a vnx::Frame value,
 * similar to a publish with vnx::Sample, only with a Stream there is just one receiver.
 */
class Stream {
public:
	Stream(const Hash64& dst_mac_);
	
	Stream(const std::string& service);
	
	~Stream();
	
	static std::shared_ptr<Stream> create(const Hash64& dst_mac_);

	static std::shared_ptr<Stream> create(const std::string& service);

	Stream(const Stream& other) = delete;
	Stream& operator=(const Stream& other) = delete;
	
	void open();
	
	void connect(	Node* node,
					int max_queue_ms, int max_queue_size = 0,
					int priority = PIPE_PRIORITY_DEFAULT);
	
	pipe_code_e send(const Value& value, uint16_t flags = 0);
	
	pipe_code_e send(std::shared_ptr<const Value> value, uint16_t flags = 0);
	
	template<typename T>
	pipe_code_e send(std::shared_ptr<T> value, uint16_t flags = 0) {
		return send(std::shared_ptr<const Value>(value), flags);
	}
	
	void close();
	
	Hash64 get_src_mac() const {
		return src_mac;
	}
	
private:
	Hash64 src_mac;
	Hash64 dst_mac;
	
	uint64_t next_seq = 1;
	std::shared_ptr<Pipe> send_pipe;
	std::shared_ptr<Pipe> recv_pipe;
	
};


} // vnx

#endif /* INCLUDE_VNX_STREAM_H_ */
