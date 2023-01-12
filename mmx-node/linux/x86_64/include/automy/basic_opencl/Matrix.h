/*
 * Matrix.h
 *
 *  Created on: Feb 5, 2021
 *      Author: mad
 */

#ifndef INCLUDE_AUTOMY_BASIC_OPENCL_MATRIX_H_
#define INCLUDE_AUTOMY_BASIC_OPENCL_MATRIX_H_

#include <automy/basic_opencl/Buffer3D.h>

#include <automy/math/Matrix.hpp>


namespace automy {
namespace basic_opencl {

template<typename T, size_t Rows, size_t Cols>
class Matrix : public Buffer3D<T> {
public:
	Matrix() : Buffer3D<T>(Rows, Cols) {}

	Matrix(size_t depth) : Buffer3D<T>(Rows, Cols, depth) {}

	void resize(size_t depth) {
		Buffer3D<T>::resize(Rows, Cols, depth);
	}

	size_t rows() const {
		return Buffer3D<T>::width();
	}

	size_t cols() const {
		return Buffer3D<T>::height();
	}

	using Buffer3D<T>::depth;

	template<typename S>
	void upload(std::shared_ptr<CommandQueue> queue, const math::Matrix<S, Rows, Cols>& mat) {
		resize(1);
		const math::Matrix<T, Rows, Cols> tmp(mat);
		Buffer3D<T>::upload(queue, tmp.get_data(), true);
	}

	template<typename S>
	void upload(std::shared_ptr<CommandQueue> queue, const std::vector<S>& mats) {
		const std::vector<math::Matrix<T, Rows, Cols>> tmp(mats.begin(), mats.end());
		resize(mats.size());
		Buffer3D<T>::upload(queue, tmp[0].get_data(), true);
	}

	void download(std::shared_ptr<CommandQueue> queue, math::Matrix<T, Rows, Cols>& mat, bool blocking = true) const {
		if(rows() != Rows  || cols() != Cols || depth() != 1) {
			throw std::logic_error("dimension mismatch");
		}
		Buffer3D<T>::download(queue, mat.get_data(), blocking);
	}

	void download(std::shared_ptr<CommandQueue> queue, std::vector<math::Matrix<T, Rows, Cols>>& mats, bool blocking = true) const {
		if(rows() != Rows  || cols() != Cols) {
			throw std::logic_error("dimension mismatch");
		}
		mats.resize(depth());
		Buffer3D<T>::download(queue, mats[0].get_data(), blocking);
	}

};


} // basic_opencl
} // automy

#endif /* INCLUDE_AUTOMY_BASIC_OPENCL_MATRIX_H_ */
