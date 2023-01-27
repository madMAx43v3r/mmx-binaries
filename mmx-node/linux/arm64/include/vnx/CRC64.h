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

#ifndef INCLUDE_VNX_CRC64_H_
#define INCLUDE_VNX_CRC64_H_

#include <stdint.h>
#include <string>
#include <cstring>


namespace vnx {

/// Class to compute 64-bit CRC
/// algorithm: CRC-64/XZ (alias CRC-64/GO-ECMA)
/// see: https://reveng.sourceforge.io/crc-catalogue/17plus.htm#crc.cat-bits.64
class CRC64 {
public:
	CRC64() {
		crc = -1;
	}
	
	CRC64(uint64_t init) {
		crc = init;
	}
	
	template<typename T>
	void update(const T& value) {
		update((const char*)&value, sizeof(value));
	}
	
	void update(const char* str) {
		update(str, ::strlen(str));
	}
	
	void update(const std::string& str) {
		update(str.c_str(), str.size());
	}
	
	void update(const char* buf, size_t len) {
		for(size_t i = 0; i < len; ++i) {
			crc = table[(buf[i] ^ crc) & 0xFF] ^ (crc >> 8);
		}
	}
	
	uint64_t get() const {
		return ~crc;
	}
	
private:
	uint64_t crc;
	
	static const uint64_t poly;
	static const uint64_t table[256];
	
};


/// Computes CRC64 of null-terminated string
uint64_t hash64(const char* str);

/// Computes CRC64 of string
uint64_t hash64(const std::string& str);

/// Computes CRC64 of binary data given by \p buf of size \p len bytes
uint64_t hash64(const char* buf, size_t len);


} // vnx

#endif /* INCLUDE_VNX_CRC64_H_ */
