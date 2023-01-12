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

#ifndef INCLUDE_VNX_MULTIRECORDREADER_H_
#define INCLUDE_VNX_MULTIRECORDREADER_H_

#include <vnx/RecordReader.h>


namespace vnx {

/** \brief A wrapper around multiple RecordReaders.
 * 
 * MultiRecordReader is used to read from multiple record files as one virtual file.
 */
class MultiRecordReader {
public:
	/**
	 * Creates new reader for given set of files.
	 */
	MultiRecordReader(const std::vector<std::string>& filenames_);
	
	~MultiRecordReader();
	
	/**
	 * Reset to the beginning.
	 */
	void reset();
	
	/**
	 * Close all files. Cannot be undone.
	 */
	void close();
	
	/**
	 * Read the next sample and return it.
	 */
	std::shared_ptr<const Sample> read_next();
	
	/**
	 * Read the previous sample and return it.
	 */
	std::shared_ptr<const Sample> read_prev();
	
	/**
	 * Seek by a relative number of samples.
	 */
	void seek_by_count(int64_t num_samples);
	
	/**
	 * Seek by a relative amount of time in micro-seconds.
	 */
	void seek_by_time(int64_t num_micros);
	
	/**
	 * Seek to an absolute point in time given as a micro-second timestamp.
	 */
	void seek_to_time(int64_t time_micros);
	
	/**
	 * Seek to an absolute position, from 0.0 (beginning) to 1.0 (end).
	 */
	void seek_to_position(double position);
	
	/**
	 * Get the timestamp of the current sample.
	 */
	int64_t get_time_micros() const;
	
	/**
	 * The the current byte position.
	 */
	int64_t get_input_pos() const;
	
	/**
	 * Get the current sample.
	 */
	std::shared_ptr<Sample> get_sample() const;
	
	/**
	 * Get the virtual record header containing statistics, etc.
	 */
	std::shared_ptr<const RecordHeader> get_header() const;
	
private:
	std::vector<std::string> filenames;
	std::vector<std::shared_ptr<RecordReader>> readers;
	
	std::shared_ptr<RecordHeader> header;
	std::shared_ptr<RecordReader> curr_file = 0;
	std::shared_ptr<Sample> curr_sample;
	int64_t curr_time = 0;
	
};


} // vnx

#endif /* INCLUDE_VNX_MULTIRECORDREADER_H_ */
