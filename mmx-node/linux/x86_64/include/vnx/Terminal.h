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

#ifndef VNX_TERMINAL_H_
#define VNX_TERMINAL_H_

#include <vnx/TerminalBase.hxx>
#include <vnx/TerminalClient.hxx>
#include <vnx/ProcessClient.hxx>
#include <vnx/GenericAsyncClient.h>
#include <vnx/TerminalInput.h>

#ifdef _WIN32
typedef unsigned long DWORD;
#else
struct termios;
#endif


namespace vnx {

/** \brief Module that provides an interactive terminal. 
 * 
 * Press enter to activate the command prompt (will pause logging output).
 * 
 * Possible commands are:
 * - \b q or \b quit Will shutdown process.
 * - \b d or \b debug [level] Sets logging level, or sets DEBUG level if no level given, (4=DEBUG, 3=INFO, ...)
 * - \b e or \b errors Shows list of last error messages.
 * - \b t or \b topic [expr] Shows list of topic statistics, shows all in case no \p expr, otherwise only topics containing \p expr.
 * - \b grep <expr> Grep logging output, just like normal UNIX grep.
 * - \b spy [expr] Shows sample info for matching topics, or all topics in case no \p expr. Same as vnxdump -i.
 * - \b dump [expr] Dumps samples for matching topics, or all topics in case no \p expr. Same as vnxdump.
 * 
 */
class VNX_EXPORT Terminal : public TerminalBase {
public:
	Terminal(const std::string& vnx_name_ = "Terminal");
	
protected:
	enum {
		INACTIVE,
		INPUT,
		DISPLAY,
		GREP_RUN,
		HTOP_RUN,
		MODULE_RUN,
		EXEC_RUN,
	};
	
	void init() override;
	
	void main() override;
	
	void read_char(const signed char &c) override;

	void read_event(const terminal_event_e &command) override;

private:
	static void read_loop(Hash64 service_addr) noexcept;
	static void read_loop_impl(Hash64 service_addr);

#ifdef _WIN32
	static DWORD saved_attributes;
#else
	static termios saved_attributes;
#endif
	static void set_terminal_mode();
	static void reset_terminal_mode();
	
	void get_completion();
	void write_editline(std::ostream &out) const;
	void write_editline_ansi(std::ostream &out) const;
#ifdef _WIN32
	void write_editline_winapi(std::ostream &out) const;
#endif

	void command(const std::vector<std::string> &commandline);
	void spy(const std::string& expr);
	void dump(const std::string& expr);
	void topic_info(const std::string& expr);
	void module_info(const std::string &expr);
	void show_config(const std::string &expr);
	void htop(const bool &order_by_avg);
	int exec(const std::string &module, const std::string &method, const std::vector<std::string> &args, const bool &async);
	void exec_return(std::shared_ptr<const Value> result);
	void exec_err(const vnx::exception &err);

	Hash64 service_addr;
	ProcessClient process;
	TerminalInput input;

#ifdef _WIN32
	bool use_ansi_escape = true;
#endif
	std::string line = "";
	std::pair<std::string, bool> completion = std::make_pair("", false);
	size_t cursor = 0;
	bool tab_once = false;
	
	int state = INACTIVE;
	Handle<Module> module;
	std::shared_ptr<GenericAsyncClient> exec_client;
	std::shared_ptr<vnx::Timer> interval;
	
	void update_hints();
	template <class Iter>
	void output_list(std::ostream &out, Iter begin, Iter end, const std::string &none) const;
	template <class T>
	void output_value(std::ostream &out, const T &value) const;
};


template <class Iter>
void Terminal::output_list(std::ostream &out, Iter begin, Iter end, const std::string &none) const{
	if(begin == end){
		out << none;
		return;
	}

	auto iter = begin;
	while(iter != end){
		output_value(out, *iter);
		iter++;
		if(iter != end) out << ", ";
	}
}


template <class T>
void Terminal::output_value(std::ostream &out, const T &value) const{
	out << vnx::to_string(value);
}



} // vnx

#endif /* VNX_TERMINAL_H_ */
