#include "Matrix.hpp"


// Special matrix transforms

// Converts a Quat, Loc, Scale array into a transform matrix
std::array<float, 12> transformsToMatrix(const std::array<float, 12>& transforms)
{
	// Quaternion
	float q_x = transforms[0];
	float q_y = transforms[1];
	float q_z = transforms[2];
	float q_w = transforms[3];

	// Location
	float l_x = transforms[4];
	float l_y = transforms[5];
	float l_z = transforms[6];

	// Scale
	float s_x = transforms[8];
	float s_y = transforms[9];
	float s_z = transforms[10];

	std::array<float, 12> out;

	// Calculate total transform matrix
	float q_xx = q_x * q_x;
	float q_yy = q_y * q_y;
	float q_zz = q_z * q_z;

	float q_xy = q_x * q_y;
	float q_xz = q_x * q_z;
	float q_yz = q_y * q_z;
	float q_xw = q_x * q_w;
	float q_yw = q_y * q_w;
	float q_zw = q_z * q_w;

	out[0] = 2 * s_x * (0.5 - q_yy - q_zz);
	out[1] = 2 * s_x * (q_xy - q_zw);
	out[2] = 2 * s_x * (q_xz + q_yw);
	out[3] = l_x;
	out[4] = 2 * s_y * (q_xy + q_zw);
	out[5] = 2 * s_y * (0.5 - q_xx - q_zz);
	out[6] = 2 * s_y * (q_yz - q_xw);
	out[7] = l_y;
	out[8] = 2 * s_z * (q_xz - q_yw);
	out[9] = 2 * s_z * (q_yz + q_xw);
	out[10] = 2 * s_z * (0.5 - q_xx - q_yy);
	out[11] = l_z;

	return out;
}

std::array<float, 12> transformsToMatrix(const std::array<float, 4>& quat, const std::array<float, 3>& loc, const std::array<float, 3>& scale)
{
	// Quaternion
	float q_x = quat[0];
	float q_y = quat[1];
	float q_z = quat[2];
	float q_w = quat[3];

	// Location
	float l_x = loc[0];
	float l_y = loc[1];
	float l_z = loc[2];

	// Scale
	float s_x = scale[0];
	float s_y = scale[1];
	float s_z = scale[2];

	std::array<float, 12> out;

	// Calculate total transform matrix
	float q_xx = q_x * q_x;
	float q_yy = q_y * q_y;
	float q_zz = q_z * q_z;

	float q_xy = q_x * q_y;
	float q_xz = q_x * q_z;
	float q_yz = q_y * q_z;
	float q_xw = q_x * q_w;
	float q_yw = q_y * q_w;
	float q_zw = q_z * q_w;

	out[0] = 2 * s_x * (0.5 - q_yy - q_zz);
	out[1] = 2 * s_x * (q_xy - q_zw);
	out[2] = 2 * s_x * (q_xz + q_yw);
	out[3] = l_x;
	out[4] = 2 * s_y * (q_xy + q_zw);
	out[5] = 2 * s_y * (0.5 - q_xx - q_zz);
	out[6] = 2 * s_y * (q_yz - q_xw);
	out[7] = l_y;
	out[8] = 2 * s_z * (q_xz - q_yw);
	out[9] = 2 * s_z * (q_yz + q_xw);
	out[10] = 2 * s_z * (0.5 - q_xx - q_yy);
	out[11] = l_z;

	return out;
}

// Multiply two transform matrices
std::array<float, 12> multiplyTransformMatrices(const std::array<float, 12>& a, const std::array<float, 12>& b)
{
	std::array<float, 12> out;

	for (uint8_t i = 0; i < 12; i += 4)
	{
		for (uint8_t j = 0; j < 4; ++j)
			out[i + j] = a[i] * b[j] + a[i + 1] * b[4 + j] + a[i + 2] * b[8 + j];
		out[i + 3] += a[i + 3];
	}

	return out;
}

// Recursively multiply transform matrices based on a parentage hierarchy
// Can re-implement this without recursion if you create a vector of parent indices
std::array<float, 12> makeTotalBoneMatrix(int16_t bone_idx, const std::vector<std::array<float, 12>>& bone_matrices, const std::vector<uint16_t>& bone_parents)
{
	uint16_t parent_idx = bone_parents[bone_idx];
	if (parent_idx == (uint16_t)-1)
		return std::array<float, 12> {1.f, 0.f, 0.f, 0.f,
		0.f, 1.f, 0.f, 0.f,
		0.f, 0.f, 1.f, 0.f};
	else
		return multiplyTransformMatrices(makeTotalBoneMatrix(parent_idx, bone_matrices, bone_parents), bone_matrices[bone_idx]);
}

// ViewInverse requires different version
// Abstract out the cofactor calculation?
inline std::array<float, 12> invertRotationLocationMatrix(const std::array<float, 12>& mat)
{
	// Invert the rotation matrix bit, which is just a transpose because it's unitary
	std::array<float, 12> out;
	out[0] = mat[0];
	out[1] = mat[4];
	out[2] = mat[8];

	out[4] = mat[1];
	out[5] = mat[5];
	out[6] = mat[9];

	out[8] = mat[2];
	out[9] = mat[6];
	out[10] = mat[10];

	// Now calculate the new locations by calculating the minor for the transpose entry + the appropriate cofactor
	// Only need to do this for the final row, since we've taken a shortcut for the unitary piece
	float r10_r21 = mat[4] * mat[9];
	float r10_r22 = mat[4] * mat[10];
	float r11_r22 = mat[5] * mat[10];
	float r12_r20 = mat[6] * mat[8];
	float r12_r21 = mat[6] * mat[9];
	float r20_r11 = mat[8] * mat[5];
	float r10_lz = mat[4] * mat[11];
	float r11_lz = mat[5] * mat[11];
	float r12_lz = mat[6] * mat[11];
	float r20_ly = mat[8] * mat[7];
	float r21_ly = mat[9] * mat[7];
	float r22_ly = mat[10] * mat[7];

	float det_10_21 = r10_r21 - r20_r11;
	float det_10_22 = r10_r22 - r12_r20;
	float det_10_23 = r10_lz - r20_ly;
	float det_11_22 = r11_r22 - r12_r21;
	float det_11_23 = r11_lz - r21_ly;
	float det_12_23 = r12_lz - r22_ly;

	out[3] = -mat[1] * det_12_23 + mat[2] * det_11_23 - mat[3] * det_11_22;
	out[7] = mat[0] * det_12_23 - mat[2] * det_10_23 + mat[3] * det_10_22;
	out[11] = -mat[0] * det_11_23 + mat[1] * det_10_23 - mat[3] * det_10_21;

	// Matrix is unitary, so no need to get a determinant

	return out;
}

// Matrix generators
std::array<float, 9> makeRotationMatrix(std::array<float, 3> position, std::array<float, 3> target, std::array<float, 3> up)
{
	std::array<float, 3> z = normalise<3>({ position[0] - target[0], position[1] - target[1], position[2] - target[2] });
	std::array<float, 3> x = normalise<3>(cross(up, z));
	std::array<float, 3> y = normalise<3>(cross(z, x));

	return
	{
		x[0],              y[0],              z[0],
		x[1],              y[1],              z[1],
		x[2],              y[2],              z[2]
	};
}

std::array<float, 16> makeViewMatrix(std::array<float, 3> position, std::array<float, 3> target, std::array<float, 3> up)
{
	std::array<float, 3> z = normalise<3>({ position[0] - target[0], position[1] - target[1], position[2] - target[2] });
	std::array<float, 3> x = normalise<3>(cross(up, z));
	std::array<float, 3> y = normalise<3>(cross(z, x));

	return
	{
					 x[0],              y[0],              z[0], 0.f,
					 x[1],              y[1],              z[1], 0.f,
					 x[2],              y[2],              z[2], 0.f,
		-dot(x, position), -dot(y, position), -dot(z, position), 1.f
	};
}

std::array<float, 16> invertViewMatrix(const std::array<float, 16>& view)
{
	return
	{
		  view[0],   view[1],   view[2],  view[3],
		  view[4],   view[5],   view[6],  view[7],
		  view[8],   view[9],  view[10], view[11],
		-view[12], -view[13], -view[14], view[15]
	};
}

std::array<float, 16> perspectiveMatrix(float fov, float aspect, float zNear, float zFar)
{
	float y_scale = cos(fov / 2) / sin(fov / 2);
	float x_scale = y_scale / aspect;
	float zDiff = zNear - zFar;
	return
	{
		x_scale,       0,                        0,  0,
			   0, y_scale,                        0,  0,
			   0,       0,   (zFar + zNear) / zDiff, -1,
			   0,       0, 2 * zFar * zNear / zDiff,  0
	};
}

std::array<float, 16> inversePerspectiveMatrix(float fov, float aspect, float zNear, float zFar)
{
	float y_scale = cos(fov / 2) / sin(fov / 2);
	float x_scale = y_scale / aspect;
	float zSum = zNear - zFar;
	float zDiff = zNear - zFar;
	float zProd = (2 * zFar * zNear);
	return
	{
		1/x_scale,          0,  0,             0,
			     0, 1/y_scale,  0,             0,
			     0,         0,  0, zDiff / zProd,
			     0,         0, -1,  zSum / zProd
	};
}


//template <typename valType>
//GLM_FUNC_QUALIFIER detail::tmat4x4<valType> ortho
//(
//	valType const& left,
//	valType const& right,
//	valType const& bottom,
//	valType const& top,
//	valType const& zNear,
//	valType const& zFar
//)
//{
//	detail::tmat4x4<valType> Result(1);
//	Result[0][0] = valType(2) / (right - left);
//	Result[1][1] = valType(2) / (top - bottom);
//	Result[2][2] = -valType(2) / (zFar - zNear);
//	Result[3][0] = -(right + left) / (right - left);
//	Result[3][1] = -(top + bottom) / (top - bottom);
//	Result[3][2] = -(zFar + zNear) / (zFar - zNear);
//	return Result;
//}