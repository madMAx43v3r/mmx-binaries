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

#ifndef INCLUDE_VNX_RECORDER_H_
#define INCLUDE_VNX_RECORDER_H_

#include <vnx/RecorderBase.hxx>
#include <vnx/RecorderStatus.hxx>
#include <vnx/ProxyClient.hxx>
#include <vnx/RecordWriter.h>


namespace vnx {

/** \brief Module to record samples to a record file.
 * 
 * Configuration options:
 * - \b filename Recording filename, use "%" as a placeholder for a date string.
 * - \b max_queue_ms Input queue lengths.
 * - \b flush_interval_ms How often to flush file.
 * - \b header_size Size of the record header in bytes, should be increased if recording a large number of topics (ie. more than 100).
 * - \b block_size Size of a block in samples.
 * - \b topics List of topics to record.
 * - \b proxy_name Name of a Proxy instance to subscribe through, optional.
 * - \b receive_tunnel A Pipe address to receive samples through, optional, used in conjunction with a Proxy.
 * 
 * Used by \ref vnxrecord.
 */
class Recorder : public RecorderBase {
public:
	Recorder(const std::string& _vnx_name = "Recorder");
	
protected:
	void init() override;
	
	void main() override;
	
	void handle(std::shared_ptr<const Sample> sample) override;
	
	void handle_resend(std::shared_ptr<const Sample> sample) override;

private:
	void flush();
	
	void print_stats();
	
private:
	std::shared_ptr<RecordWriter> writer;
	std::shared_ptr<ProxyClient> proxy;
	
	RecorderStatus info;
	
	int64_t last_output_pos = 0;
	int64_t last_sample_count = 0;
	
};


} // vnx

#endif /* INCLUDE_VNX_RECORDER_H_ */
