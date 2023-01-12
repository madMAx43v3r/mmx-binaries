
// AUTO GENERATED by vnxcppcodegen

#ifndef INCLUDE_vnx_ModuleInfo_HXX_
#define INCLUDE_vnx_ModuleInfo_HXX_

#include <vnx/package.hxx>
#include <vnx/Endpoint.hxx>
#include <vnx/Hash64.hpp>
#include <vnx/TypeCode.hpp>
#include <vnx/Value.h>


namespace vnx {

class VNX_EXPORT ModuleInfo : public ::vnx::Value {
public:
	
	int64_t time = 0;
	::vnx::Hash64 id;
	::vnx::Hash64 src_mac;
	std::string name;
	std::string type;
	int64_t time_started = 0;
	int64_t time_idle = 0;
	int64_t time_running = 0;
	int64_t time_idle_total = 0;
	int64_t time_running_total = 0;
	int64_t num_async_pending = 0;
	int64_t num_async_process = 0;
	std::vector<std::string> sub_topics;
	std::vector<std::string> pub_topics;
	std::map<::vnx::Hash64, std::shared_ptr<const ::vnx::Endpoint>> remotes;
	::vnx::TypeCode type_code;
	
	typedef ::vnx::Value Super;
	
	static const vnx::Hash64 VNX_TYPE_HASH;
	static const vnx::Hash64 VNX_CODE_HASH;
	
	static constexpr uint64_t VNX_TYPE_ID = 0xde56f839ffcee92ull;
	
	ModuleInfo() {}
	
	vnx::Hash64 get_type_hash() const override;
	std::string get_type_name() const override;
	const vnx::TypeCode* get_type_code() const override;
	
	virtual vnx::float64_t get_cpu_load() const;
	virtual vnx::float64_t get_cpu_load_total() const;
	
	static std::shared_ptr<ModuleInfo> create();
	std::shared_ptr<vnx::Value> clone() const override;
	
	void read(vnx::TypeInput& _in, const vnx::TypeCode* _type_code, const uint16_t* _code) override;
	void write(vnx::TypeOutput& _out, const vnx::TypeCode* _type_code, const uint16_t* _code) const override;
	
	void read(std::istream& _in) override;
	void write(std::ostream& _out) const override;
	
	template<typename T>
	void accept_generic(T& _visitor) const;
	void accept(vnx::Visitor& _visitor) const override;
	
	vnx::Object to_object() const override;
	void from_object(const vnx::Object& object) override;
	
	vnx::Variant get_field(const std::string& name) const override;
	void set_field(const std::string& name, const vnx::Variant& value) override;
	
	friend std::ostream& operator<<(std::ostream& _out, const ModuleInfo& _value);
	friend std::istream& operator>>(std::istream& _in, ModuleInfo& _value);
	
	static const vnx::TypeCode* static_get_type_code();
	static std::shared_ptr<vnx::TypeCode> static_create_type_code();
	
protected:
	std::shared_ptr<vnx::Value> vnx_call_switch(std::shared_ptr<const vnx::Value> _method) override;
	
};

template<typename T>
void ModuleInfo::accept_generic(T& _visitor) const {
	_visitor.template type_begin<ModuleInfo>(16);
	_visitor.type_field("time", 0); _visitor.accept(time);
	_visitor.type_field("id", 1); _visitor.accept(id);
	_visitor.type_field("src_mac", 2); _visitor.accept(src_mac);
	_visitor.type_field("name", 3); _visitor.accept(name);
	_visitor.type_field("type", 4); _visitor.accept(type);
	_visitor.type_field("time_started", 5); _visitor.accept(time_started);
	_visitor.type_field("time_idle", 6); _visitor.accept(time_idle);
	_visitor.type_field("time_running", 7); _visitor.accept(time_running);
	_visitor.type_field("time_idle_total", 8); _visitor.accept(time_idle_total);
	_visitor.type_field("time_running_total", 9); _visitor.accept(time_running_total);
	_visitor.type_field("num_async_pending", 10); _visitor.accept(num_async_pending);
	_visitor.type_field("num_async_process", 11); _visitor.accept(num_async_process);
	_visitor.type_field("sub_topics", 12); _visitor.accept(sub_topics);
	_visitor.type_field("pub_topics", 13); _visitor.accept(pub_topics);
	_visitor.type_field("remotes", 14); _visitor.accept(remotes);
	_visitor.type_field("type_code", 15); _visitor.accept(type_code);
	_visitor.template type_end<ModuleInfo>(16);
}


} // namespace vnx


namespace vnx {

} // vnx

#endif // INCLUDE_vnx_ModuleInfo_HXX_
