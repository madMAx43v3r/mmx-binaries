/*
 * Authentication.h
 *
 *  Created on: Nov 24, 2020
 *      Author: jaw
 */

#ifndef INCLUDE_VNX_AUTHENTICATION_H_
#define INCLUDE_VNX_AUTHENTICATION_H_

#include <vnx/User.hxx>
#include <vnx/Session.hxx>
#include <vnx/permission_e.hxx>
#include <vnx/access_role_e.hxx>

#include <mutex>
#include <unordered_set>
#include <unordered_map>


namespace std {
	template<> struct hash<std::pair<vnx::Hash64, std::string>> {
		size_t operator()(const std::pair<vnx::Hash64, std::string>& x) const {
			return x.first ^ std::hash<std::string>{}(x.second);
		}
	};
} // std


namespace vnx {

/**
 * All methods of AuthenticationServer are thread-safe.
 */
class AuthenticationServer {
public:
	AuthenticationServer();

	std::shared_ptr<const Session> login(	const std::string &name, const std::string &password,
											const std::string &default_access = "DEFAULT");

	std::shared_ptr<const Session> login_anonymous(const std::string &access_role);
	std::shared_ptr<const Session> login_anonymous(const std::vector<std::string> &access_roles);

	template<typename T>
	std::shared_ptr<const Session> login_anonymous(const T &access_role) {
		return login_anonymous(vnx::to_string_value(access_role));
	}

	void logout(Hash64 session_id);

	void add_user(std::shared_ptr<const User> user);

	std::shared_ptr<User> get_user(const std::string &name) const;

	std::shared_ptr<const Session> get_session(Hash64 session_id) const;

	std::set<std::string> get_permissions(const std::string &role) const;

	template<typename T>
	std::set<std::string> get_permissions(const T &access_role) const {
		return get_permissions(vnx::to_string_value(access_role));
	}

	bool is_allowed(Hash64 session_id, const std::string &perm) const;

	template<typename T>
	bool is_allowed(Hash64 session_id, const T &perm) const {
		return is_allowed(session_id, vnx::to_string_value_full(perm));
	}

	std::string final_hash(const std::string &input) const;

protected:
	Hash64 new_session_id();

	void add_session(std::shared_ptr<Session> session);

	void add_permissions(const access_role_e &access_role, const std::set<vnx::permission_e> &perms);

private:
	static const std::string salt;
	mutable std::mutex mutex;
	std::map<std::string, User> users;
	std::map<std::string, std::set<std::string>> permissions;
	std::unordered_map<Hash64, std::shared_ptr<const Session>> sessions;
	std::unordered_multiset<std::pair<Hash64, std::string>> permissions_lookup;
};


/// Returns global instance (never a nullptr)
std::shared_ptr<AuthenticationServer> get_auth_server();

/// Returns corresponding session if it exists (otherwise nullptr)
std::shared_ptr<const Session> get_session(Hash64 session_id);

/// Returns true if session has specified permission
bool is_allowed(Hash64 session_id, const std::string& perm);

/// Returns true if session has specified permission
template<typename T>
bool is_allowed(Hash64 session_id, const T& perm) {
	return is_allowed(session_id, vnx::to_string_value_full(perm));
}


namespace process {
	extern std::shared_ptr<AuthenticationServer> auth_server;
} // process

} // vnx

#endif /* INCLUDE_VNX_AUTHENTICATION_H_ */
