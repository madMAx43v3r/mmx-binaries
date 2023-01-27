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

#ifndef INCLUDE_VNX_REQUEST_ID_T_H_
#define INCLUDE_VNX_REQUEST_ID_T_H_

#include <vnx/Hash64.h>


namespace vnx {

/// Unique identifier for a Request (src_mac, request_id)
typedef std::pair<Hash64, uint64_t> request_id_t;

inline
bool operator<(const request_id_t& lhs, const request_id_t& rhs) {
	if(lhs.first == rhs.first) {
		return lhs.second < rhs.second;
	}
	return lhs.first < rhs.first;
}

inline
bool operator==(const request_id_t& lhs, const request_id_t& rhs) {
	return lhs.first == rhs.first && lhs.second == rhs.second;
}

inline
bool operator!(const request_id_t& rhs) {
	return rhs.first == Hash64() && rhs.second == 0;
}


} // vnx


namespace std {
	template<> struct hash<vnx::request_id_t> {
		size_t operator()(const vnx::request_id_t& x) const {
			return x.first ^ x.second;
		}
	};
} // std

#endif /* INCLUDE_VNX_REQUEST_ID_T_H_ */
