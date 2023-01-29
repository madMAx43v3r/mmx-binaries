
// AUTO GENERATED by vnxcppcodegen

#ifndef INCLUDE_vnx_Message_HXX_
#define INCLUDE_vnx_Message_HXX_

#include <vnx/package.hxx>
#include <vnx/Hash64.hpp>
#include <vnx/Value.h>


namespace vnx {

class VNX_EXPORT Message : public ::vnx::Value {
public:
	static const uint16_t BLOCKING = 1;
	static const uint16_t REPLAY = 2;
	static const uint16_t NO_RETURN = 4;
	static const uint16_t NON_BLOCKING = 8;
	static const uint16_t SHUTDOWN = 16;
	static const uint16_t RESEND = 32;
	static const uint16_t FORWARD = 64;
	
	::vnx::Hash64 stream;
	uint16_t flags = 0;
	uint16_t hop_count = 0;
	
	typedef ::vnx::Value Super;
	
	static const vnx::Hash64 VNX_TYPE_HASH;
	static const vnx::Hash64 VNX_CODE_HASH;
	
	static constexpr uint64_t VNX_TYPE_ID = 0xff12a5be92ae4dafull;
	
	Message() {}
	
	vnx::Hash64 get_type_hash() const override;
	std::string get_type_name() const override;
	const vnx::TypeCode* get_type_code() const override;
	
	static std::shared_ptr<Message> create();
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
	
	friend std::ostream& operator<<(std::ostream& _out, const Message& _value);
	friend std::istream& operator>>(std::istream& _in, Message& _value);
	
	static const vnx::TypeCode* static_get_type_code();
	static std::shared_ptr<vnx::TypeCode> static_create_type_code();
	
protected:
	std::shared_ptr<vnx::Value> vnx_call_switch(std::shared_ptr<const vnx::Value> _method) override;
	
};

template<typename T>
void Message::accept_generic(T& _visitor) const {
	_visitor.template type_begin<Message>(3);
	_visitor.type_field("stream", 0); _visitor.accept(stream);
	_visitor.type_field("flags", 1); _visitor.accept(flags);
	_visitor.type_field("hop_count", 2); _visitor.accept(hop_count);
	_visitor.template type_end<Message>(3);
}


} // namespace vnx


namespace vnx {

} // vnx

#endif // INCLUDE_vnx_Message_HXX_