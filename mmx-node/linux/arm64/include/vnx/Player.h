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

#ifndef INCLUDE_VNX_PLAYER_H_
#define INCLUDE_VNX_PLAYER_H_

#include <vnx/PlayerBase.hxx>
#include <vnx/TimeControl.hxx>
#include <vnx/PlayerStatus.hxx>
#include <vnx/MultiRecordReader.h>


namespace vnx {

/** \brief Module to play back a or multiple recording file/s.
 * 
 * Used by \ref vnxplay.
 */
class Player : public PlayerBase {
public:
	Player(const std::string& _vnx_name = "Player");
	
protected:
	void init() override;
	
	void main() override;
	
	RecordHeader get_info() const override;
	
	void play() override;
	
	void pause() override;
	
	void toggle() override;
	
	void stop() override;
	
	void seek_by_count(const int64_t& delta_count) override;
	
	void seek_by_time(const int64_t& delta_us) override;
	
	void seek_to_position(const float64_t& position) override;
	
	void seek_to_time(const int64_t& time_us) override;
	
	void set_speed(const float64_t& speed) override;
	
private:
	void open();
	
	void process();
	
	void trigger();
	
	void update();
	
	void update_stats(bool force = true);
	
private:
	std::shared_ptr<MultiRecordReader> reader;
	std::shared_ptr<const RecordHeader> header;
	std::shared_ptr<Sample> sample;
	
	std::shared_ptr<Timer> timer;
	TimeControl control;
	PlayerStatus status;
	
	int64_t sample_count = 0;
	int64_t last_sample_count = 0;
	int64_t last_input_pos = 0;
	
};


} // vnx

#endif /* INCLUDE_VNX_PLAYER_H_ */
