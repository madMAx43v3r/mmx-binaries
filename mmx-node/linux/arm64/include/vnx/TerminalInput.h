/*
 * TerminalInput.h
 *
 *  Created on: Sep 14, 2020
 *      Author: jaw
 */


#ifndef VNX_TERMINALINPUT_H_
#define VNX_TERMINALINPUT_H_


#include <string>
#include <list>
#include <vector>
#include <map>
#include <set>



namespace vnx{

class TerminalInput{
public:
	TerminalInput();

	void add_history(const std::string &line);
	std::string history_up();
	std::string history_down();
	void history_reset();
	void add_command(const std::string &command);
	void add_argument(const std::string &command, const std::string &argument);
	void clear_commands();
	void clear_arguments(const std::string &command);
	std::pair<std::string, bool> complete_command(const std::string &prefix) const;
	std::pair<std::string, bool> complete_argument(const std::string &command, const std::string &prefix) const;
	std::vector<std::string> guess_command(const std::string &prefix) const;
	std::vector<std::string> guess_argument(const std::string &command, const std::string &prefix) const;

private:
	static size_t string_differ(const std::string &one, const std::string &two);
	std::list<std::string> m_history;
	std::list<std::string>::iterator m_historyPos;
	std::set<std::string> m_commands;
	std::map<std::string, std::set<std::string>> m_arguments;
	std::pair<std::string, bool> complete(const std::string &prefix, const std::set<std::string> &set) const;
	std::vector<std::string> guess(const std::string &prefix, const std::set<std::string> &set) const;
};



} // vnx

#endif /* VNX_TERMINAL_H_ */
