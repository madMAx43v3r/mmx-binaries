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

#ifndef INCLUDE_VNX_HASH64_H_
#define INCLUDE_VNX_HASH64_H_

#include <string>
#include <cstdint>

#include <vnx_base_export.h>


namespace vnx {

/// Class to hold and compute a 64-bit hash
class VNX_BASE_EXPORT Hash64 {
public:
	uint64_t value = 0;
	
	Hash64() {}
	
	Hash64(const Hash64&) = default;
	
	explicit Hash64(uint64_t hash) : value(hash) {}
	
	explicit Hash64(const std::string& name);
	
	Hash64(const void* data, size_t length);
	
	Hash64(const Hash64& A, const Hash64& B);
	
	static Hash64 rand();					///< Returns random (non-zero) hash value
	
	operator uint64_t() const {
		return value;
	}
	
	Hash64& operator=(const Hash64&) = default;
	
	bool operator==(const Hash64& other) const;
	
	bool operator!=(const Hash64& other) const;
	
	Hash64 operator^(const Hash64& other) const;		///< Computes bit-wise xor
	
	std::string to_string() const;			///< Returns decimal hash number
	
	std::string to_hex_string() const;		///< Returns hexadecimal hash number
	
};


inline bool Hash64::operator==(const Hash64& other) const {
	return value == other.value;
}

inline bool Hash64::operator!=(const Hash64& other) const {
	return value != other.value;
}

inline Hash64 Hash64::operator^(const Hash64& other) const {
	return Hash64(value ^ other.value);
}


} // vnx


namespace std {
	template<> struct hash<vnx::Hash64> {
		size_t operator()(const vnx::Hash64& x) const {
			return x.value;
		}
	};
	template<> struct hash<std::pair<vnx::Hash64, vnx::Hash64>> {
		size_t operator()(const std::pair<vnx::Hash64, vnx::Hash64>& x) const {
			return x.first.value ^ x.second.value;
		}
	};
} // std

#endif /* INCLUDE_VNX_HASH64_H_ */
