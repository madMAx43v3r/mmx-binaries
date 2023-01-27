/*
 * DeflateOutputStream.h
 *
 *  Created on: Jun 28, 2020
 *      Author: mad
 */

#ifndef INCLUDE_VNX_ADDONS_DEFLATEOUTPUTSTREAM_H_
#define INCLUDE_VNX_ADDONS_DEFLATEOUTPUTSTREAM_H_

#include <vnx/OutputStream.h>
#include <vnx/Buffer.h>

#include <zlib.h>


namespace vnx {
namespace addons {

class DeflateOutputStream : public OutputStream {
public:
	DeflateOutputStream(Buffer* buffer, int level = 9, int chunk_size = 0);
	
	~DeflateOutputStream();
	
	void set_output(Buffer* buffer);

	void write(const void* buf, size_t len) override;
	
	void flush();
	
	void finish();

private:
	int compress_loop(int flag);
	
private:
	bool is_finished = false;
	Buffer* buffer = nullptr;
	size_t chunk_size = 262144;

	z_stream strm = {};
	
};


} // addons
} // vnx

#endif /* INCLUDE_VNX_ADDONS_DEFLATEOUTPUTSTREAM_H_ */
