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

#ifndef INCLUDE_VNX_CONFIG_H_
#define INCLUDE_VNX_CONFIG_H_

#include <vnx/Type.h>
#include <vnx/Variant.h>
#include <vnx/Object.h>


namespace vnx {

/// Returns config value for key \p key, returns empty in case no such key.
VNX_EXPORT Variant get_config(const std::string& key, bool protect = false);

/// Returns a config object for all sub-keys in the given namespace.
VNX_EXPORT Object get_config_object(const std::string& name_space, bool protect = false);

/// Returns a list of all config entries (a copy thereof)
VNX_EXPORT std::vector<std::pair<std::string, Variant>> get_all_configs(bool protect = false);

/// Set config value (in-memory)
VNX_EXPORT void set_config(const std::string& key, const Variant& value, bool protect = false);

/// Set config value via JSON string (in-memory)
VNX_EXPORT void set_config(const std::string& key, const std::string& value, bool protect = false);

/// Read config file system tree starting at \p root_path
VNX_EXPORT void read_config_tree(const std::string& root_path);

/// Reads a JSON config file (returns empty object if file does not exist)
VNX_EXPORT vnx::Object read_config_file(const std::string& file_path);

/// Writes a JSON config file
VNX_EXPORT void write_config_file(const std::string& file_path, const vnx::Object& config);

/// Add protection for given config key and all sub-keys
VNX_EXPORT void protect_config(const std::string& key);

/// Returns if config key is protected
VNX_EXPORT bool is_config_protected(const std::string& key);

/// Returns set of protected config keys
VNX_EXPORT std::set<std::string> get_config_protection();


} // vnx

#endif /* INCLUDE_VNX_CONFIG_H_ */
