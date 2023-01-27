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

#ifndef INCLUDE_VNX_TOSTRINGVALUE_H_
#define INCLUDE_VNX_TOSTRINGVALUE_H_

#include <vnx/DefaultPrinter.h>


namespace vnx {

/** \brief A Visitor that generates JSON string output with some exceptions.
 *
 * Same as DefaultPrinter except:
 *  - strings are without quotes (at the top level)
 *  - enum values are without quotes (at the top level)
 *  - null is an empty string
 *
 * This Visitor is used when the desired output is a value.
 */
class ToStringValue : public DefaultPrinter {
public:
	ToStringValue(std::ostream& out);

	void visit_null() override;

	void visit(const std::string& value) override;

};


} // vnx

#endif /* INCLUDE_VNX_TOSTRINGVALUE_H_ */
