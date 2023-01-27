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

#ifndef INCLUDE_VNX_HANDLE_H_
#define INCLUDE_VNX_HANDLE_H_

#include <memory>


namespace vnx {

/** \brief Smart pointer class for Modules.
 *
 * Handle is wrapper around std::shared_ptr<T> for Modules.
 * It will automatically stop and delete a Module unless it has been detached.
 */
template<typename T>
class Handle {
public:
	Handle() {}

	/// Takes ownership of the Module (may stop the module).
	Handle(T* module_) {
		*this = module_;
	}

	/// Does not take ownership of the Module (detached).
	Handle(std::shared_ptr<T> module_) {
		*this = module_;
	}

	Handle(const Handle& other) {
		*this = other;
	}

	template<typename S>
	Handle(const Handle<S>& other) {
		*this = other;
	}

	/// Will close the Module if it's not detached and this is the only Handle.
	~Handle() {
		release();
	}

	/// Takes ownership of the Module (may stop the module).
	Handle& operator=(T* module_) {
		reset(std::shared_ptr<T>(module_), true);
		return *this;
	}

	/// Does not take ownership of the Module (detached).
	Handle& operator=(std::shared_ptr<T> module_) {
		reset(module_, false);
		return *this;
	}

	Handle& operator=(const Handle& other) {
		reset(other.module, !other.detached);
		return *this;
	}

	template<typename S>
	Handle& operator=(const Handle<S>& other) {
		reset(other.shared_ptr(), !other.is_detached());
		return *this;
	}

	operator bool() {
		return bool(module);
	}

	/// Returns module id
	Hash64 get_id() const {
		if(!module) {
			throw std::logic_error("!module");
		}
		return module->vnx_get_id();
	}

	/// Returns module name
	std::string get_name() const {
		if(!module) {
			throw std::logic_error("!module");
		}
		return module->vnx_get_name();
	}

	/// Access the module. Makes sure it is not already running.
	T& operator*() {
		if(!module) {
			throw std::logic_error("!module");
		}
		if(module->vnx_is_running()) {
			throw std::logic_error("Handle: module already running");
		}
		return *module;
	}

	/// Access the module. Makes sure it is not already running.
	T* operator->() {
		if(!module) {
			throw std::logic_error("!module");
		}
		if(module->vnx_is_running()) {
			throw std::logic_error("Handle: module already running");
		}
		return module.get();
	}

	/// Returns true if module is running.
	bool is_running() const {
		return module && module->vnx_is_running();
	}

	/// Returns true if this Handle is detached.
	bool is_detached() const {
		return module && detached;
	}

	/** \brief Starts the module in a new thread.
	 *
	 * Returns after Module::init() is done.
	 */
	void start() {
		if(module) {
			module->vnx_start(module);
		}
	}

	/** \brief Starts the module and detaches it.
	 *
	 * Module will self destruct upon shutdown.
	 */
	void start_detached() {
		start();
		detach();
	}

	/** \brief Restarts the module by stopping and starting it again. (blocking)
	 *
	 * Will call init() and main() again after clearing state.
	 */
	void restart() {
		exit();
		wait();
		start();
	}

	/** \brief Restarts the module in-place. (non-blocking)
	 *
	 * Will call init() and main() again after clearing state.
	 */
	void restart_async() {
		if(module) {
			module->vnx_restart();
		}
	}

	/** \brief Detaches the module.
	 *
	 * Module will self destruct upon shutdown.
	 */
	void detach() {
		if(module && !detached) {
			detached = true;
			module->ref_count--;
		}
	}

	/** \brief Attach to a detached module.
	 *
	 * @return If attach was successful, ie. the Module was detached and still running.
	 */
	bool attach() {
		if(module && detached) {
			detached = false;
			module->ref_count++;
			return module->vnx_is_running();
		}
		return false;
	}

	/// Wait for the module to exit / finish.
	void wait() {
		if(module) {
			module->vnx_wait();
		}
	}

	/// Trigger the module to exit.
	void exit() {
		if(module) {
			module->exit();
		}
	}

	/// Ask the module to exit, will invoke vnx_shutdown().
	void stop() {
		if(module) {
			module->vnx_stop();
		}
	}

	/// Trigger the module to exit, wait for it to finish and then delete it.
	void close() {
		exit();
		wait();
		release();
	}

	/// Ask the module to exit, wait for it to finish and then delete it.
	void shutdown() {
		stop();
		wait();
		release();
	}

	/// Release ownership of the module, same as ~Handle().
	void release() {
		if(module && !detached) {
			if(--module->ref_count == 0) {
				module->exit();
				module->vnx_wait();
			}
		}
		detached = false;
		module = nullptr;
	}

	/// Release ownership of the module and assign new ownership, same as ~Handle() + Handle().
	void reset(std::shared_ptr<T> module_, bool do_attach) {
		release();
		module = module_;
		if(module) {
			if(do_attach) {
				module->ref_count++;
			} else {
				detached = true;
			}
		}
	}

	/// Internal use only.
	std::shared_ptr<T> shared_ptr() const {
		return module;
	}

private:
	bool detached = false;
	std::shared_ptr<T> module;

};


} // vnx

#endif /* INCLUDE_VNX_HANDLE_H_ */
