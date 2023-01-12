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

#ifndef INCLUDE_VNX_DIRECTORY_H
#define INCLUDE_VNX_DIRECTORY_H

#include <vnx/File.h>

#include <dirent.h>


namespace vnx {

/// Wrapper class to handle a file-system directory.
class Directory {
public:
	/// Creates empty directory object
	Directory();
	
	/// Creates directory object without opening it
	explicit Directory(const std::string& path_);
	
	Directory(const Directory&) = delete;
	
	/// Closes directory if still open. Will throw exception in case of failure.
	~Directory();
	
	Directory& operator=(Directory&) = delete;
	
	/// Sets a new path without opening it. Closes directory beforehand if still open.
	void set_path(const std::string& path_);

	/// Opens the directory and resets the path. Closes directory beforehand if still open.
	void open(const std::string& path_);
	
	/// Opens directory with path given before. Closes directory beforehand if still open.
	void open() const;
	
	/// Creates the directory and all intermediate directories and resets the path.
	void create(const std::string &path_);

	/// Creates the directory at the path given before and all intermediate directories.
	void create();

	/** \brief Returns a list of files in this directory.
	 * 
	 * Returned files are not opened.
	 * Directory needs to be open before this call, otherwise an empty vector is returned.
	 */
	std::vector<std::shared_ptr<File>> files(bool hidden = false, bool system = false) const;
	
	/** \brief Returns a list of sub-directories in this directory.
	 * 
	 * Returned directories are not opened.
	 * Directory needs to be open before this call, otherwise an empty vector is returned.
	 */
	std::vector<std::shared_ptr<Directory>> directories(bool hidden = false, bool system = false) const;
	
	/** \brief Returns a list of files and sub-directories in this directory.
	 * 
	 * Same as files() + directories() + any non-regular files (ie. symbolic links, sockets, etc).
	 */
	std::vector<std::shared_ptr<File>> all_files() const;
	
	/// Returns directory path with trailing "/" at the end if path is not empty.
	std::string get_path() const;
	
	/// Returns directory name without trailing "/", except root directory will return "/".
	std::string get_name() const;
	
	/// Closes the directory. Safe to call multiple times or on an empty object.
	void close() const;
	
private:
	std::string path;

#ifndef _WIN32
	mutable ::DIR* p_dir = nullptr;
#endif
	
};


} // vnx

#endif // INCLUDE_VNX_DIRECTORY_H
