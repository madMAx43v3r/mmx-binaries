/*
 * SHA256.h
 *
 *  Created on: 11.01.2014
 *      Author: mad
 *  Imported on: 30.11.2020
 *      Auther: jaw
 */


#ifndef INCLUDE_VNX_SHA256_H_
#define INCLUDE_VNX_SHA256_H_

#include <string>
#include <inttypes.h>

namespace vnx {


class SHA256{
public:
	typedef uint8_t uint8;
	typedef uint32_t uint32;
	typedef uint64_t uint64;

	const static uint32 sha256_k[];
	static const unsigned int SHA224_256_BLOCK_SIZE = (512/8);
public:
	void init();
	void update(const unsigned char *message, unsigned int len);
	void final(unsigned char *digest);
	static const unsigned int DIGEST_SIZE = ( 256 / 8);

public:
	void transform(const unsigned char *message, unsigned int block_nb);
	unsigned int m_tot_len;
	unsigned int m_len;
	unsigned char m_block[2*SHA224_256_BLOCK_SIZE];
	uint32 m_h[8];
};

std::string sha256_str(const std::string &input);


} // vnx


#endif /* INCLUDE_VNX_SHA256_H_ */


