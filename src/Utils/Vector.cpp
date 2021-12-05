#include "Vector.hpp"


std::array<float, 3> cross(const std::array<float, 3>& vec_1, const std::array<float, 3>& vec_2)
{
	std::array<float, 3> out;
	out[0] =   vec_1[1] * vec_2[2] - vec_1[2] * vec_2[1];
	out[1] = -(vec_1[0] * vec_2[2] - vec_1[2] * vec_2[0]);
	out[2] =   vec_1[0] * vec_2[1] - vec_1[1] * vec_2[0];
	return out;
}

std::array<float, 4> mulQuats(const std::array<float, 4>& quat_1, const std::array<float, 4>& quat_2)
{
	const float& w_1 = quat_1[3];
	const float& w_2 = quat_2[3];
	float dot_prod                  = dot<3>({ quat_1[0], quat_1[1], quat_1[2] }, { quat_2[0], quat_2[1], quat_2[2] });
	std::array<float, 3> cross_prod = cross ({ quat_1[0], quat_1[1], quat_1[2] }, { quat_2[0], quat_2[1], quat_2[2] });

	return
	{
		(w_1 * quat_2[0]) + (w_2 * quat_1[0]) + cross_prod[0],
		(w_1 * quat_2[1]) + (w_2 * quat_1[1]) + cross_prod[1],
		(w_1 * quat_2[2]) + (w_2 * quat_1[2]) + cross_prod[2],
		(w_1 * w_2) - dot_prod
	};
}

void fixQuaternionSigns(std::vector<std::array<float, 4>>& quat_vec)
{
	// If the projection between a quaternion and its neighbour is negative, flip the neighbour
	// This can't be parallised since we have to flip each quaternion before deciding if we should
	// flip the next this way
	for (size_t i = 1; i < quat_vec.size(); ++i)
	{
		// Get a sign of -1 or 1 depending on whether the projection is < 0 or >= 0
		int8_t factor = 2*(!signbit(dot(quat_vec[i - 1], quat_vec[i]))) - 1;
		quat_vec[i] = { factor * quat_vec[i][0], factor * quat_vec[i][1], factor * quat_vec[i][2], factor * quat_vec[i][3] };
	}
}


// Inverts fewer quats but not convinced it's more efficient, computationally or memory-wise
// Maybe it will beat out the other one with SIMD or something? Not convinced this is a
// big cost though. Something to bear in mind in case it does become an issue.
void fixQuaternionSigns2(std::vector<std::array<float, 4>>& quat_vec)
{
	// Fill an array of boundaries: 1 if the quats are in the same cover of SO(3), 0 if not
	// Project one quaternion onto the other, and see if they are colinear or anti-colinear
	std::vector<bool> boundaries(quat_vec.size() - 1);
	for (size_t i = 0; i < quat_vec.size() - 1; ++i)
		boundaries[i] = !signbit(dot(quat_vec[i], quat_vec[i + 1]));

	// Deduce which half of SO(3) the quaternions live in: arbitarily call whatever half the
	// first quaternion lives in as '1', and the other half as '0'
	// To find this out, just use a simple truth table.
	// Quaternion lives in (1), and there is not a boundary flip (1) -> Second quaternion lives in (1)
	// Quaternion lives in (1), and there is a boundary flip     (0) -> Second quaternion lives in (0)
	// Quaternion lives in (0), and there is not a boundary flip (1) -> Second quaternion lives in (0)
	// Quaternion lives in (0), and there is a boundary flip     (0) -> Second quaternion lives in (1)
	// That's the same as [quat_2_half = NOT XOR(quat_1_half, is_there_a_boundary)]
	// ... so let's do that!
	size_t quat_half = 1;
	// We need to remember which quaternions live in which half, too
	std::array<std::vector<size_t>, 2> quaternion_indices;
	quaternion_indices[1].push_back(0);
	size_t num_in_same_half_as_first_quat = 1;
	for (size_t i = 1; i < quat_vec.size(); ++i)
	{
		quat_half = !(quat_half ^ boundaries[i - 1]);
		quaternion_indices[quat_half].push_back(i);
		num_in_same_half_as_first_quat += quat_half;
	}

	// Now... which half should we flip? Let's choose whichever half requires the least inversions.
	uint8_t half_to_flip = num_in_same_half_as_first_quat < (quat_vec.size() / 2);
	size_t num_to_flip = num_in_same_half_as_first_quat;
	if (!half_to_flip)
		num_to_flip = quat_vec.size() - num_to_flip;
	// Finally, do the flips
	for (size_t i = 0; i < num_to_flip; ++i)
	{
		auto quat_idx = quaternion_indices[half_to_flip][i];
		quat_vec[quat_idx] = { -quat_vec[quat_idx][0], -quat_vec[quat_idx][1], -quat_vec[quat_idx][2], -quat_vec[quat_idx][3] };
	}
}