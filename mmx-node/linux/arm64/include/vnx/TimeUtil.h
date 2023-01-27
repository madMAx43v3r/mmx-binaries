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

#ifndef INCLUDE_VNX_TIME_H_
#define INCLUDE_VNX_TIME_H_

#include <vnx/TimeControl.hxx>
#include <vnx/TimeSync.hxx>


namespace vnx {

int64_t get_time_seconds();				///< Returns current virtual time in seconds
int64_t get_time_millis();				///< Returns current virtual time in milli-seconds
int64_t get_time_micros();				///< Returns current virtual time in micro-seconds

int64_t get_sync_time_micros();			///< Returns current sync time in micro-seconds

int64_t get_wall_time_seconds();		///< Returns current wall time in seconds
int64_t get_wall_time_millis();			///< Returns current wall time in milli-seconds
int64_t get_wall_time_micros();			///< Returns current wall time in micro-seconds
int64_t get_wall_time_nanos();			///< Returns current wall time in nano-seconds

/// Returns date string in format "%Y-%m-%d_%H:%M:%S"
std::string get_date_string(bool UTC = false, int64_t time_secs = -1);

/// Returns date string in given format
std::string get_date_string_ex(const std::string& format, bool UTC = false, int64_t time_secs = -1);

/// Get current time control state
TimeControl get_time_control();

void set_time_control(const TimeControl& state);

/// Get current time sync state
TimeSync get_time_sync();

void set_time_sync(const TimeSync& state);


class Timer {
public:
	/// Reset (ie. restart) the timer using \p now [usec] as the current virtual time.
	void reset(int64_t now) {
		do_reset = false;
		is_active = true;
		deadline = now + interval;
	}
	
	/// Reset (ie. restart) the timer as soon as possible.
	void reset() {
		do_reset = true;
		is_active = false;
	}

	/// Reset timer with new millisecond interval starting now.
	void set_millis(int64_t interval_ms) {
		interval = interval_ms * 1000;
		reset();
	}
	
	/// Reset timer with new microsecond interval starting now.
	void set_micros(int64_t interval_us) {
		interval = interval_us;
		reset();
	}
	
	/// Stop this timer.
	void stop() {
		do_reset = false;
		is_active = false;
	}
	
	int64_t interval = 0;			///< Timer interval in usec
	int64_t deadline = 0;			///< Timestamp in usec when it will expire next
	
	bool is_repeat = false;			///< If Timer is set on repeat
	bool is_active = false;			///< If Timer is set to be active
	bool is_one_shot = false;		///< If to delete Timer after first expired
	bool do_reset = false;			///< If to reset Timer as soon as possible
	
	std::function<void()> func;		///< User callback function
	
};


} // vnx

#endif /* INCLUDE_VNX_TIME_H_ */
