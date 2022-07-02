#pragma once

#include <vector>
#include "Vector.hpp"

template<uint8_t left_rows, uint8_t common_size, uint8_t right_columns>
std::array<float, left_rows*right_columns> genericMatrixProduct(const std::array<float, left_rows*common_size>& mat_left, const std::array<float, common_size*right_columns>& mat_right)
{
	std::array<float, left_rows*right_columns> out{};
	for (uint8_t rcol = 0; rcol < right_columns; ++rcol)
		for (uint8_t lrow = 0; lrow < left_rows; ++lrow)
			for (uint8_t dot_idx = 0; dot_idx < common_size; ++dot_idx)
				out[rcol + lrow * right_columns] += mat_left[dot_idx + lrow * left_rows] * mat_right[rcol + right_columns * dot_idx];
	return out;
}

// Special matrix transforms
std::array<float, 12> transformsToMatrix(const std::array<float, 12>& transforms);
std::array<float, 12> transformsToMatrix(const std::array<float, 4>& quat, const std::array<float, 3>& loc, const std::array<float, 3>& scale);
std::array<float, 12> multiplyTransformMatrices(const std::array<float, 12>& a, const std::array<float, 12>& b);
std::array<float, 12> makeTotalBoneMatrix(int16_t bone_idx, const std::vector<std::array<float, 12>>& bone_matrices, const std::vector<uint16_t>& bone_parents);
inline std::array<float, 12> invertRotationLocationMatrix(const std::array<float, 12>& mat);

// Matrix generators
std::array<float, 9> makeRotationMatrix(const std::array<float, 3>& position, const std::array<float, 3>& target, const std::array<float, 3>& up);
std::array<float, 16> makeViewMatrix(const std::array<float, 3>& position, const std::array<float, 3>& target, const std::array<float, 3>& up);
std::array<float, 16> invertViewMatrix(const std::array<float, 16>& view, const std::array<float, 3>& position);
std::array<float, 16> perspectiveMatrix(float fov, float aspect, float zNear, float zFar);
std::array<float, 16> inversePerspectiveMatrix(float fov, float aspect, float zNear, float zFar);
