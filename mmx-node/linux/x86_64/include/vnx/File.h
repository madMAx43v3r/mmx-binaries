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

#ifndef INCLUDE_VNX_FILE_H
#define INCLUDE_VNX_FILE_H

#include <vnx/Input.h>
#include <vnx/Output.h>

#include <cstdio>


namespace vnx {

/// Wrapper class to handle a file-system file.
class File {
public:
	TypeInput in;		///< Can be used to read from the file
	TypeOutput out;		///< Can be used to write to the file
	
	/// Creates empty file object
	File();
	
	/// Creates a new File object without opening the file
	explicit File(const std::string& path);
	
	File(const File&) = delete;
	
	/// Closes file if still open. Will throw exception in case of failure.
	~File();
	
	File& operator=(const File&) = delete;
	
	/** \brief Open file with specified mode as in fopen().
	 * 
	 * Will close file beforehand if already open.
	 */
	void open(const std::string& mode);
	
	/** \brief Open file with specified mode as in fopen().
	 * 
	 * Resets the path to given path.
	 * Will close file beforehand if already open.
	 */
	void open(const std::string& path, const std::string& mode);
	
	/** \brief Creates file if it doesn't exist yet.
	 *
	 * Does not open the file.
	 * Returns true if file was created and it didn't exist before.
	 */
	bool create();

	/** \brief Returns the first CODE in the file.
	 * 
	 * Will skip over CODE_NONE and CODE_MAGIC.
	 */
	uint16_t read_header();
	
	/// Writes CODE_NONE + CODE_MAGIC
	void write_header();
	
	/// Seeks to beginning of the file. Will flush output buffer before doing so.
	void seek_begin();
	
	/// Seeks to the end of the file. Will flush output buffer before doing so.
	void seek_end();
	
	/// Seeks to the specified byte position. Will flush output buffer before doing so.
	void seek_to(int64_t pos);
	
	/// Calls posix_fadvise() (length <= 0 means all remaining)
	void fadvise(int advice, int64_t pos = 0, int64_t length = 0);
	
	/// Returns file path as it was given
	std::string get_path() const {
		return path;
	}
	
	/// Returns file name without any directories and without trailing "/"
	std::string get_name(bool with_ext = true) const;
	
	/// Returns file extension including the last ".". Returns empty string if it has no extension.
	std::string get_extension() const;
	
	/// Returns byte offset from current read position to beginning of file when reading data.
	int64_t get_input_pos() const {
		return in.get_input_pos();
	}
	
	/// Returns byte offset from current write position to beginning of file when writing data.
	int64_t get_output_pos() const {
		return out.get_output_pos();
	}
	
	/// Returns last modification time in seconds
	int64_t last_write_time() const;
	
	/// Returns the current file size in bytes
	int64_t file_size() const;
	
	/// Returns if file does exist
	bool exists() const;
	
	/// Returns if file is a directory
	bool is_directory() const;
	
	/// Returns if file is a regular file (excluding symbolic links)
	bool is_regular() const;
	
	/// Returns if file is a symbolic link
	bool is_symlink() const;
	
	/// Flushes output buffer and then calls fflush()
	void flush();
	
	/// Flushes output buffer and then closes file. Safe to call multiple times or on an empty object.
	void close();
	
	/// Deletes the file, closes file first if still open. (non-existent file is ignored)
	void remove();
	
	/// Renames file to the new given file path. (will close file first if still open)
	void rename(const std::string& new_path);

	/// Locks the entire file for exclusive access
	void lock_exclusive();

	/// Unlocks a previous file lock (called automatically on close)
	void unlock();

	/// Returns internal file pointer
	::FILE* get_handle() const { return p_file; }
	
private:
	std::string path;
	::FILE* p_file = nullptr;
	bool is_locked = false;
	
	FileInputStream stream_in;
	FileOutputStream stream_out;
	
};


} // vnx

#endif // INCLUDE_VNX_FILE_H
