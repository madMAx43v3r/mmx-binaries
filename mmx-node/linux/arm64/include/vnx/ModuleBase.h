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

#ifndef INCLUDE_VNX_MODULEBASE_H_
#define INCLUDE_VNX_MODULEBASE_H_

#include <vnx/TimeUtil.h>
#include <vnx/Task.h>
#include <vnx/Stream.h>
#include <vnx/Publisher.h>
#include <vnx/Subscriber.h>
#include <vnx/AsyncClient.h>
#include <vnx/request_id_t.h>
#include <vnx/LogMsg.hxx>
#include <vnx/FlowMessage.hxx>
#include <vnx/ModuleInfo.hxx>
#include <vnx/Frame.hxx>

#include <sstream>
#include <functional>


namespace vnx {

class ModuleBase : protected Subscriber {
public:
	ModuleBase(const std::string& name);

	~ModuleBase();

	typedef Subscriber Super;

	static const int UNLIMITED = Pipe::UNLIMITED;				///< Shortcut to denote unlimited queue length

	static const int PRIORITY_DEFAULT = vnx::PIPE_PRIORITY_DEFAULT;
	static const int PRIORITY_HIGH = vnx::PIPE_PRIORITY_HIGH;
	static const int PRIORITY_LOW = vnx::PIPE_PRIORITY_LOW;

	static const pipe_mode_e MODE_DEFAULT = vnx::PIPE_MODE_DEFAULT;
	static const pipe_mode_e MODE_LATEST = vnx::PIPE_MODE_LATEST;

	static const int BLOCKING = Message::BLOCKING;				///< Flag to send blocking messages
	static const int NON_BLOCKING = Message::NON_BLOCKING;		///< Flag to send non_blocking messages
	static const int RESEND = Message::RESEND;					///< Flag to resend same message again (same sequence number)

	static const int ERROR = LogMsg::ERROR;
	static const int WARN = LogMsg::WARN;
	static const int INFO = LogMsg::INFO;
	static const int DEBUG = LogMsg::DEBUG;

	virtual Hash64 get_type_hash() const = 0;
	virtual std::string get_type_name() const = 0;
	virtual const TypeCode* get_type_code() const = 0;

	virtual void read(std::istream& _in) = 0;
	virtual void write(std::ostream& _out) const = 0;
	virtual void accept(vnx::Visitor& _visitor) const = 0;

	virtual Object to_object() const = 0;
	virtual void from_object(const Object& _object) = 0;

	virtual Variant get_field(const std::string& _name) const = 0;
	virtual void set_field(const std::string& _name, const Variant& _value) = 0;

	friend std::ostream& operator<<(std::ostream& _out, const ModuleBase& _value);
	friend std::istream& operator>>(std::istream& _in, ModuleBase& _value);

	Node* vnx_get_node();						///< Returns a pointer to vnx::Node sub-class

	Hash64 vnx_get_id() const;					///< Returns unique module id

	std::string vnx_get_name() const;			///< Returns the module name (Module::vnx_name)

	Object vnx_get_config_object() const;		///< Returns entire config

	Variant vnx_get_config(const std::string& name);		///< Returns current config value

	void vnx_set_config_object(const Object& config);		///< Sets multiple config values

	void vnx_set_config(const std::string& name, const Variant& value);		///< Sets a new config value

	TypeCode vnx_get_type_code() const;			///< Returns the module's type code

	std::shared_ptr<const ModuleInfo> vnx_get_module_info() const;			///< Returns current module info

	bool vnx_virtual_time = true;				///< If to use virtual time for timers
	bool vnx_clean_exit = false;				///< If to process all remaining messages on shutdown
	bool vnx_auto_shutdown = true;				///< If to close Module automatically on shutdown
	bool vnx_auto_decompress = true;			///< If to automatically decompress Sample values

	int vnx_log_level = INFO;					///< The display log level of this module (see LogMsg)
	int vnx_task_priority = PRIORITY_DEFAULT;	///< Priority for task pipe (see add_task())
	int vnx_default_queue_ms = 100;				///< Maximum queue length for default pipe (module_id)
	int vnx_default_queue_size = 1000;			///< Maximum queue size for default pipe (module_id)

	int64_t vnx_time_slice_us = 10000;			///< Maximum time to spend processing messages in a loop [us]
	int64_t vnx_heartbeat_interval_ms = 1000;	///< Interval at which to publish ModuleInfo on vnx.module_info [ms]

protected:
	const std::string vnx_name;			///< Name of the module

	std::shared_ptr<const Frame> vnx_frame;			///< Current Frame being processed (null otherwise)
	
	std::shared_ptr<const Sample> vnx_sample;		///< Current Sample being processed (null otherwise)

	std::shared_ptr<const Request> vnx_request;		///< Current Request being processed (null otherwise)

	std::shared_ptr<const Return> vnx_return;		///< Current Return being processed (null otherwise)

	std::shared_ptr<const Task> vnx_task;			///< Current Task being processed (null otherwise)

	std::map<Hash64, std::shared_ptr<const Endpoint>> vnx_remotes;		///< Map of connected processes (process id => endpoint)

	/** \brief Publish a copy of the value. [thread-safe]
	 *
	 * @param flags List of or'ed flags for Message, for example Message::BLOCKING
	 */
	void publish(const Value& value, TopicPtr topic, uint16_t flags = 0) const {
		publisher->publish(value.clone(), topic, flags);
	}

	/** \brief Publish the actual value directly. (zero-copy) [thread-safe]
	 *
	 * @param flags List of or'ed flags for Message, for example Message::BLOCKING
	 *
	 * WARNING: \p value may not be modified anymore after this call, since other threads have a pointer now.
	 */
	template<typename T>
	void publish(std::shared_ptr<T> value, TopicPtr topic, uint16_t flags = 0) const {
		publisher->publish(value, topic, flags);
	}

	/** \brief Adds a task to this modules message queue. [thread-safe]
	 *
	 * The task will be processed in the main() loop like an incoming message (see vnx_task_priority).
	 * Returns false in case of shutdown. Will never block (unlimited queue).
	 */
	bool add_task(const std::function<void()>& func) const;

	/** \brief Publishes log output. [thread-safe]
	 *
	 * Use ERROR, WARN, INFO, DEBUG or custom level.
	 * Usage: log(level) << "..."; 		// no std::endl needed at the end
	 */
	LogPublisher log(int level) const {
		return LogPublisher(publisher, vnx_name, level, vnx_log_level);
	}

	/** \brief Create a new timer.
	 *
	 * The created timer is not started/activated by this call.
	 * Can be started manually using Timer::reset() or Timer::set_millis() / Timer::set_micros().
	 * Can be turned into a repeating timer by setting Timer::is_repeat = true;
	 * Usage: add_timer(std::bind(&Class::function, this));
	 */
	std::shared_ptr<Timer> add_timer(const std::function<void()>& func);

	/** \brief Creates a temporary timer, for one time use.
	 *
	 * The created timer is already started/activated.
	 * Can be stopped using Timer::stop().
	 * Can be restarted using Timer::reset() or Timer::set_millis() / Timer::set_micros().
	 * Usage: set_timeout_micros(interval_us, std::bind(&Class::function, this));
	 */
	std::weak_ptr<Timer> set_timeout_micros(int64_t interval_us, const std::function<void()>& func);

	/** \brief Creates a temporary timer, for one time use.
	 *
	 * The created timer is already started/activated.
	 * Can be stopped using Timer::stop().
	 * Can be restarted using Timer::reset() or Timer::set_millis() / Timer::set_micros().
	 * Usage: set_timeout_millis(interval_us, std::bind(&Class::function, this));
	 */
	std::weak_ptr<Timer> set_timeout_millis(int64_t interval_ms, const std::function<void()>& func);

	/** \brief Create and start a repeating timer.
	 *
	 * The created timer is already started/activated.
	 * Can be stopped using Timer::stop().
	 * Can be restarted using Timer::reset() or Timer::set_millis() / Timer::set_micros().
	 * Usage: set_timer_micros(interval_us, std::bind(&Class::function, this));
	 */
	std::shared_ptr<Timer> set_timer_micros(int64_t interval_us, const std::function<void()>& func);

	/** \brief Create and start a repeating timer.
	 *
	 * The created timer is already started/activated.
	 * Can be stopped using Timer::stop().
	 * Can be restarted using Timer::reset() or Timer::set_millis() / Timer::set_micros().
	 * Usage: set_timer_millis(interval_ms, std::bind(&Class::function, this));
	 */
	std::shared_ptr<Timer> set_timer_millis(int64_t interval_ms, const std::function<void()>& func);

	/** \brief Adds an AsyncClient to this module.
	 *
	 * Needs to be done before calling any service functions.
	 */
	void add_async_client(std::shared_ptr<AsyncClient> client);

	/** \brief Removes an AsyncClient from this module.
	 *
	 * Any pending requests/returns will be discarded.
	 */
	void rem_async_client(std::shared_ptr<AsyncClient> client);

	/// %Process a Value (internal use and special cases only)
	virtual void handle(std::shared_ptr<const Value> value);

	/// %Process a Message (internal use and special cases only)
	virtual void handle(std::shared_ptr<const Message> msg);

	/// %Process a Frame (internal use and special cases only)
	virtual void handle(std::shared_ptr<const Frame> frame);
	
	/// %Process a Sample (internal use and special cases only)
	virtual void handle(std::shared_ptr<const Sample> sample);

	/// %Process a Task (internal use and special cases only)
	virtual void handle(std::shared_ptr<const Task> task);

	/// %Process a FlowMessage (internal use and special cases only)
	virtual void handle(std::shared_ptr<const FlowMessage> msg);

	/// %Process a Request (internal use and special cases only)
	virtual std::shared_ptr<const Return> handle(std::shared_ptr<const Request> request);

	/// %Process a Return (internal use and special cases only)
	virtual bool handle(std::shared_ptr<const Return> result);

	/// %Process a resent Sample (special cases only)
	virtual void handle_resend(std::shared_ptr<const Sample> sample) {}

	/// Call handle() function for a Sample (internal use and special cases only)
	virtual void vnx_handle_switch(std::shared_ptr<const Value> value) = 0;

	/// Call service function for a Request (internal use and special cases only)
	virtual std::shared_ptr<Value> vnx_call_switch(std::shared_ptr<const Value> method, const vnx::request_id_t& request_id) = 0;

	/// Sends a Return back to the client [thread-safe]
	virtual bool vnx_async_return(const vnx::request_id_t& request_id, std::shared_ptr<const Value> value) const;

	/// Sends an Exception back to the client [thread-safe]
	virtual bool vnx_async_return_ex(const vnx::request_id_t& request_id, const std::exception& ex) const;

	/// Sends an Exception back to the client [thread-safe]
	virtual bool vnx_async_return_ex_what(const vnx::request_id_t& request_id, const std::string& ex_what) const;

	/// Needs to be called by Module implementation before processing any messages.
	virtual void vnx_setup();

	/// Needs to be called by Module implementation upon exiting.
	virtual void vnx_cleanup();

	/// Reset internal states regarding communication (eg. seq_map)
	virtual void vnx_reset();

	/// Runs one processing cycle, with a maximum time of (vnx_time_slice_us + timers).
	/// Returns timeout in usec until next timer deadline, or -1 if none.
	virtual int64_t vnx_process(bool blocking);

	/// Trigger module to stop [thread-safe]
	virtual void vnx_stop() = 0;

	/// Restarts the Module in-place after clearing state. [thread-safe]
	virtual void vnx_restart() = 0;

	/// Will publish a ModuleInfo message on "vnx.module_info"
	virtual void vnx_heartbeat() const;

	/// Performs user defined self test (by default returns true)
	virtual bool_t vnx_self_test() const;

private:
	const Hash64 module_id;
	TimeControl time_state;
	std::shared_ptr<Publisher> publisher;
	std::unordered_map<std::pair<Hash64, Hash64>, uint64_t> seq_map;
	std::set<std::shared_ptr<Timer>> timers;
	std::shared_ptr<Pipe> task_pipe;
	std::map<std::string, std::string> type_map;

	mutable std::mutex async_mutex;
	std::map<Hash64, std::shared_ptr<AsyncClient>> async_clients;
	mutable std::map<request_id_t, std::shared_ptr<Return>> async_requests;

	mutable int64_t last_time = 0;
	mutable int64_t time_idle = 0;
	mutable int64_t time_running = 0;
	mutable ModuleInfo module_info;

};


} // vnx

#endif /* INCLUDE_VNX_MODULEBASE_H_ */
