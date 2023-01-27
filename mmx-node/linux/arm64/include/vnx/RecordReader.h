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

#ifndef INCLUDE_VNX_RECORDREADER_H_
#define INCLUDE_VNX_RECORDREADER_H_

#include <vnx/Sample.hxx>
#include <vnx/RecordHeader.hxx>
#include <vnx/RecordIndex.hxx>
#include <vnx/InputStream.h>


namespace vnx {

/** \brief Class to read from a recording file.
 * 
 * Used by MultiRecordReader and Player.
 */
class RecordReader {
public:
	/// Open given file
	RecordReader(const std::string& filename_);
	
	~RecordReader();
	
	/// Reset to the beginning.
	void reset();
	
	/// Close file. Cannot be undone.
	void close();
	
	/// Read the next sample and return it.
	std::shared_ptr<const Sample> read_next();
	
	/// Read the previous sample and return it.
	std::shared_ptr<const Sample> read_prev();
	
	/// Seek by a relative number of samples.
	void seek_by_count(int64_t num_samples);
	
	/// Seek by a relative amount of time in micro-seconds.
	void seek_by_time(int64_t num_micros);
	
	/// Seek to an absolute point in time given as a micro-second timestamp.
	void seek_to_time(int64_t time_micros);
	
	/// Seek to an absolute position, from 0.0 (beginning) to 1.0 (end).
	void seek_to_position(double position);
	
	/// Get the timestamp of the current sample.
	int64_t get_time_micros() const;
	
	/// The the current byte position.
	int64_t get_input_pos() const;
	
	/// Get the current sample.
	std::shared_ptr<Sample> get_sample() const;
	
	/// Get the record header containing statistics, etc.
	std::shared_ptr<const RecordHeader> get_header() const;
	
	/// Returns true if current sample is the first.
	bool begin_of_file() const;
	
	/// Returns true if current sample is the last.
	bool end_of_file() const;
	
protected:
	bool seek_next();
	
	bool seek_prev();
	
	void fix_index_pos();
	
	void seek_and_read_sample();
	
	void read_sample();
	
	void read_index();
	
private:
	FILE* file = 0;
	std::string filename;
	
	FileInputStream stream;
	TypeInput in;
	
	std::shared_ptr<RecordHeader> header;
	std::shared_ptr<RecordIndex> curr_block;
	std::shared_ptr<Sample> curr_sample;
	int32_t index_pos = -1;
	
	bool is_begin_of_file = false;
	bool is_end_of_file = false;
	
};


} // vnx

#endif /* INCLUDE_VNX_RECORDREADER_H_ */
