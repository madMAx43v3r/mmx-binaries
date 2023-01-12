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

#ifndef INCLUDE_VNX_DEFAULTPRINTER_H_
#define INCLUDE_VNX_DEFAULTPRINTER_H_

#include <vnx/Visitor.h>


namespace vnx {

/** \brief A Visitor that generates JSON string output.
 * 
 * The DefaultPrinter does not generate any newlines.
 */
class DefaultPrinter : public Visitor {
public:
	DefaultPrinter(std::ostream& out);
	
	void visit_null() override;
	void visit(const bool& value) override;
	void visit(const uint8_t& value) override;
	void visit(const uint16_t& value) override;
	void visit(const uint32_t& value) override;
	void visit(const uint64_t& value) override;
	void visit(const int8_t& value) override;
	void visit(const int16_t& value) override;
	void visit(const int32_t& value) override;
	void visit(const int64_t& value) override;
	void visit(const float32_t& value) override;
	void visit(const float64_t& value) override;
	void visit(const std::string& value) override;
	
	void list_begin(size_t size) override;
	void list_element(size_t index) override;
	void list_end(size_t size) override;
	
	void type_begin(size_t num_fields, const std::string* type_name) override;
	void type_field(const std::string& field, size_t index) override;
	void type_end(size_t num_fields, const std::string* type_name) override;
	
protected:
	std::ostream& out;

	int stack = 0;

};


} // vnx

#endif /* INCLUDE_VNX_DEFAULTPRINTER_H_ */
