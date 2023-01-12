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

#ifndef INCLUDE_VNX_RECORDWRITER_H_
#define INCLUDE_VNX_RECORDWRITER_H_

#include <vnx/Sample.hxx>
#include <vnx/RecordHeader.hxx>
#include <vnx/RecordIndex.hxx>
#include <vnx/OutputStream.h>


namespace vnx {

/** \brief Class to write a recording file.
 * 
 * Used by Recorder.
 */
class RecordWriter {
public:
	/** \brief Create a new writer.
	 * 
	 * Will truncate an already existing file.
	 * 
	 * @param header_size Maximum size in bytes of the header that is reserved at the beginning of the file.
	 * @block_size Size of each block in terms of number of samples.
	 */
	RecordWriter(const std::string& filename, uint32_t header_size = 262144, uint32_t block_size = 4096);
	
	~RecordWriter();
	
	/// Write sample to file
	void write(std::shared_ptr<const Sample> sample);
	
	/** \brief Rewrite the header and the current index and then flush the file.
	 * 
	 * Used to limit data loss in case of a crash.
	 */
	void flush();
	
	/// Close the file.
	void close();
	
	/// Get the record header.
	const RecordHeader& get_header() const;
	
	/// Get the current byte position.
	int64_t get_output_pos() const;
	
protected:
	void write_header();
	
	void write_index();
	
	void add_block();
	
private:
	struct channel_t {
		uint64_t seq_num = 0;
		int64_t output_pos = -1;
	};

	FILE* file = nullptr;
	FileOutputStream stream;
	TypeOutput out;
	
	RecordHeader header;
	int64_t header_start_pos = -1;
	
	std::shared_ptr<RecordIndex> curr_block;
	int32_t index_pos = 0;
	int64_t curr_block_pos = -1;
	int64_t last_sample_time = 0;
	
	std::unordered_map<TopicPtr, record_topic_info_t> topic_map;
	std::unordered_map<std::pair<Hash64, Hash64>, channel_t> channel_map;
	
};


} // vnx

#endif /* INCLUDE_VNX_RECORDWRITER_H_ */
