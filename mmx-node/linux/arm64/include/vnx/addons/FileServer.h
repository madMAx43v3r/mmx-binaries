/*
 * FileServer.h
 *
 *  Created on: Jul 30, 2020
 *      Author: mad
 */

#ifndef INCLUDE_VNX_ADDONS_FILESERVER_H_
#define INCLUDE_VNX_ADDONS_FILESERVER_H_

#include <vnx/addons/FileServerBase.hxx>


namespace vnx {
namespace addons {

class FileServer : public FileServerBase {
public:
	FileServer(const std::string& _vnx_name);

protected:
	void init() override;

	void main() override;

	vnx::Buffer read_file(const std::string& path) const override;

	vnx::Buffer read_file_range(const std::string& path, const int64_t& offset, const int64_t& length) const override;

	file_info_t get_file_info(const std::string& path) const override;

	std::vector<file_info_t> read_directory(const std::string& path) const override;

	void write_file(const std::string& path, const vnx::Buffer& data) override;

	void delete_file(const std::string& path) override;

	void http_request_async(std::shared_ptr<const HttpRequest> request,
							const std::string& sub_path,
							const vnx::request_id_t& request_id) const override;

	void http_request_chunk_async(std::shared_ptr<const HttpRequest> request,
							const std::string& sub_path,
							const int64_t& offset,
							const int64_t& max_bytes,
							const vnx::request_id_t& _request_id) const override;

private:
	void write_file_internal(const std::string& path, const vnx::Buffer& data) const;

	void delete_file_internal(const std::string& path) const;

	std::string detect_mime_type(const std::string& path) const;

	bool http_request_boilerplate(std::shared_ptr<const HttpRequest> request, std::shared_ptr<HttpResponse> response, const std::string &sub_path, std::string &file_path) const;

};


} // addons
} // vnx

#endif /* INCLUDE_VNX_ADDONS_FILESERVER_H_ */
