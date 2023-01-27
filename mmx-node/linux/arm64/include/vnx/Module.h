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

#ifndef INCLUDE_VNX_MODULE_H_
#define INCLUDE_VNX_MODULE_H_

#include <vnx/ModuleBase.h>
#include <vnx/Handle.h>

#include <atomic>
#include <thread>


namespace vnx {

/// Adds a module to the global registry (used by vnx::Handle only)
VNX_EXPORT void add_module(std::shared_ptr<Module> module);

/// Removes a module from the global registry (used by vnx::Handle only)
VNX_EXPORT void remove_module(std::shared_ptr<Module> module);

/// Returns the latest module handle found for given name.
VNX_EXPORT Handle<Module> get_module(const std::string& name);

/// Returns all module handles found for the given name.
VNX_EXPORT std::vector<Handle<Module>> get_modules(const std::string& name);

/// Returns array of all modules.
VNX_EXPORT std::vector<Handle<Module>> get_all_modules();


/** \brief Module is the base class for all user Modules.
 * 
 * A Module has it's own thread and main() function, just like a normal process.
 * The user's implementation will inherit from the generated base class which in turn
 * inherits from this class.
 * 
 * The Module class handles timer callbacks, receiving of Sample%s as well as processing
 * of Request%s inside it's main() function. The user can override main() to implement
 * initialization and cleanup functionality, but must call Module::main() at some point.
 */
class Module : public ModuleBase {
public:
	/** \brief Creates a new Module with given name.
	 * 
	 * @param name Module name (does not have to be unique)
	 */
	Module(const std::string& name);
	
	~Module();
	
	typedef ModuleBase Super;

protected:
	/// Returns false if this module is being shut down. [thread-safe]
	bool vnx_do_run() const;
	
	/** \brief Called before main() from the thread starting the module.
	 * 
	 * Used to make sure that the module is initialized after Handle::start() returns.
	 * For example, open_pipe() calls should be done in init() such that a client connecting after
	 * Handle::start() is guaranteed to succeed, which would not be the case if open_pipe() was called in
	 * main() since that is a different thread that was just spawned in the background and it is undefined when
	 * it will actually start to execute.
	 */
	virtual void init() {}
	
	/** \brief Main function, just like the real int main().
	 * 
	 * Called from within a new thread that is spawned when starting the Module.
	 * Make sure to call Super::main() (ie. Module::main()) at some point when over-riding it,
	 * since all the processing is done in Module::main().
	 */
	virtual void main();

	/// Same as Node::exit()
	virtual void exit() override;
	
	/// Trigger module to stop [thread-safe]
	virtual void vnx_stop() override;

	/// Restarts the Module in-place, calling init() and main() again after clearing state. [thread-safe]
	virtual void vnx_restart() override;

	/** \brief Shutdown handler, called upon module/process shutdown. Needs to be thread-safe!
	 *
	 * Can be overridden to customize shutdown behavior.
	 * @return True if module should shutdown now (default), otherwise shutdown is delayed until manual call to Module::exit().
	 */
	virtual bool vnx_shutdown();

	using Super::handle;

private:
	void vnx_entry();
	
	void vnx_start(std::shared_ptr<Module> self_ptr_);
	
	bool vnx_is_running();
	
	void vnx_wait();
	
private:
	std::thread thread;
	std::shared_ptr<Module> self_ptr;
	std::atomic<size_t> ref_count {0};
	bool do_restart = false;
	bool is_waiting = false;
	
	template<typename T>
	friend class Handle;
	
};


} // vnx

#endif /* INCLUDE_VNX_MODULE_H_ */
